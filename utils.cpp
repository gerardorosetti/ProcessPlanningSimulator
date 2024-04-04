/*
    2024
    This file contains the definition of the class GlobalVariables.
    This file contains the definition of the sleep for method
*/
#include <utils.hpp>

bool GlobalVariables::going = true;
double GlobalVariables::total_time;
uint64_t GlobalVariables::total_waited_time = 0;
uint64_t GlobalVariables::average_waited_time = 0;
uint64_t GlobalVariables::total_executed_time = 0;
uint64_t GlobalVariables::average_executed_time = 0;
uint64_t GlobalVariables::total_blocked_time = 0;
uint64_t GlobalVariables::average_blocked_time = 0;
uint64_t GlobalVariables::total_processes_completed = 0;
uint64_t GlobalVariables::total_processes_created = 0;
uint64_t GlobalVariables::total_processes_blocked = 0;
double GlobalVariables::CPU_usage = 0;
int GlobalVariables::lambda = 1500;
int GlobalVariables::tick = 10;

void GlobalVariables::reset()
{
    GlobalVariables::going = true;
    GlobalVariables::total_time = 0;
    GlobalVariables::total_waited_time = 0;
    GlobalVariables::average_waited_time = 0;
    GlobalVariables::total_executed_time = 0;
    GlobalVariables::average_executed_time = 0;
    GlobalVariables::total_blocked_time = 0;
    GlobalVariables::average_blocked_time = 0;
    GlobalVariables::total_processes_completed = 0;
    GlobalVariables::total_processes_created = 0;
    GlobalVariables::total_processes_blocked = 0;
    GlobalVariables::lambda = 1500;
    GlobalVariables::tick = 10;
}

//calculate the variables for showing at the stop of the simulation
void GlobalVariables::update()
{
    if(GlobalVariables::total_processes_blocked == 0)
    {
        GlobalVariables::average_blocked_time = 0;
    }else
    {
        GlobalVariables::average_blocked_time = GlobalVariables::total_blocked_time/GlobalVariables::total_processes_blocked;
    }
    if(GlobalVariables::total_processes_completed == 0)
    {
        GlobalVariables::average_executed_time = 0;
        GlobalVariables::average_waited_time = 0;
    }else
    {
        GlobalVariables::average_executed_time = GlobalVariables::total_executed_time/GlobalVariables::total_processes_completed;
        GlobalVariables::average_waited_time = GlobalVariables::total_waited_time/GlobalVariables::total_processes_completed;
    }
    GlobalVariables::CPU_usage = 100*(static_cast<double>(GlobalVariables::total_executed_time)/(GlobalVariables::total_time));
}

//sleep for based on the tick submitted
void sleep_for(int64_t time)
{

    auto tp1 = std::chrono::high_resolution_clock::now();
    uint64_t time_counter = 0;
    while(GlobalVariables::going)
    {
        auto tp2 = std::chrono::high_resolution_clock::now();
        time_counter += std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
        if(time_counter >= time)
        {
            break;
        }
        tp1 = tp2;
        std::this_thread::sleep_for(std::chrono::milliseconds(GlobalVariables::tick));
    }
}
