/*
    2024
    This file contains the definition of the class GlobalVariables.
*/

#include "globalvariables.hpp"

bool GlobalVariables::going = true;
double GlobalVariables::total_time;
uint64_t GlobalVariables::total_waited_time = 0;
uint64_t GlobalVariables::average_waited_time = 0;
uint64_t GlobalVariables::total_executed_time = 0;
uint64_t GlobalVariables::average_executed_time = 0;
uint64_t GlobalVariables::total_blocked_time = 0;
uint64_t GlobalVariables::average_blocked_time = 0;
uint64_t GlobalVariables::total_processes_compleated = 0;
uint64_t GlobalVariables::total_processes_created = 0;
uint64_t GlobalVariables::total_processes_blocked = 0;
double GlobalVariables::CPU_usage = 0;
int GlobalVariables::lambda = 2000;
int GlobalVariables::tick = 1;

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
    GlobalVariables::total_processes_compleated = 0;
    GlobalVariables::total_processes_created = 0;
    GlobalVariables::total_processes_blocked = 0;
    GlobalVariables::lambda = 2000;
    GlobalVariables::tick = 1;
}
void GlobalVariables::update()
{
    if(GlobalVariables::total_processes_blocked == 0)
    {
        GlobalVariables::average_blocked_time = 0;
    }else
    {
        GlobalVariables::average_blocked_time = GlobalVariables::total_blocked_time/GlobalVariables::total_processes_blocked;
    }
    if(GlobalVariables::total_processes_compleated == 0)
    {
        GlobalVariables::average_executed_time = 0;
        GlobalVariables::average_waited_time = 0;
    }else
    {
        GlobalVariables::average_executed_time = GlobalVariables::total_executed_time/GlobalVariables::total_processes_compleated;
        GlobalVariables::average_waited_time = GlobalVariables::total_waited_time/GlobalVariables::total_processes_compleated;
    }
    GlobalVariables::CPU_usage = 100*(static_cast<double>(GlobalVariables::total_executed_time)/(GlobalVariables::total_time));
}
