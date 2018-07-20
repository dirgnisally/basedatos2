#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <string>
#include <string.h>
#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cmath>


using namespace std;

mutex iomutex;

const int PORT_SERVER1 = 7100;
const char IP_SERVER1[] = "192.168.3.210";

const int PORT_SERVER0 = 7000;
const char IP_SERVER0[] = "192.168.3.210";

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

/******************************************************************************************/

//select *from machine_attributes where machineId=5
//delete from machine_attributes where machineId=5
//update machine_attributes set time=123456 where machineId=5

//select *from task_constraints where jobId=3418375
//delete from task_constraints where jobId=3418375
//update task_constraints set time=12345 where jobId=3418375
void server0(){
	struct sockaddr_in servidor;
	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr(IP_SERVER0);
	servidor.sin_port = htons(PORT_SERVER0);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	int conexion = connect(cliente, (struct sockaddr*)&servidor, sizeof(servidor));
	if (conexion != 0) {
		cout << "No se pudo conectar" << endl;
		return ;
	}

	char query[100] = "select *from machine_attributes where machineId=5";
	int size = 100;
	int n;

	char rp[3000];
    char *p = &rp[0];
	 
	send(cliente, &query, size, 0);
	recv(cliente, &rp, 3000, 0);

	bool select = true;
	if(select){
		MachineAttributes job;
		size = sizeof(MachineAttributes);

		memcpy(&n, &rp, sizeof(n));
		for(int i = 0; i < n; ++i){
			memcpy(&job, p + 4 + (i * size) , sizeof(MachineAttributes));
			cout << job.time << endl;
		}
	}

	memcpy(&n, &rp, sizeof(n));
	cout << "estado: " << n << endl;
}

//select *from job_events where jobId=3418309
//delete from job_events where jobId=3418309
//update job_events set time=523433 where jobId=3418309

//select *from machine_events where machineId=27
//delete from machine_events where machineId=27
//update machine_events set time=523433 where machineId=27
void server1(){
	struct sockaddr_in servidor;
	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr(IP_SERVER1);
	servidor.sin_port = htons(PORT_SERVER1);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	int conexion = connect(cliente, (struct sockaddr*)&servidor, sizeof(servidor));
	if (conexion != 0) {
		cout << "No se pudo conectar" << endl;
		return ;
	}

	char query[100] = "select *from job_events where jobId=3418309";
	int size = 100;
	int n;

	char rp[3000];
    char *p = &rp[0];
	 
	send(cliente, &query, size, 0);
	recv(cliente, &rp, 3000, 0);

	bool select = true;
	if(select){
		JobEvents job;
		size = sizeof(JobEvents);

		memcpy(&n, &rp, sizeof(n));
		for(int i = 0; i < n; ++i){
			memcpy(&job, p + 4 + (i * size) , sizeof(JobEvents));
			cout << job.time << endl;
		}
	}

	memcpy(&n, &rp, sizeof(n));
	cout << "estado: " << n << endl;
}


int main(){
	
	thread hilo1(server0);    
  	//thread hilo2(server1);

  	cout<<"Iniciaron los hilos" << endl;

  	hilo1.join();
  	//hilo2.join();

  	cout<<"Terminaron los hilos" << endl;
	 

	return 0;
}