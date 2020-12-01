/*
Fernanda Nava Moya
Reto 4
*/
#include <iostream>
#include <string>
#include <fstream> //libreta para usar ifstream
#include <sstream> //pasar flujo de datos por pedazo
#include <vector>
#include <list>
#include <unordered_map> //asiganr key y un value (key:value)
#include <map>

using namespace std;

//CLASE RECORD
class Record
{
public:
    string date;
    string time;
    string sourceIP;
    int sourcePort;
    string nameSource;
    string desIP;
    int desPort;    
    string nameDestiny;

    Record(string date_, string time_, string sourceIP_, string sourcePort_, string nS, string desIP_, string desPort_, string nD)
    {
        date = date_;
        time = time_;
        sourceIP = sourceIP_;

        if(sourcePort_ == "-")
        {
            sourcePort = 0;
        }
        else
        {
            try
            {
                sourcePort = stoi(sourcePort_);
            }
            catch(const std::invalid_argument& ia)
            {
                sourcePort = 0;
                cout << "error" << sourcePort_ << endl;
            }            
        } 

        nameSource = nS;
        desIP = desIP_;  

        if(desPort_ == "-")
        {
            desPort = 0;
        }
        else
        {
            try
            {
                desPort = stoi(desPort_);
            }
            catch(const std::invalid_argument& ia)
            {
                desPort = 0;
                cout << "error" << desPort_ << endl;
            }            
        }
           
        nameDestiny = nD;
    }

    //Record(){}

    void imprimirRecord()
    {
        cout << date << "   " << time << "   " << sourceIP << "   " << sourcePort << "   " << nameSource << "   " << desIP << "   " << desPort << "   " << nameDestiny << endl; 
    }
};

//CLASE NODO
template <class T>
class Nodo
{
public:
	T value; //numero de veces accedido en dicha fecha
    string name; //nombre del sitio
	Nodo<T> *izq;
	Nodo<T> *der;
	
	Nodo()
	{
		value=NULL;
		izq=NULL;
		der=NULL;
	}
	
	Nodo(T val, string name_)
	{
        name = name_;
		value=val;
		izq=NULL;
		der=NULL;
	}
};

//CLASE BST
template <class T>
class BST
{
public:
	Nodo<T> *root;
	
	BST()
	{
		root=NULL;
	}
	
	void insertar(T value, string name)
	{
		Nodo<T> *nuevo=new Nodo<T>(value, name);
		if(root==NULL)
		{
			root=nuevo;
		}
		else
		{
			insertar(nuevo, root);
		}
	}
	
	void insertar(Nodo<T> *nuevo, Nodo<T> *temp)
	{
		if(nuevo->value==temp->value)//si el valor es igual, terminamos
		{
            //sitios[nuevo->value].push_back(nuevo->name); //añade los nombres de los sitios para que no se pierdan
			return;
		}
		else if(nuevo->value < temp->value)//si el valor es menor que el valor de temp
		{
			if(temp->izq==NULL)//si el izquierdo es nulo, ahi agregamos
			{
				temp->izq=nuevo;
			}
			else
			{
				insertar(nuevo, temp->izq); //sino, temp=temp->izq
			}
		}
		else //si el valor es mayor que el valor de temp
		{
			if(temp->der==NULL) //si el derecho es nulo, ahi agregamos
			{
				temp->der=nuevo;
                //sitios[nuevo->value].push_back(nuevo->name);
			}
			else
			{
				insertar(nuevo, temp->der);//sino, temp=temp->der
			}
		}
	}

    void inorder(){
		inorder(root);
		cout<<endl;
	} 
	
	void inorder(Nodo<T> *temp)
	{
		if(temp!=NULL)
		{
			inorder(temp->izq);
			cout<< endl << temp->name << ": " << temp->value << " ";
			inorder(temp->der);
		}
	}
};   

//FUNCION QUE GUARDA VALORES EN VECTOR CONEXIONES
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

//FUNCION PARA ENCONTRAR reto.com
bool checkReto(string name)
{
    string reto = "reto.com";
    int j = 1, len = 4;
    for (int i = 1; i < name.size(); i++)
    {
        if (name[i] == reto[j] && name[i - 1] == reto[j - 1])
        {
            j++;
        }
    }
    if (j == len)
    {
        return true; //tiene reto.com
    }
    return false; //no tiene reto.com
}

