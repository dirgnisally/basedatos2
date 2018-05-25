#include "MachineEvents.h"
MachineEvents::MachineEvents(){
	machine_id = 1;
}

string MachineEvents::getTimestamp(){

	return timestamp;
}
string MachineEvents::getMachine_id(){

	return machine_id;
}

int MachineEvents::getEvent_type(){

	return event_type;
}


int MachineEvents::getPlataform_ide(){
	return plataform_id;
}

int MachineEvents::getCapacity_cpu(){
	return capacity_cpu;

}
int MachineEvents::getCapacity_memory(){
	return capacity_memory;
}

void MachineEvents::setTimestamp(string t){
	timestamp = t;
}
void MachineEvents::setMachine_id(int m){
	machine_id = m;
}
void MachineEvents::setEvent_type(int e){
	event_type = e;
}
void MachineEvents::setPlataform_ide(int p){
	plataform_id = p;
}
void MachineEvents::setCapacity_cpu(int c){
	capacity_cpu = c;
}
void MachineEvents::setCapacity_memory(int ca){
	capacity_memory = ca;
}