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
1
10
1
44
1
44
44
672074(jobid unico) 2012242(registros en total)
*/
/* Calculamos el tamaño de los atributos */
void sizeAtrib(string file)
{
    long size, cont = 0;

    ifstream read(file.c_str());
    string linea;
    vector<string> palabras;
    vector<long> tam(8,0);
    map<string, int> jobId;

    getline(read, linea);
    while(!read.eof())
    {
        palabras.clear();
        split(palabras, linea, ',');

        for(int i = 0; i < 8; ++i){
            size = palabras[i].size();
            if(size > tam[i]) tam[i] = size; 
        }

        jobId[palabras[2]] = 0;

        cont++;
        getline(read, linea);
        
    }

    for(int i = 0; i < 8; ++i)
        cout << tam[i] << endl;

    cout << jobId.size() << " " << cont << endl;

    read.close();

 }

void createBinaryAndIndex(string file)
{
    long pos;

    ifstream read(file.c_str());
    string linea;
    vector<string> palabras;
    map<long, unordered_map<long, bool> > jobId; // Guarda los jobId con los registros

    FILE *binary;
    binary = fopen("job_events.bin","wb");

    for(long i = 0; i < 2012242; ++i){
        getline(read, linea);
        palabras.clear();
        split(palabras, linea, ',');

        JobEvents jobEvents;

        pos = ftell(binary);

        if(palabras[0].size() != 0) jobEvents.time = atol(palabras[0].c_str());
        if(palabras[1].size() != 0) jobEvents.missingInfo = palabras[1][0];
        jobEvents.jobId = atol(palabras[2].c_str());
        if(palabras[3].size() != 0) jobEvents.eventType = palabras[3][0];
        strcpy(jobEvents.user, palabras[4].c_str());
        if(palabras[5].size() != 0) jobEvents.schedulingClass = palabras[5][0];
        strcpy(jobEvents.jobName, palabras[6].c_str());
        strcpy(jobEvents.logicalJobName, palabras[7].c_str());

        fwrite (&jobEvents , sizeof(JobEvents), 1, binary);

        jobId[jobEvents.jobId][pos] = true;

    }

    cout << jobId.size() << endl;
    read.close();
    fclose(binary);

    ofstream es("jobId.key");
    es << 2012242 << endl;

    for(auto it1 = jobId.begin(); it1 != jobId.end(); ++it1){
        es << it1->first;
        for(auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
            es << " " << it2->first;
        
        es << endl; 
    }

    es.close();

 }

 int main()
 {
     
     createBinaryAndIndex("job_events.csv");
     return 0;
 }