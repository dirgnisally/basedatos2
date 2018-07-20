#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

/*****************************BLOQUEOS EN Delete***************************/
mutex muAttributes;
mutex muContraints;

/*********************INDICES*********************/
unordered_map<long, vector<long> > machine_attributes_machineId;
unordered_map<long, vector<long> > task_constraints_jobId;

const int PORT = 7000;

/****************CLIENTES CONECTADOS(CONCURRENCIA)****************************/
vector<thread> clients;

int open_socket();
int bind_to_port(int, int);

/**********************************CARGAR INDICES*******************************************/

struct MachineAttributes{
    long time; //Integer
    long machineId; //Integer
    char attributeName[45]; //String 44 espacios
    char attributeValue[45]; //String 44 espacios
    bool attributeDeleted;
    bool active; //Para saber si el registro fue borrado(false) o no(true)

    MachineAttributes(){
        time = -1;
        machineId = -1;
        active = true;
    }
};

struct TaskConstraints{
    long time; //Integer
    long jobId; //Integer
    int taskIndex; //Integer
    char comparisonOperator; //Integer
    char attributeName[45]; //String 44 espacios
    char attributeValue[45]; //String 44 espacios
    bool active; //Para saber si el registro fue borrado(false) o no(true)

    TaskConstraints(){
        time = -1;
        jobId = -1;
        active = true;
    }
};

//Divide una cadena de acuerdo a un caracter
void split(vector<string> &v, const string& s, const char& c)
{
	string buff = "";
	
	for(auto n:s){
		if(n != c) buff += n; 
        else { v.push_back(buff); buff = ""; }
	}
	v.push_back(buff);
}

//Carga los indices de machineId
void loadMachineAttributes()
{
    ifstream read("machine_attributes_machineId.key");
    string linea;
    vector<string> palabras;

    getline(read, linea);
    getline(read, linea);
    while(!read.eof())
    {
        palabras.clear();
        split(palabras, linea, ' ');

        long id = atol(palabras[0].c_str());
        for(int i = 1; i < palabras.size(); ++i)
            machine_attributes_machineId[id].push_back(atol(palabras[i].c_str()));

        getline(read, linea);        
    }

    read.close();
}

void loadTaskConstraints()
{
    ifstream read("task_constraints_jobId.key");
    string linea;
    vector<string> palabras;

    getline(read, linea);
    getline(read, linea);
    while(!read.eof())
    {
        palabras.clear();
        split(palabras, linea, ' ');

        long id = atol(palabras[0].c_str());
        for(int i = 1; i < palabras.size(); ++i)
            task_constraints_jobId[id].push_back(atol(palabras[i].c_str()));

        getline(read, linea);        
    }

    read.close();
}

void loadIndexs()
{
    loadMachineAttributes();
    loadTaskConstraints();
}

/*****************************************************************************/

//select *from machine_attributes where machineID=5
int selectMachineAttributes(const char *query, MachineAttributes *lista)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = machine_attributes_machineId[id];

    int cont = 0;

    FILE *binary;
    binary = fopen("machine_attributes.bin", "rb");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&lista[i], sizeof(MachineAttributes), 1, binary);
        cont++;
        if(cont >= 10) break;
    }
    fclose(binary);

    return cont;
}

//delete from machine_attributes where machineID=5
void deleteMachineAttributes(const char *query)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = machine_attributes_machineId[id];

    MachineAttributes attributes;

    FILE *binary;
    binary = fopen("machine_attributes.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&attributes, sizeof(MachineAttributes), 1, binary);
        attributes.active = false;

        fseek(binary, pos, SEEK_SET);
        fwrite(&attributes, sizeof(MachineAttributes), 1, binary);
    }

    fclose(binary);

    muAttributes.lock();
    machine_attributes_machineId[id].clear();
    muAttributes.unlock();

}

//update machine_attributes set time=123456 where machineID=5
void updateMachineAttributes(const char *query)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    cadena = cadena.substr(pos + 1);
    pos = cadena.find(" ");
    string tiemS = cadena.substr(0, pos);
    long tiem = atol(tiemS.c_str());

    pos = cadena.find("=");
    cadena = cadena.substr(pos + 1);
    long id = atol(cadena.c_str());

    vector<long> regs = machine_attributes_machineId[id];

    MachineAttributes attributes;

    FILE *binary;
    binary = fopen("machine_attributes.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&attributes, sizeof(MachineAttributes), 1, binary);
        attributes.time = tiem;

        fseek(binary, pos, SEEK_SET);
        fwrite(&attributes, sizeof(MachineAttributes), 1, binary);
    }

    fclose(binary);

}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//select *from task_constraints where jobId=3418375
int selectTaskConstraints(const char *query, TaskConstraints *lista)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = task_constraints_jobId[id];

    int cont = 0;

    FILE *binary;
    binary = fopen("task_constraints.bin", "rb");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&lista[i], sizeof(TaskConstraints), 1, binary);
        cont++;
        if(cont >= 10) break;
    }
    fclose(binary);

    return cont;
}

