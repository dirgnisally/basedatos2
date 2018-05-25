#ifndef JobEvents
#define JobEvents

#include <iostream>
#include <string>
using namespace std;

class JobEvents
{
	private :
		string timestamp;
		int missing_info;
		int job_id;
		int event_type;
		string user_name;
		int scheduling_class;
		string job_name;
		string logical_name 
		
	public : 
		JobEvents();
		string getTimestamp();
		int getMissing_info();
		int getJob_id();
		int getEvent_type();
		string getUser_name();
		int getScheduling_class();
		string getJob_name();
		string getLogical_name();
		
		

		void setTimestamp(string t);
		void setMissing_info(int m);
		void setJob_id(int ji);
		void setEvent_type(int et);
		void setUser_name(string un);
		void setScheduling_class(int sc);
		void setJob_name(string jn);
		void setLogical_name(string ln);
		

};
#endif