#ifndef MachineEvents
#define MachineEvents

#include <iostream>
#include <string>
using namespace std;

class MachineEvents12
{
	private :
		string timestamp_;
		int machine_id;
		int event_type;
		int plataform_id;
		int capacity_cpu;
		int capacity_memory;
	public : 
		MachineEvents12();
		string getTimestamp();
		int getMachine_id();
		int getEvent_type();
		int getPlataform_ide();
		int getCapacity_cpu();
		int getCapacity_memory();

		void setTimestamp(string t);
		void setMachine_id(int m);
		void setEvent_type(int e);
		void setPlataform_ide(int p);
		void setCapacity_cpu(int c);
		void setCapacity_memory(int ca); 

};
#endif