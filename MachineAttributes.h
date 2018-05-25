#ifndef MachineAttributes
#define MachineAttributes

#include <iostream>
#include <string>
using namespace std;

class MachineAttributes
{
	private :
		string timestamp;
		int machine_id;
		string attribute_name;
		string attribute_value;
		string attribute_deleted 
		
	public : 
		MachineAttributes();
		string getTimestamp();
		int getMachine_id();
		string getAttribute_name();
		string getAttribute_value();
		string getAttribute_deleted();
		

		void setTimestamp(string t);
		void setMachine_id(int m);
		void setAttribute_name(string an);
		void setAttribute_value(string av);
		void setAttribute_deleted(string ad);
		

};
#endif