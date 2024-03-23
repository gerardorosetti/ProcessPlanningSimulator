#ifndef GLOBALVARIABLES_HPP
#define GLOBALVARIABLES_HPP

#include <iostream>

class GlobalVariables
{
public:
    static bool going;
    static uint64_t total_time;
    static uint64_t total_waited_time;
    static uint64_t total_executed_time;
    static uint64_t total_blocked_time;
    static uint64_t total_processes_compleated;
    static uint64_t total_processes_created;
    static uint64_t total_processes_blocked;
};

#endif // GLOBALVARIABLES_HPP
