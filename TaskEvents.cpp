#include "TaskEvents.h"

TaskEvents::TaskEvents(){
	missing_info = 1;
}

string TaskEvents::getTimestamp(){

	return timestamp;
}

int TaskEvents::getMissing_info(){

	return missing_info;
}

int TaskEvents::getJob_id(){

	return job_id;
}


int TaskEvents::getEvent_type(){
	return event_type;
}

string TaskEvents::getUser_name(){
	return user_name;

}

int TaskEvents::getScheduling_class(){
	return scheduling_class;

}

string TaskEvents::getJob_name(){
	return job_name;

}

string TaskEvents::getLogical_name(){
	return logical_name;

}

void TaskEvents::setTimestamp(string t){
	timestamp = t;
}
void TaskEvents::setMissing_info(int m){
	missing_info = m;
}
void TaskEvents::setJob_id(int ji){
	ob_id = ji;
}
void TaskEvents::setEvent_type(int et){
	event_type = et;
}
void TaskEvents::setUser_name(string un){
	user_name = un;
}

void TaskEvents::setScheduling_class(int sc){
	scheduling_class = sc;
}

void TaskEvents::setJob_name(string jn){
	job_name = jn;
}

void TaskEvents::setLogical_name(string ln){
	logical_name = ln;
}

