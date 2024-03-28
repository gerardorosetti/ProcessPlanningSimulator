/*
    2024
    This file contains the declaration of the class GlobalVariables.
*/
#ifndef GLOBALVARIABLES_HPP
#define GLOBALVARIABLES_HPP

#include <iostream>
#include <thread>

class GlobalVariables
{
public:
    static bool going;
    static double total_time; //easy peace
    static uint64_t total_waited_time;//done pending for checking
    static uint64_t average_waited_time;//done
    static uint64_t total_executed_time;//done
    static uint64_t average_executed_time; //done
    static uint64_t total_blocked_time;//done
    static uint64_t average_blocked_time;//done
    static uint64_t total_processes_compleated;
    static uint64_t total_processes_created;//done
    static uint64_t total_processes_blocked;//done
    static double CPU_usage;
    static int lambda;
    static int tick;
    static void reset();
    static void update();
};


#endif // GLOBALVARIABLES_HPP
