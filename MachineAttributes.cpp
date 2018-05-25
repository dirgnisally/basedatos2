#include "MachineAttributes.h"

MachineAttributes::MachineAttributes(){
	machine_id = 1;
}

string MachineAttributes::getTimestamp(){

	return timestamp;
}

string MachineAttributes::getMachine_id(){

	return machine_id;
}

string MachineAttributes::getAttribute_name(){

	return attribute_name;
}


string MachineAttributes::getAttribute_value(){
	return attribute_value;
}

string MachineAttributes::getAttribute_deleted(){
	return attribute_deleted;

}

void MachineAttributes::setTimestamp(string t){
	timestamp = t;
}
void MachineAttributes::setMachine_id(int m){
	machine_id = m;
}
void MachineAttributes::setaAttribute_name(int an){
	attribute_name = an;
}
void MachineAttributes::setAttribute_value(int av){
	attribute_value = av;
}
void MachineAttributes::setAttribute_deleted(int ad){
	attribute_deleted = ad;
}
