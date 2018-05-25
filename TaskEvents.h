#ifndef TaskEvents
#define TaskEvents

#include <iostream>
#include <string>
using namespace std;

class TaskEvents
{
	private :
		string timestamp;
		int missing_info;
		int job_id;
		int task_index;
		int machine_id;
		int event_type;
		string user_name;
		int scheduling_class
		int priority;
		int request_cpu;
		int request_ram;
		int request_disk;
		int diferente_machine;
		
		
	public : 
		TaskEvents();
		string getTimestamp();
		int getMissing_info();
		int getJob_id();

		int getTask_index();
		int getMachine_id();
		int getEvent_type();
		string getUser_name();
		int getScheduling_class();
		int getPriority();
		int getRequest_cpu();
		int getRequest_ram();
		int getRequest_disk();
		int getDiferent_machine();
		
		
		

		void setTimestamp(string t);
		void setMissing_info(int m);
		void setJob_id(int ji);
		void setTask_index(int ti);
		void setMachine_id(int mi);

		void setEvent_type(int et);
		void setUser_name(string un);
		void setPriority(int sc);
		void setRequest_cpu(int rc);
		void setRequest_ram(int rr);
		void setRequest_disk(int rd);
		void setDiferente_machine(int rm);



		

};
#endif