//CLASE PARA ASIGNAR PUERTO, IP Y NOMBRE DE UNA CONEXION
class infoConexion
{
public:
    int remotePort;
    string remoteIP;
    string remoteName;

    infoConexion(int rP, string rIP, string rN)
    {
        remotePort = rP;
        remoteIP = rIP;
        remoteName = rN;
    }
};

class ConexionesComputadora
{
public:
    string ip;
    string name;
    list<infoConexion> entrantes; //lista de conexiones entrantes con sus puertos,ip y nombre
    list<infoConexion> salientes; //lista de conexiones salientes con sus puertos,ip y nombre
    int numEntrantes = 0;

    ConexionesComputadora(){}

    ConexionesComputadora(string ip, string name)
    {
        this->ip = ip;
        this->name = name;
    }

    void nuevaEntrante(int port, string ip, string name)
    {
        infoConexion a(port, ip, name); //crea una conexion
        entrantes.push_front(a); //conexion a entrantes
        numEntrantes++;
    }

    void nuevaSaliente(int port, string ip, string name)
    {
        infoConexion a(port, ip, name); //crea una conexion
        salientes.push_back(a); //conexion a salientes
    }
      
};

unordered_map<string, ConexionesComputadora> cc;
unordered_map<string, int> cE;
unordered_map<string, int> conexionesPorDia(string date)
{
    cE.clear();
    cc.clear();
    for(Record r : conexiones)
    {
        if(r.date == date && r.nameDestiny != "-" && checkReto(r.nameDestiny) == false)
        {
            if(cc.find(r.nameDestiny) == cc.end())
            {
                ConexionesComputadora a(r.desIP, r.nameDestiny);
                cc[r.nameDestiny] = a;
            }
            cc[r.nameDestiny].nuevaEntrante(r.sourcePort, r.sourceIP, r.nameSource);
        }
    }

    //se crea un unordered_map de las conexiones entrantes
    for(auto it = cc.begin(); it != cc.end(); it++)
    {
        cE[it->first] = cc[it->first].numEntrantes;
    }
    
    /*
    for(auto it = cE.begin(); it != cE.end(); it++)
    {
        cout << it->first << ": " << it->second;
        cout << endl;
    }
    */

    return cE;
}  

/*
Esta función debe imprimir los n sitios con más accesos en esa fecha. 
Para ello, puedes usar la función conexionesPorDia y 
debes agregar los sitios a un BST utilizando como parametro de ordenamiento
la cantidad de conexiones entrantes. 
*/
bool top(int n, string date)
{
    conexionesPorDia(date);
    BST<int> sitio;
    map<int, vector<string>> sitios; //mapa que guarda nombre de los sitio con el mismo numero de visitas
    for(auto it = cE.begin(); it != cE.end(); it++)
    {
        sitio.insertar(it->second, it->first);
        sitios[it->second].push_back(it->first);
    }

    /*
    for(auto it = sitios.begin(); it != sitios.end(); it++)
    {
        cout << it->first << ": ";
        for(string a : sitios[it->first])
        {
            cout << a << ",";
        }
        cout << endl;
    }
    */

    int counter = 0; //n veces que se repite 
    for(auto it = sitios.rbegin(); it != sitios.rend(); it++)
    {
        if(counter < n)
        {
            cout << it->first << ":";
            for(string s : sitios[it->first])
            {
                cout << s << ", ";
                counter++;          
            }
            if(counter >= n)
            {
                cout << endl;
                return true;
            } 
            cout << endl;
        }
        else
        {
            return true;
        }            
    }
    return false;
}

int main()
{
    leerDatos("/mnt/c/Users/ferna/OneDrive/Desktop/tec/3er Semestre/estructuraDatos/nuevo6.csv");
    cout << "---------- DATOS LEIDOS ----------" << endl;

    //string fecha = ("10-8-2020");
    //top(5, fecha);

    //CREAR VECTOR CON TODAS LAS FECHAS
    vector<string> dates;
    dates.push_back(conexiones[0].date);
    for(Record r : conexiones)
    {
        if(dates[dates.size()-1] != r.date)
        {
            dates.push_back(r.date);
        }
    }
    
    /*
    for (int i = 0; i < dates.size(); i++)
    {
        cout << dates[i] << endl;
    }
    */

    //IMPRIMIR TOP 5 DE TODAS LAS FECHAS
    for (int i = 0; i < dates.size(); i++)
    {
        cout << "---- " << dates[i] << " ----" << endl;
        top(5, dates[i]);
        cout << endl;
    }
}
