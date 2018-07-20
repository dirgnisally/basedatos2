#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <vector>

using namespace std;

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

/*
13
10
1
44
4
7
12583(machine id unico) 37780
*/

void sizeAtrib(string file)
{
    long tie, job, sizeUser = 0, sizeName = 0, sizeLogical = 0, size;
    long cont = 0;

    ifstream read(file.c_str());
    string linea;
    vector<string> palabras;
    vector<long> tam(6,0);
    //map<string, int> jobId;
    map<string, int> machineId;

    getline(read, linea);
    while(!read.eof())
    {
        palabras.clear();
        split(palabras, linea, ',');

        for(int i = 0; i < 6; ++i){
            size = palabras[i].size();
            if(size > tam[i]) tam[i] = size; 
        }

        //jobId[palabras[2]] = 0;
        machineId[palabras[1]] = 0;

        cont++;
        if(cont % 10000 == 0){
            cout << cont << endl;
        }
        getline(read, linea);
        
    }

    for(int i = 0; i < 6; ++i)
        cout << tam[i] << endl;

    cout << machineId.size() << " " << cont << endl;

    read.close();

 }

 void createBinaryAndIndex(string file)
{

    long pos = 0;

    ifstream read(file.c_str());
    string linea;
    vector<string> palabras;
    //unordered_map<long, vector<long> > jobId; // Guarda los jobId con los registros
    unordered_map<long, vector<long> > machineId; // Guarda los machineId con los registros
    //unordered_map<long, vector<long> > taskIndex; // Guarda los taskIndex con los registros

    FILE *binary;
    binary = fopen("machine_events.bin","wb");

    for(long i = 0; i < 37780; ++i){
        getline(read, linea);
        palabras.clear();
        split(palabras, linea, ',');

        MachineEvents machineEvents;

        pos = ftell(binary);

        if(palabras[0].size() != 0) machineEvents.time = atol(palabras[0].c_str());
        if(palabras[1].size() != 0) machineEvents.machineId = atol(palabras[1].c_str());
        if(palabras[2].size() != 0) machineEvents.eventType = palabras[2][0];
        strcpy(machineEvents.platformId, palabras[3].c_str());
        if(palabras[4].size() != 0) machineEvents.cpus = atof(palabras[4].c_str());
        if(palabras[5].size() != 0) machineEvents.memory = atof(palabras[5].c_str());

        fwrite (&machineEvents , sizeof(machineEvents), 1, binary);

        //jobId[taskEvents.jobId].push_back(pos);
        machineId[machineEvents.machineId].push_back(pos);
        //taskIndex[taskEvents.taskIndex].push_back(pos);


        if(i % 20000 == 0) cout << i << endl;

    }

    //cout << jobId.size() << endl; //" " << machineId.size() << " " << taskIndex.size() << endl;
    read.close();
    fclose(binary);

    /*
    ofstream esJob("jobId.key");
    esJob << 144648282 << endl;
    for(auto it1 = jobId.begin(); it1 != jobId.end(); ++it1){
        esJob << it1->first;
        for(int i = 0; i <  it1->second.size(); ++i)
            esJob << " " << it1->second[i];
        
        esJob << endl; 
    }
    esJob.close();*/

    
    ofstream esMa("machineId.key");
    esMa << 37780 << endl;
    for(auto it1 = machineId.begin(); it1 != machineId.end(); ++it1){
        esMa << it1->first;
        for(int i = 0; i <  it1->second.size(); ++i)
            esMa << " " << it1->second[i];
        
        esMa << endl; 
    }
    esMa.close();

    /*
    ofstream esTa("taskIndex.key");
    esTa << 144648282 << endl;
    for(auto it1 = taskIndex.begin(); it1 != taskIndex.end(); ++it1){
        esTa << it1->first;
        for(int i = 0; i <  it1->second.size(); ++i)
            esTa << " " << it1->second[i];
        
        esTa << endl; 
    }
    esTa.close();
    */

 }

 int main()
 {
     
     createBinaryAndIndex("machine_events.csv");
     return 0;
 }