#ifndef TASK_H
#define TASK_H


#include <stdio.h>
#include <vector>
#include <string>
#include <tuple>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>

struct TaskInfo
{
	
	TaskInfo(bool a, const std::string & b)
	{
		isCompleted = a;
		taskName =b;
	}
	bool isCompleted;
	std::string taskName;

};

class Task {

private:
	  using myTask =TaskInfo ;
	
	std::vector<myTask> myTasks;


public:
	Task( );
	void AddTask( const std::string &);
	int RemoveTask( const std::string &);
	void getTasks()const;
	bool MarkCompletedTask(std::string);
	void saveToFile (const std::string &);
	void loadFromFile (const std::string &);
	bool mytaskIsEmpty() const;

};
#endif

