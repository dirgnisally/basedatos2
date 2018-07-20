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

/********************************************************/
mutex muJob;
mutex muMachine;

/*********************INDICES*********************/
unordered_map<long, vector<long> > job_events_jobId;
unordered_map<long, vector<long> > machine_events_machineId;

const int PORT = 7100;

/****************CLIENTES CONECTADOS****************************/
vector<thread> clients;

int open_socket();
int bind_to_port(int, int);

/**********************************CARGAR INDICES*******************************************/
struct JobEvents{
    long time; //Integer
    char missingInfo; //Integer
    long jobId; //Integer
    char eventType; //Integer
    char user[45]; //String 44 espacios
    char schedulingClass; //Integer
    char jobName[45]; //String 44 espacios
    char logicalJobName[45]; //String 44 espacios
    bool active; //Para saber si el registro fue borrado(false) o no(true)

    JobEvents(){
        time = -1;
        jobId = -1;
        active = true;
    }
};

struct MachineEvents{
    long time; //Integer
    long machineId; //Integer
    char eventType; //Integer
    char platformId[45]; //String 44 espacios
    float cpus; //Float
    float memory; //Float
    bool active; //Para saber si el registro fue borrado(false) o no(true)

    MachineEvents(){
        time = -1;
        machineId = -1;
        active = true;
    }
};

void split(vector<string> &v, const string& s, const char& c)
{
	string buff = "";
	
	for(auto n:s){
		if(n != c) buff += n; 
        else { v.push_back(buff); buff = ""; }
	}
	v.push_back(buff);
}

void loadJobEvents()
{
    ifstream read("job_events_jobId.key");
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
            job_events_jobId[id].push_back(atol(palabras[i].c_str()));

        getline(read, linea);        
    }

    read.close();
}

void loadMachineEvents()
{
    ifstream read("machine_events_machineId.key");
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
            machine_events_machineId[id].push_back(atol(palabras[i].c_str()));

        getline(read, linea);        
    }

    read.close();
}

void loadIndexs()
{
    loadJobEvents();
    loadMachineEvents();
}

/*****************************************************************************/

//select *from job_events where jobId=3418309
int selectJob(const char *query, JobEvents *lista)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = job_events_jobId[id];

    int cont = 0;

    FILE *binary;
    binary = fopen("job_events.bin", "rb");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&lista[i], sizeof(JobEvents), 1, binary);
        cont++;
        if(cont >= 10) break;
    }
    fclose(binary);

    return cont;
}

//delete from job_events where jobId=3418309
void deleteJob(const char *query)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = job_events_jobId[id];

    JobEvents job;

    FILE *binary;
    binary = fopen("job_events.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&job, sizeof(JobEvents), 1, binary);
        job.active = false;

        fseek(binary, pos, SEEK_SET);
        fwrite(&job, sizeof(JobEvents), 1, binary);
    }

    fclose(binary);

    muJob.lock();
    job_events_jobId[id].clear();
    muJob.unlock();

}

//update job_events set time=523433 where jobId=3418309
void updateJob(const char *query)
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

    vector<long> regs = job_events_jobId[id];

    JobEvents job;

    FILE *binary;
    binary = fopen("job_events.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&job, sizeof(JobEvents), 1, binary);
        job.time = tiem;

        fseek(binary, pos, SEEK_SET);
        fwrite(&job, sizeof(JobEvents), 1, binary);
    }

    fclose(binary);

}


/*****************************************************************************/

//select *from machine_events where machineId=27
int selectMachine(const char *query, MachineEvents *lista)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = machine_events_machineId[id];

    int cont = 0;

    FILE *binary;
    binary = fopen("machine_events.bin", "rb");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&lista[i], sizeof(MachineEvents), 1, binary);
        cont++;
        if(cont >= 10) break;
    }
    fclose(binary);

    return cont;
}

//delete from machine_events where machineId=27
void deleteMachine(const char *query)
{
    string cadena = query;
    size_t pos = cadena.find("=");
    string idS = cadena.substr(pos + 1);
    long id = atol(idS.c_str());

    vector<long> regs = machine_events_machineId[id];

    MachineEvents machine;

    FILE *binary;
    binary = fopen("machine_events.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&machine, sizeof(MachineEvents), 1, binary);
        machine.active = false;

        fseek(binary, pos, SEEK_SET);
        fwrite(&machine, sizeof(MachineEvents), 1, binary);
    }

    fclose(binary);

    muMachine.lock();
    machine_events_machineId[id].clear();
    muMachine.unlock();

}

//update machine_events set time=523433 where machineId=27
void updateMachine(const char *query)
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

    vector<long> regs = machine_events_machineId[id];

    MachineEvents machine;

    FILE *binary;
    binary = fopen("machine_events.bin", "r+b");

    for(int i = 0; i < regs.size(); ++i){
        long pos = regs[i];
        fseek(binary, pos, SEEK_SET);
        fread(&machine, sizeof(MachineEvents), 1, binary);
        machine.time = tiem;

        fseek(binary, pos, SEEK_SET);
        fwrite(&machine, sizeof(MachineEvents), 1, binary);
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

    size_t pos = query.find("job_events");

    char rp[3000];
    char *p = &rp[0];

    if(pos != string::npos)
    {
        //Entonces es job_events
        if(query[0] == 's'){
            JobEvents lista[10];
            int n = selectJob(query.c_str(), lista);
            int size = sizeof(JobEvents);

            memcpy(p, &n, sizeof(n));
            for(int i = 0; i < n; ++i)
                memcpy( p + 4 + (i * size), &lista, sizeof(JobEvents) );        

            send(connect, &rp, 3000, 0);        
        }
        else if(query[0] == 'd'){
            deleteJob(query.c_str());
            int n = 0;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0); 
        }
        else if(query[0] == 'u'){
            updateJob(query.c_str());
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
        //Entonces es machin
        if(query[0] == 's'){
            MachineEvents lista[10];
            int n = selectMachine(query.c_str(), lista);
            int size = sizeof(MachineEvents);

            memcpy(p, &n, sizeof(n));
            for(int i = 0; i < n; ++i)
                memcpy( p + 4 + (i * size), &lista, sizeof(MachineEvents) );        

            send(connect, &rp, 3000, 0);        
        }
        else if(query[0] == 'd'){
            deleteMachine(query.c_str());
            int n = 0;
            memcpy(p, &n, sizeof(n));
            send(connect, &rp, 3000, 0); 
        }
        else if(query[0] == 'u'){
            updateMachine(query.c_str());
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