//delete from task_constraints where jobId=3418375
void deleteTaskConstraints(const char *query)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = task_constraints_jobId[id];

    TaskConstraints contraints;

    FILE *binary;
    binary = fopen("task_constraints.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&contraints, sizeof(TaskConstraints), 1, binary);
        contraints.active = false;

        fseek(binary, pos, SEEK_SET);
        fwrite(&contraints, sizeof(TaskConstraints), 1, binary);
    }

    fclose(binary);

    muContraints.lock();
    task_constraints_jobId[id].clear();
    muContraints.unlock();

}

//update task_constraints set time=12345 where jobId=3418375
void updateTaskConstraints(const char *query)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    cadena = cadena.substr(pos + 1);
    pos = cadena.find(" ");
    string tiemS = cadena.substr(0, pos);
    long tiem = atol(tiemS.c_str());

    pos = cadena.find("=");
    cadena = cadena.substr(pos + 1);
    long id = atol(cadena.c_str());

    vector<long> regs = task_constraints_jobId[id];

    TaskConstraints contraints;

    FILE *binary;
    binary = fopen("task_constraints.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&contraints, sizeof(TaskConstraints), 1, binary);
        contraints.time = tiem;

        fseek(binary, pos, SEEK_SET);
        fwrite(&contraints, sizeof(TaskConstraints), 1, binary);
    }

    fclose(binary);

}

/*****************************************************************************/

int transactionsPriority(const char *query)
{
    switch(query[0]){
        case 's':
            return 99;
        case 'u':
            return 66;
        case 'd':
            return 33;
        case 'i':
            return 1; 
    }

    return 1;
}

void consulta(int connect, int priority, string query)
{
    sched_param sch;
    sch.sched_priority = priority;
    if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch)) {
        cout << "Error: " << strerror(errno) << endl;
    }

    size_t pos = query.find("machine_attributes");

    char rp[3000];
    char *p = &rp[0];

    if(pos != string::npos)
    {
        //Entonces es machine_attributes
        if(query[0] == 's'){
            MachineAttributes lista[10];
            int n = selectMachineAttributes(query.c_str(), lista);
            int size = sizeof(MachineAttributes);

            memcpy(p, &n, sizeof(n));
            for(int i = 0; i < n; ++i)
                memcpy( p + 4 + (i * size), &lista, sizeof(MachineAttributes) );        

            send(connect, &rp, 3000, 0);        
        }
        else if(query[0] == 'd'){
            deleteMachineAttributes(query.c_str());
            int n = 0;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0); 
        }
        else if(query[0] == 'u'){
            updateMachineAttributes(query.c_str());
            int n = 0;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0); 
        }
        else{
            int n = -1;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0);
        }

    }
    else{
        //Entonces es task_constraints
        if(query[0] == 's'){
            TaskConstraints lista[10];
            int n = selectTaskConstraints(query.c_str(), lista);
            int size = sizeof(TaskConstraints);

            memcpy(p, &n, sizeof(n));
            for(int i = 0; i < n; ++i)
                memcpy( p + 4 + (i * size), &lista, sizeof(TaskConstraints) );        

            send(connect, &rp, 3000, 0);        
        }
        else if(query[0] == 'd'){
            deleteTaskConstraints(query.c_str());
            int n = 0;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0); 
        }
        else if(query[0] == 'u'){
            updateTaskConstraints(query.c_str());
            int n = 0;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0); 
        }
        else{
            int n = -1;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0);
        }
    }
    
    close(connect);
}

int main()
{
    loadIndexs();
    
    int listener = open_socket();
    if(listener == -1) {
        cout << "Error al conectar..." << endl;
        return -1;    
    }        
    
    if(bind_to_port(listener, PORT) == -1){
        return -1;
    }
    
    if(listen(listener, 10) == -1) {
        cout << "No es posible usar el puerto..." << endl;
        return -1;
    }
    
    cout << "Enlazado al puerto..." << endl;
    
    while(1){
        struct sockaddr_storage client;
        unsigned int addres_size = sizeof(client);
        cout << "Esperando al cliente..." << endl;
        int connect = accept(listener, (struct sockaddr*) &client, &addres_size);
        if(connect == -1) {
            cout << "Error al conectar..." << endl;
            return -1;    
        }  
        
        char query[100];
        int size = 100;

        int nBytes = recv(connect, &query, size, 0);
        if(nBytes < 0){
            cout << "Se deconnecto" << endl;
            return -1;
        }

        int priority = transactionsPriority(query);
        cout << "Atendiendo al cliente..." << query << " connection: " << connect << endl;

        string queryS = query;

        clients.push_back(thread(consulta, connect, priority, queryS));
        
    }
    
    
    return 0;
}

int open_socket(){
    int s =  socket(PF_INET, SOCK_STREAM, 0);
    if(s == -1) {
        cout << "Error al conectar..." << endl;  
    } 
    return s;
}

int bind_to_port(int socket, int port){
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int reuse = 1;
    if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1){
        cout << "No es posible reusar socket..." << endl;
        return -1; 
    }
    
    int c = bind(socket, (struct sockaddr*)&name, sizeof(name));
    if(c == -1){
        cout << "No es posible enlazar al puerto, ya esta en uso..." << endl; 
         return -1;
    }
    return 0;
}

