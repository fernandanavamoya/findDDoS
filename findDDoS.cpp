#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <list>
#include <string>
#include <fstream> //libreta para usar ifstream
#include <sstream> //pasar flujo de datos por pedazo

using namespace std;

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

template <class T>
class SNode
{
public:
    T value;
    SNode *next;
    SNode() {}
    SNode(T val)
    {
        value = val;
        next = 0;
    }
};

//CLASE STACK
template <class T>
class Stack
{
private:
    SNode<T> *head;

public:
    Stack() { head = 0; }
    void push(T value)
    {
        SNode<T> *t = new SNode<T>(value);
        t->next = head;
        head = t;
    }

    T pop()
    {
        SNode<T> *t = head;
        if (head->next)
            head = head->next;
        else
            head = 0;
        T val = t->value;
        delete t;
        return val;
    }

    //funcion que checa si el stack esta empty
    bool isEmpty()
    {
        if (head)
            return false;
        return true;
    }
};

template <class T>
class QNode
{
public:
    T value;
    QNode *next;
    QNode *prev;
    QNode(){};
    QNode(T val)
    {
        value = val;
        next = NULL;
        prev = NULL;
    }
};

//CLASE QUEUE
template <class T>
class Queue
{
public:
    QNode<T> *front;
    QNode<T> *back;
    Queue()
    {
        front = NULL;
        back = NULL;
    }

    //agrega values al queue
    void enqueue(T value)
    {
        QNode<T> *nuevo = new QNode<T>(value);
        if (back == NULL)
        {
            back = nuevo;
            front = back;
        }
        else
        {
            back->next = nuevo;
            nuevo->prev = back;
            back = nuevo;
        }
    }

    //regresa el primer valor del queue
    T dequeue()
    {
        QNode<T> *t = front;
        if (front->next)
            front = front->next;
        else
            front = back = 0;
        T value = t->value;
        delete t;
        return value;
    }

    //chehca si el queue esta vacio
    bool isEmpty()
    {
        if (back)
        {
            return false;
        }
        return true;
    }
};

//FUNCION PARA ENCONTRAR reto.com
bool checkReto(string searchHere, string searching)
{
    int j = 1, finalLen = searching.size();
    bool found;
    for (int i = 1; i < searchHere.size(); i++)
    {

        if (searchHere[i] == searching[j] && searchHere[i - 1] == searching[j - 1])
            j++;
    }
    if (j >= finalLen)
        return true;
    return false;
}

//CLASE NODO
template <class T>
class Nodo
{
public:
    T value; //nombre computadora
    bool visited;
    int x = 0;
    unordered_map<Nodo<T> *, int> siguientes; //Nodo IP fer: 1
    Nodo(T val)
    {
        value = val;
        visited = false; //si ya fue visitado
    }

    //agrega un arco dirigido con peso
    //peso = # de conexiones
    void agregarArcoDirigidoConPeso(Nodo<T> *sig, int peso)
    {
        if (siguientes.find(sig) == siguientes.end())
            siguientes[sig] = peso;
    }

    //agrega un arco dirigido con peso de 1
    void agregarArcoDirigido(Nodo<T> *sig)
    {
        agregarArcoDirigidoConPeso(sig, 1);
    }

    void imprimirNodo()
    {
        cout << value << ":";
        for (auto nodoSiguiente : siguientes)
        {
            cout << nodoSiguiente.first->value << "," << nodoSiguiente.second << ";";
        }
        cout << endl;
    }
};

//CLASE GRAFO
template <class T>
class Graph
{
public:
    unordered_map<T, Nodo<T> *> nodos; //IP brian: nodo de brian

    int checkConexion(T searching)
    {
        int count = 0;
        for (auto x : nodos)
        {
            for (auto it = x.second->siguientes.begin(); it != x.second->siguientes.end(); it++)
            {
                if (it->first->value == searching)
                {
                    count++;
                }
            }
        }
        return count;
    }

    void agregarNodo(T value)
    {
        if (nodos.find(value) == nodos.end())
        {
            Nodo<T> *nuevo = new Nodo<T>(value);
            nodos[value] = nuevo;
        }
    }

    //agrega un arco dirigido de nodo1 a nodo2 con peso
    void agregarArcoDirigidoConPeso(T nodo1, T nodo2, int peso)
    {
        if (nodos.find(nodo1) != nodos.end() && nodos.find(nodo2) != nodos.end())
        {
            nodos[nodo1]->agregarArcoDirigidoConPeso(nodos[nodo2], peso);
        }
        else
        {
            cout << "Alguno de los 2 nodos no se encuentra en el grafo" << endl;
        }
    }

    //agrega un arco dirigido de nodo1 a nodo2 con peso de 1
    void agregarArcoDirigido(T nodo1, T nodo2)
    {
        agregarArcoDirigidoConPeso(nodo1, nodo2, 1);
    }

    //agrega un arco dirigido de nodo1 a nodo2 con peso y viceversa
    void agregarArcoConPeso(T nodo1, T nodo2, int peso)
    {
        agregarArcoDirigidoConPeso(nodo1, nodo2, peso);
        agregarArcoDirigidoConPeso(nodo2, nodo1, peso);
    }

