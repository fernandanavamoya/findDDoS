/*
Fernanda Nava Moya
Reto 2
*/
#include <iostream>
#include <string>
#include <fstream> //libretia para usar ifstream
#include <sstream> //pasar flujo de datos por pedazo
#include <vector>

using namespace std;

//CLASE RECORD
class Record
{
public:
    string date;
    string time;
    string sourceIP;
    string sourcePort;
    string nameSource;
    string desIP;
    string desPort;    
    string nameDestiny;

    Record(string date_, string time_, string sourceIP_, string sourcePort_, string nS, string desIP_, string desPort_, string nD)
    {
        date = date_;
        time = time_;
        sourceIP = sourceIP_;
        sourcePort = sourcePort_;
        nameSource = nS;
        desIP = desIP_;   
        desPort = desPort_;    
        nameDestiny = nD;
    }

    Record(){}

    void imprimirRecord()
    {
        cout << date << "   " << time << "   " << sourceIP << "   " << sourcePort << "   " << nameSource << "   " << desIP << "   " << desPort << "   " << nameDestiny << endl; 
    }
};

//FUNCION PQUE GUARDA VALORES EN VECTOR CONEXIONES
vector<Record> conexiones;
void leerDatos(string path) //path = doc de datos
{
    //ifstream = input file stream (para leer datos)
    ifstream fileIn;
    fileIn.open(path); //se abre el archivo fileIn y se leen los datos path
    string line, partes; 
    vector<string>valores;

    while(getline(fileIn, line)) 
    {
        istringstream sIn(line); //lee linea por partes

        //TRUE = no se ha acabado, FALSE = se acabo
        while (getline(sIn, partes, ','))//lee las partes de la linea separada por una coma
        {
            valores.push_back(partes);
        }
        //se quita '\r' para poder comparar
        if(valores[7].find('\r')!=valores[7].npos){//quita '\r'
			valores[7]=valores[7].substr(0, valores[7].size()-1);
		}
		Record r(valores[0], valores[1], valores[2], valores[3], valores[4], valores[5], valores[6], valores[7]);
		conexiones.push_back(r);
		valores.clear();    
    }
}

template<class T>
class Nodo
{
public:
	T value;
	Nodo<T> *next;
		
	Nodo(T val)
    {
		value=val;
		next=NULL;
	}
};

template <class T>
class LinkedList
{
public:
	Nodo<T> *root;
	
	LinkedList()
    {
		root=NULL;
	}
	
	void append(Nodo<T> *nuevo)
    {
		if(root==NULL)
        {
			root=nuevo;
		}
        else
        {
			Nodo<T> *temp=root;
			while(temp->next!=NULL)
            {
				temp=temp->next;
			}
			temp->next=nuevo;
		}
	}
	
	void append(T value)
    {
		Nodo<T> *nuevo=new Nodo<T>(value);
		append(nuevo);
	}
	
	void insert(Nodo<T> *nuevo, int pos){
		if(root==NULL){
			root=nuevo;
		}else if(pos==0){
			nuevo->next=root;
			root=nuevo;
		}else{
			Nodo<T> *temp=root;
			int i=0;
			while(temp->next!=NULL && i<pos-1){
				temp=temp->next;
				i++;
			}
			nuevo->next=temp->next;
			temp->next=nuevo;
		}
	}
	
	void insert(T value, int pos){
		Nodo<T> *nuevo=new Nodo<T>(value);
		insert(nuevo, pos);
	}
	
	void print()
    {
		Nodo<T> *temp=root;
		while(temp!=NULL)
        {
            cout << temp->value << ", ";
			temp=temp->next;
		}	
		cout<<endl;
	}

    //DEVUELVE EL SIZE DE LA LINKED LIST
	int size()
	{
		Nodo<T> *temp = root;
		int count = 0;//contador
		while(temp != NULL)
		{
			count ++;
			temp = temp -> next;//se actualiza el valor de temp al siguiente nodo
		}
		return count;
	}

    T getElementAt(int pos){
		Nodo<T> *temp=root;
		int i=0;
		while(temp!=NULL && i<pos){
			temp=temp->next;
			i++;
		}
        return temp->value;
		//if(i==pos && temp!=NULL){
		//	return temp->value;
		//}
	}
};

template<class T>
class ConexionesComputadora
{
public:
    string IP; //ip entrante
    string Nombre; //usuario - servicio
    LinkedList<T> ConexionesEntrantes; //ip destino
    LinkedList<T> ConexionesSalientes; //ip fuente
};


//172.26.185.0
int main()
{
    string num;
    cout << "num: "; 
    cin >> num; //89
    string ip = "172.26.185." + num;
    cout<<ip<<endl;

    leerDatos("/mnt/c/Users/ferna/OneDrive/Desktop/nuevo6.csv");
    cout << "---------- DATOS LEIDOS ----------" << endl;


    //172.26.185.89
    ConexionesComputadora<Record> a;
    a.IP = ip;
    for(int i = 0; i <conexiones.size(); i++)
    {
        Record e(conexiones[i].date, conexiones[i].time, conexiones[i].sourceIP, conexiones[i].sourcePort, conexiones[i].nameSource, conexiones[i].desIP, conexiones[i].desPort, conexiones[i].nameDestiny);
        if(conexiones[i].desIP == ip)
        {
            a.ConexionesEntrantes.append(e);
            //cout << "A" << endl;            
        }
        else if (conexiones[i].sourceIP == ip)
        {
            a.ConexionesSalientes.append(e);
        }
    }

    //larry.reto.com
    ConexionesComputadora<Record> b;
    string user = "larry.reto.com";
    b.Nombre = user;
    
    cout << "-----------CONEXIONES------------" << endl;
    for (int i = 0; i < conexiones.size(); i++)
    {
        Record g(conexiones[i].date, conexiones[i].time, conexiones[i].sourceIP, conexiones[i].sourcePort, conexiones[i].nameSource, conexiones[i].desIP, conexiones[i].desPort, conexiones[i].nameDestiny);
        if (conexiones[i].nameDestiny == user)
        {
            b.ConexionesEntrantes.append(g);
            //g.imprimirRecord();
        }   
        else if (conexiones[i].nameSource == user)
        {
            b.ConexionesSalientes.append(g);
            //g.imprimirRecord();
        }     
    }

    /*
    //ULTIMA IP    
    b.ConexionesEntrantes.getElementAt(b.ConexionesEntrantes.size());
    b.ConexionesSalientes.getElementAt(b.ConexionesSalientes.size());
    */

    //TOTAL DE CONEXIONES ENTRANTES
    cout << "TOTAL DE CONEXIONES ENTRANTES: " << b.ConexionesEntrantes.size() << endl;
    //TOTAL DE CONEXIONES SALIENTES
    cout << "TOTAL DE CONEXIONES SALIENTES: " << b.ConexionesSalientes.size() <<endl;

    //CONEXION A 3 VECES SEGUIDAS A UN SERVICIO
    int j = 1;
    int k = 2;
    for(int i = 0; i < b.ConexionesSalientes.size(); i++)
    {
        if(b.ConexionesSalientes.getElementAt(i).nameDestiny == b.ConexionesSalientes.getElementAt(j).nameDestiny && b.ConexionesSalientes.getElementAt(i).nameDestiny == b.ConexionesSalientes.getElementAt(k).nameDestiny)
        {
            b.ConexionesSalientes.getElementAt(i).imprimirRecord();
        }
        j++;
        k++;
    }  
    
    return 0;
}