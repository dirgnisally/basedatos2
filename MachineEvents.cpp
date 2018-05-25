#include "MachineEvents.h"

string MachineEvents12::getTimestamp(){

	return timestamp_;
}

int MachineEvents12::getMachine_id(){

	return machine_id;
}

int MachineEvents12::getEvent_type(){

	return event_type;
}


int MachineEvents12::getPlataform_ide(){
	return plataform_id;
}

int MachineEvents12::getCapacity_cpu(){
	return capacity_cpu;

}
int MachineEvents12::getCapacity_memory(){
	return capacity_memory;
}

void MachineEvents12::setTimestamp(string t){
	timestamp_ = t;
}

void MachineEvents12::setMachine_id(int m){
	machine_id = m;
}
void MachineEvents12::setEvent_type(int e){
	event_type = e;
}
void MachineEvents12::setPlataform_ide(int p){
	plataform_id = p;
}
void MachineEvents12::setCapacity_cpu(int c){
	capacity_cpu = c;
}
void MachineEvents12::setCapacity_memory(int ca){
	capacity_memory = ca;
}