    //agrega un arco dirigido de nodo1 a nodo2 con peso de 1 y viceversa
    void agregarArco(T nodo1, T nodo2)
    {
        agregarArcoConPeso(nodo1, nodo2, 1);
    }

    void imprimirGrafo()
    {
        for (auto parValorNodo : nodos)
        {
            parValorNodo.second->imprimirNodo();
            cout << endl;
        }
    }

    //BREATH FRIST SEARCJ
    bool BFS(T v1, T v2)
    {
        quitarVisita();
        Queue<T> adyacentes; //queue de nodos adyacentes al nodo actual (temp)
        adyacentes.enqueue(v1);
        while (adyacentes.isEmpty() == false) //miestras el queue no este vacio
        {
            T temp = adyacentes.dequeue();
            //cout << temp << endl;
            nodos[temp]->visited = true; //es visitado
            if (temp == v2)
            {
                return true; //termina while
            }
            else
            {
                for (auto t : nodos[temp]->siguientes)
                {
                    if (t.first->visited == false) //si no ha sido visitado
                    {
                        adyacentes.enqueue(t.first->value); //agrega al queue
                    }
                }
            }
        }
        return false; //si no lo encuentra regresa false
    }

    //DEPTH FIRST SEARCH
    bool DFS(T nodo1, T nodo2)
    {
        quitarVisita();
        Stack<T> adyacentes;                  //crea un stack s
        adyacentes.push(nodo1);               //agrega nodo actual
        while (adyacentes.isEmpty() == false) //miestra el stack no este vacio
        {
            T temp = adyacentes.pop();
            //cout << temp << endl;
            nodos[temp]->visited = true; //es visitado
            if (temp == nodo2)
            {
                return true; //se encuentra y regresa true
            }
            else
            {
                for (auto t : nodos[temp]->siguientes)
                {
                    if (t.first->visited == false) //si no han sido visitados
                    {
                        adyacentes.push(t.first->value); //se agrega el stack
                    }
                }
            }
        }
        return false; //si no lo encuentra regresa false
    }

    void quitarVisita()
    {
        for (auto t : nodos)
        {
            t.second->visited = false;
        }
    }

    int conexionesPorDia(string date, string name)
    {
        int count = 0;
        vector<string> dominios;

        for (Record r : conexiones)
        {
            if (r.date == date && name == r.nameDestiny)
            {
                //se crea un iterador que va de un vector donde se añaden las computadora que cumplen con la condición
                //si el nombre de la computadora ya se encuntra en este vector entonces no añade 1 al contador count
                vector<string>::iterator it = find(dominios.begin(), dominios.end(), r.nameSource);
                if (it == dominios.end())
                {
                    count++;
                    dominios.push_back(r.nameSource);
                }
            }
        }
        return count;
    }
};

int main()
{
    leerDatos("/mnt/c/Users/ferna/OneDrive/Desktop/tec/3er Semestre/estructuraDatos/nuevo6.csv");
    cout << "---------- DATOS LEIDOS ----------" << endl;

    Graph<string> interna;
    int count;
    //AGREGAR COMPUTADORAS INTERNAS A GRAFO
    for (Record r : conexiones)
    {
        if (checkReto(r.desIP, "172.26.185.") == true && checkReto(r.sourceIP, "172.26.185.") == true)
        {
            interna.agregarNodo(r.sourceIP);
            interna.agregarNodo(r.desIP);
            interna.agregarArcoDirigido(r.sourceIP, r.desIP);
        }
    }

    cout << interna.nodos["172.26.185.121"]->siguientes.size() << endl;
    //interna.imprimirGrafo();
    cout << "brian.reto.com tiene " << interna.checkConexion("172.26.185.121") << " conexiones entrantes" << endl;

    Graph<string> sitios;
    //AGREGAR GRAFO DE SITIOS WEB
    for (Record r : conexiones)
    {
        if (r.nameDestiny.find("reto.com") == string::npos)
        {
            //cout << "i" << endl;
            sitios.agregarNodo(r.nameSource);  //computadora interna
            sitios.agregarNodo(r.nameDestiny); //sitios web
            sitios.agregarArcoDirigido(r.nameSource, r.nameDestiny);
        }
    }
    cout << "nytimes.com tiene " << sitios.checkConexion("nytimes.com") << " conexiones entrantes" << endl;
    //sitios.nodos["brian.reto.com"]->imprimirNodo();

    //CREA VECTOR CON TODAS LAS FECHAS DEL ARCHIVO
    vector<string> dates;
    dates.push_back(conexiones[0].date);
    for (Record r : conexiones)
    {
        if (dates[dates.size() - 1] != r.date)
        {
            dates.push_back(r.date);
        }
    }

    //IMPRIME NUEMERO DE VECES QUE SE CONECTO EN TAL FECHA
    for (int i = 0; i < dates.size(); i++)
    {
        cout << dates[i] << ": " << sitios.conexionesPorDia(dates[i], "nytimes.com");
        cout << endl;
    }

    return 0;
}