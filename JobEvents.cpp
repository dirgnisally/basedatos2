#include "JobEvents.h"

JobEvents::JobEvents(){
	missing_info = 1;
}

string JobEvents::getTimestamp(){

	return timestamp;
}

int JobEvents::getMissing_info(){

	return missing_info;
}

int JobEvents::getJob_id(){

	return job_id;
}


int JobEvents::getEvent_type(){
	return event_type;
}

string JobEvents::getUser_name(){
	return user_name;

}

int JobEvents::getScheduling_class(){
	return scheduling_class;

}

string JobEvents::getJob_name(){
	return job_name;

}

string JobEvents::getLogical_name(){
	return logical_name;

}

void JobEvents::setTimestamp(string t){
	timestamp = t;
}
void JobEvents::setMissing_info(int m){
	missing_info = m;
}
void JobEvents::setJob_id(int ji){
	ob_id = ji;
}
void JobEvents::setEvent_type(int et){
	event_type = et;
}
void JobEvents::setUser_name(string un){
	user_name = un;
}

void JobEvents::setScheduling_class(int sc){
	scheduling_class = sc;
}

void JobEvents::setJob_name(string jn){
	job_name = jn;
}

void JobEvents::setLogical_name(string ln){
	logical_name = ln;
}

