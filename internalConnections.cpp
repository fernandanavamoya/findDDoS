/*
Fernanda Nava Moya
Reto 3
*/
#include <iostream>
#include <string>
#include <fstream> //libreta para usar ifstream
#include <sstream> //pasar flujo de datos por pedazo
#include <list>
#include <set>
#include <vector>
#include <unordered_map> //asiganr key y un value (key:value)

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

        if (sourcePort_ == "-")
        {
            sourcePort = 0;
        }
        else
        {
            try
            {
                sourcePort = stoi(sourcePort_);
            }
            catch (const std::invalid_argument &ia)
            {
                sourcePort = 0;
                cout << "error" << sourcePort_ << endl;
            }
        }

        nameSource = nS;
        desIP = desIP_;

        if (desPort_ == "-")
        {
            desPort = 0;
        }
        else
        {
            try
            {
                desPort = stoi(desPort_);
            }
            catch (const std::invalid_argument &ia)
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

//FUNCION QUE GUARDA VALORES EN VECTOR CONEXIONES
vector<Record> conexiones;
void leerDatos(string path) //path = doc de datos
{
    //ifstream = input file stream (para leer datos)
    ifstream fileIn;
    fileIn.open(path); //se abre el archivo fileIn y se leen los datos path
    string line, partes;
    vector<string> valores;

    while (getline(fileIn, line))
    {
        istringstream sIn(line); //lee linea por partes

        //TRUE = no se ha acabado, FALSE = se acabo
        while (getline(sIn, partes, ',')) //lee las partes de la linea separada por una coma
        {
            valores.push_back(partes);
        }
        //se quita '\r' para poder comparar
        if (valores[7].find('\r') != valores[7].npos)
        { //quita '\r'
            valores[7] = valores[7].substr(0, valores[7].size() - 1);
        }
        Record r(valores[0], valores[1], valores[2], valores[3], valores[4], valores[5], valores[6], valores[7]);
        conexiones.push_back(r);
        valores.clear();
    }
}

bool find(string searchHere, string searching)
{
    int j = 1, finalLen = 8;
    bool found;
    for (int i = 1; i < searchHere.size(); i++)
    {

        if (searchHere[i] == searching[j] && searchHere[i - 1] == searching[j - 1])
            j++;
    }
    if (j == finalLen)
        return true;
    return false;
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
        return true;
    }
    return false;
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

    ConexionesComputadora() {}

    ConexionesComputadora(string ip, string name)
    {
        this->ip = ip;
        this->name = name;
    }

    void nuevaEntrante(int port, string ip, string name)
    {
        infoConexion a(port, ip, name); //crea una conexion
        entrantes.push_front(a);        //conexion a entrantes
    }

    void nuevaSaliente(int port, string ip, string name)
    {
        infoConexion a(port, ip, name); //crea una conexion
        salientes.push_back(a);         //conexion a salientes
    }
};

int main()
{
    leerDatos("/mnt/c/Users/ferna/OneDrive/Desktop/tec/3er Semestre/estructuraDatos/nuevo6.csv");
    cout << "---------- DATOS LEIDOS ----------" << endl;

    //CONJUNTO DE STRINGS SIN reto.com
    set<string> nombres;
    for (Record r : conexiones)
    {
        if (checkReto(r.nameDestiny) == false || checkReto(r.nameSource) == false)
        {
            nombres.insert(r.nameSource);
        }
    }

    /*
    for(string i : nombres)
    {
        cout << i << endl;
    }
    cout << nombres.size() << endl;
    */

    //asigna que cc es de tipo <string, ConexionesComputadora>
    //la llave va a ser un string: nombre
    //el valor va a ser una ConexionesComputadora: conexiones
    unordered_map<string, ConexionesComputadora> cc;
    for (Record r : conexiones) //hasta pasar todas las conexiones
    {
        if (r.sourceIP != "-")
        {
            //si la llave no se encuentra en map iterator al end,
            if (cc.find(r.sourceIP) == cc.end())
            {
                ConexionesComputadora a(r.sourceIP, r.nameSource);
                cc[r.sourceIP] = a; //r.name : a(r.sourceIP, r.nameSource)
            }
            //si la llave se encuentra en map iterator,
            //se crea una nueva conexion saliente con ese sourceName
            cc[r.sourceIP].nuevaSaliente(r.desPort, r.desIP, r.nameDestiny); //r.name : a(r.sourceIP, r.nameSource), salientes.push_back(a)
        }
        if (r.nameDestiny != "-")
        {
            if (cc.find(r.nameDestiny) == cc.end())
            {
                ConexionesComputadora a(r.desIP, r.nameDestiny);
                cc[r.nameDestiny] = a; //r.name : a(r.sourceIP, r.nameSource)
            }
            cc[r.nameDestiny].nuevaEntrante(r.sourcePort, r.sourceIP, r.nameSource); //r.name : a(r.destinyIP, r.nameDestiny), entrantes.push_back(a)
        }
    }

    //3.COMPUTADORAS CON CONEXIONES ENTRANTES
    int cont = 0;
    for (auto it = cc.begin(); it != cc.end(); it++)
    {
        if (cc[it->first].entrantes.size() > 0 && find(cc[it->first].name, "reto.com"))
            cont++;
    }
    if (cont == 0)
        cout << "No hay conexeiones entrantes en los hosts de reto.com" << endl;
    else
        cout << "Entrantes: " << cont << endl;

    //4. IP ENTRANTES DE COMPUTADORAS INTERNAS
    vector<string> hostsInfectados;
    for (auto it = cc.begin(); it != cc.end(); it++)
    {
        if (cc[it->first].salientes.size() != 0)
        {
            //se crea una lista con los valores de la lista salientes para poder acceder a ella
            list<infoConexion> consalientes = cc[it->first].salientes;
            for (auto v : consalientes)
            {
                if (find(v.remoteName, "reto.com") == true)
                {
                    cout << cc[it->first].name << ": " << cc[it->first].ip << endl;
                    hostsInfectados.push_back(cc[it->first].name);
                    break;
                }
            }
        }
    }

    //6
    if (hostsInfectados.size() > 0)
    {
        for (auto it = cc.begin(); it != cc.end(); it++)
        {
            //se crea una lista con los valores de la lista salientes para poder acceder a ella
            list<infoConexion> consalientes = cc[it->first].salientes;
            for (auto v : consalientes)
            {
                if (v.remoteName == "x42olekrcpb929dv2iwn.xxx " || v.remoteName == "m2v28pfij68yh7kcaobi.com")
                {
                    std::cout << "SI SE METIO" << std::endl;
                }
            }
        }
    }

    return 0;
}
