/*
Fernanda Nava Moya - A01023896
Reto 1
Leer datos de archivo y los manipular
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

    void imprimirRecord()
    {
        cout << date << endl;
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
		Record r(valores[0], valores[1], valores[3], valores[2], valores[4], valores[5], valores[6], valores[7]);
		conexiones.push_back(r);
		valores.clear();    
    }
}

//COMPARADOR POR FUENTE
int byNameSource(Record a, Record b)
{
    if(a.nameSource < b.nameSource)
    {
        return -1;
    }
    else if (a.nameSource == b.nameSource)
    {
        return 0;
    }
    else
    {
        return 1;
    }    
}

//COMPARADOR POR DESTINO
int byDestiny(Record a, Record b)
{
    if(a.nameDestiny < b.nameDestiny)
    {
        return -1;
    }
    else if (a.nameDestiny == b.nameDestiny)
    {
        return 0;
    }
    else
    {
        return 1;
    }    
}

//COMPARADOR POR FECHA
int byDate(Record a, Record b)
{
    if(a.date < b.date)
    {
        return -1;
    }
    else if (a.date == b.date)
    {
        return 0;
    }
    else
    {
        return 1;
    }    
}

//COMPARADOR POR NOMBRE DE DESTINO
int byNameDestiny(Record a, Record b)
{
    if(a.nameDestiny < b.nameDestiny)
    {
        return -1;
    }
    else if (a.nameDestiny == b.nameDestiny)
    {
        return 0;
    }
    else
    {
        return 1;
    }    
}

//COMPARADOR POR PUERTO DESTINO
int byDesPort(Record a, Record b)
{
    if(a.desPort < b.desPort)
    {
        return -1;
    }
    else if (a.desPort == b.desPort)
    {
        return 0;
    }
    else
    {
        return 1;
    }    
}

//CLASE SORT
template <class T>
class Sort
{
public:
	virtual void sort(vector<T> &data, int comparador(T a, T b))=0;
	void intercambiar(vector<T> &data, int posI, int posJ)
    {
		T temp=data[posI];
		data[posI]=data[posJ];
		data[posJ]=temp;
	}
	
	void imprimir(vector<T> data)
    {
		for(int i=0; i<data.size(); i++)
        {
			cout<<data[i]<<",";
		}
		cout<<endl;
	}
};


//CLASEMERGE SORT = metodo de ordenamiento, hereda a clase Sort
//Se usa MERGE SORT por que es un poco mas rapido 
template <class T>
class MergeSort: public Sort<T>
{	
public:
	void sort(vector<T> &data, int comparador(T a, T b))
    {
		sortAux(data, 0, data.size()-1, comparador);
	}
private:
	void sortAux(vector<T> &data, int lo, int hi, int comparador(T a, T b))
    {
		if(lo>=hi)
        {
			return;
		}
		int mid=(lo+hi)/2;
		sortAux(data, lo, mid, comparador);
		sortAux(data, mid+1, hi, comparador);
		merge(data, lo, mid, hi, comparador);
	}	
	void merge(vector<T> &data, int low, int mid, int hi, int comparador(T a, T b))
    {
	int sizeA=mid-low+1;
	int sizeB=hi-mid;
	vector<T> copiaA;
	vector<T> copiaB;
	
	for(int i=0; i<sizeA; i++)
    {
		copiaA.push_back(data[low+i]);
	}
	
	for(int i=0; i<sizeB; i++)
    {
		copiaB.push_back(data[mid+i+1]);
	}
	
	int indexA=0;
	int indexB=0;
	int indexData=low;
	while(indexA<sizeA&&indexB<sizeB)
    {
		//if(copiaA[indexA]<copiaB[indexB])
		if(comparador(copiaA[indexA],copiaB[indexB])<0) 
        {
			data[indexData]=copiaA[indexA];
			indexA++;
		}
        else
        {
			data[indexData]=copiaB[indexB];
			indexB++;
		}
		indexData++;
	}
	
	while(indexA<sizeA)
    {
		data[indexData]=copiaA[indexA];
		indexA++;
		indexData++;
	}
	
	while(indexB<sizeB)
    {
		data[indexData]=copiaB[indexB];
		indexB++;
		indexData++;
	}
    }	
};

//BUSQUEDA BINARIA con COMPARADOR, cuando los datos ya estan ordenados, si no se usa una busqueda secuencial
template <class T>
int busquedaBinaria(vector<T> a, T buscado, int comparador(T a, T b)){
	int inicio=0;
	int fin=a.size()-1;
	while(fin>=inicio)
    {
		int medio=(inicio+fin)/2;
		if(comparador(a[medio],buscado)==0)
        {
			return medio;
		}
        else if(comparador(buscado,a[medio])>0)
        {
			inicio=medio+1;
		}
        else
        {
			fin=medio-1;
		}
	}
	return -1;
}

int main()
{
    leerDatos("/mnt/c/Users/ferna/OneDrive/Desktop/equipo_6.csv");
    cout << "---------- DATOS LEIDOS ----------" << endl;
    cout << "total de registros: " << conexiones.size() << endl;
    //cout << "total de puertos: " << conexiones_port.size() << endl;
    MergeSort<Record> s;

    //ENCONTRAR FECHAS
    s.sort(conexiones, byDate);    
    cout << "DATOS ORDENADOS" << endl;
    string dia = conexiones[0].date;
    cout << "----- FECHAS -----"<< endl;
    int dia_ = 1;
    cout << "dia " << dia_ << ": " << dia << endl;
    for (int i = 0; i <conexiones.size(); i++)
    {
        if(dia<conexiones[i+1].date)
        {
            dia_ ++;
            cout << "dia " << dia_ << ": " << conexiones[i+1].date << endl;
            dia = conexiones[i+1].date;
        }
    }

    //ENCONTRAR RECORDS EN FECHA
    string fecha = "11/8/2020";
    int e = 0;
    for(int i=0; i<conexiones.size(); i++)
    {
        if (conexiones[i].date == fecha)
        {
            e++;
        }        
    }    
    cout << "total en dia 2: " << e << endl;

    //ENCONTRAR USUARIO
    s.sort(conexiones, byNameSource);
    cout << "------- ECONTRAR USUARIO -------" << endl;
    string usuario = "server.reto.com";
    Record r("","","","",usuario,"","","");
    int pos_u=busquedaBinaria(conexiones,r,byNameSource);
    if(pos_u != -1)
    {
        cout << usuario << " ---> " << "ENCONTRADO" << endl;
    }
    else
    {
        cout << usuario << " ---> " << "NO ENCONTRADO" << endl;
    }
    
    //ENCONTRAR NOMBRE DESTINO
    s.sort(conexiones, byNameDestiny);
    cout << "---- ECONTRAR NOMBRE DESTINO ----" << endl;
    string nombre_des = "freemailserver.com";
    Record d("","","","","","","",nombre_des);
    int pos_des=busquedaBinaria(conexiones,d,byNameDestiny);
    if(pos_des != -1)
    {
        cout << nombre_des << " ---> " << "ENCONTRADO" << endl;
    }
    else
    {
        cout << nombre_des << " ---> " << "NO ENCONTRADO" << endl;
    }

    //LISTA DE PUERTOS DESTINO
    s.sort(conexiones, byDesPort);
    cout << "---- LISTA DE PUERTOS ----" << endl;
    string port = "";
    for (int i = 0; i < conexiones.size(); i++)
    {
        if (conexiones[i].desPort != "1000" && conexiones[i].desPort != port)
        {
            cout << conexiones[i].desPort << "  " << conexiones[i].nameDestiny << endl;
            port = conexiones[i].desPort;
        }        
    }

    /*
    for(int i=0; i<conexiones.size(); i++)
    {
        conexiones[i].imprimirRecord();
    }
    */
    return 0;
}