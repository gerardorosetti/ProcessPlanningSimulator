#ifndef GLOBALVARIABLES_HPP
#define GLOBALVARIABLES_HPP

#include <iostream>


class GlobalVariables
{
public:
    static bool going;
    static uint64_t total_time; //easy peace
    static uint64_t total_waited_time;//done pending for checking
    static uint64_t total_executed_time;//done
    static uint64_t total_blocked_time;//done
    static uint64_t average_blocked_time;//done
    static uint64_t total_processes_compleated;
    static uint64_t total_processes_created;//done
    static uint64_t total_processes_blocked;//done
    static int lambda;
    static int tick;
    static void reset();
};


#endif // GLOBALVARIABLES_HPP
