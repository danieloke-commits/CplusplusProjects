

#include <iostream>
#include "Task.h"
#include <algorithm>


using std::get; using std::max;


Task::Task()
{
    myTasks = {};
}

std::string trim(const std::string & mystr)
{
    std::size_t beg = mystr.find_first_not_of(" \t\n\r\f\v");
    if (beg == std::string::npos)
        return "";
    std::size_t end = mystr.find_last_not_of(" \t\n\r\f\v");
    return mystr.substr(beg, end - beg+1);

}



void Task::AddTask(const std::string & tk)
{
    if (tk.empty())
    {
        std::cout<<"Empty string entered, please try again";
    }
    else{
    if (myTasks.empty())
        myTasks = {};
    std::string trimmed = trim(tk);
    myTasks.push_back(TaskInfo(false,trimmed ));

    }
}

int  Task::RemoveTask(const std::string & tk )
{

     std::string trimmed = trim(tk);
    //1 indicates success 
    int val = 0;
    //if task is empty send message to requestor
    if (myTasks.empty())
    {
    std::cout<<" \nyou have  no items on listed on tasks\n"<<std::endl;
    return val;
    }
    
    if (trimmed.empty())
    {
        std::cout << " \nyou entered an empty string \n" << std::endl;
        return val;
    }
        
    //if task is not empty do a search
    else
    {  
        ////find string
        auto it = std::find_if(myTasks.begin(), myTasks.end(), [&](myTask & u) {  return u.taskName == trimmed; });



        //if found, return success
        if (it != myTasks.end())
        {
            myTasks.erase(it);
            return 1;
        }
            
        else
            std::cout << trimmed << " is not listed on your task\n" << std::endl;
    }
  
    return val;
}

bool Task::MarkCompletedTask(std::string s)
{
    bool isdone = false;
    s = trim(s);
    auto it = std::find_if(myTasks.begin(), myTasks.end(), [&](myTask& u) {  return u.taskName == s; });

    if (it == myTasks.end())
        std::cout << s << " is not listed on your task\n" << std::endl;
    else
    {
        (*it).isCompleted = true;
        isdone = true;
    }
    

    return isdone;
}

void Task::getTasks()const {

    for (auto a : myTasks)
        std::cout << a.taskName << std::endl;
    
}

	void Task::saveToFile (const std::string & s)
    {
        if (myTasks.empty())
            return;
        std::ofstream myfile(s);
        if (!myfile.is_open())
        {
            std::cerr<<"Unable to create file";
        }
        
        for (auto u: myTasks)
        {
            myfile<<trim(u.taskName)<<" | "<<(u.isCompleted == 0? "False": "True")<<"\n";
        }
    }
	void Task::loadFromFile (const std::string &s)
        {
                std::ifstream myfile (s);
                    if (myfile)
                    {
                        std::string result;
                        std::string complet;
                        std::string name;
                        bool completed;
                       while (std::getline(myfile,result) )
                       {
                        std::istringstream temp (result);
                         std::getline(temp,name, '|' );
                         std::getline(temp,complet);
                         name = trim(name);
                        complet == "True"? completed = true: completed = false;
                        myTasks.push_back( TaskInfo(completed, name));
                       }
                        
                    }
                


    }
    bool Task::mytaskIsEmpty()const
    {
        return myTasks.size()==0;
    }

int main()
{
    Task first{};
    first.loadFromFile("Tasks.txt");
    char temp = ' ';
    std::string res1{}, res2{};
    while (temp != 'q')
    {
        std::cout<<"-------------------------------\n"
         << "Enter 1 to add a Task \n"
        <<"Enter 2 to remove a task \nEnter 3 to mark a task completed \n"
        <<"Enter 4 to list all your tasks\nEnter q to end \n"
        <<"---------------------------------\n" << std::endl;

        std::cin >> temp;
       
        //discards everything until the end of the current line, so the next input starts fresh.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (temp)
        {

        case '1':
           
            std::cout << "Enter task here below \n";
            std::getline(std::cin, res1);
            if (!res1.empty())
            {
             first.AddTask(res1);
             std::cout << "Task was entered!\n\n";
            }
          
            break;
        case '2':
            std::cout <<"Enter task to remove below \n";
            std::getline(std::cin, res1);
             if (first.RemoveTask(res1) ==1)
            std::cout << "Task was removed!\n\n";
            break;
        case '3':
            std::cout << "Enter task below to mark as completed \n";
            std::getline(std::cin, res1);
            if (first.MarkCompletedTask(res1))
            std::cout << "Task was was marked completed!\n\n";
            break;
        case '4':
            if (!first.mytaskIsEmpty())
            {
            first.getTasks();
            std::cout << "\n\n";              
            }
            else
            std::cout<<"You currently have no task stored"<<std::endl;
            break;
        case 'q':
            first.saveToFile("Tasks.txt");
            
            break;
        default:
            std::cout << "Not a valid entry \n\n";
            break;
        }

    }

}


