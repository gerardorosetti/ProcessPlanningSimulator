/*
    2024
    This file contains the declaration of the class GlobalVariables.
    This file contains the declaration of the sleep for method
*/
#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <thread>

class GlobalVariables
{
public:
    static bool going;
    static double total_time;
    static uint64_t total_waited_time;
    static uint64_t average_waited_time;
    static uint64_t total_executed_time;
    static uint64_t average_executed_time;
    static uint64_t total_blocked_time;
    static uint64_t average_blocked_time;
    static uint64_t total_processes_compleated;
    static uint64_t total_processes_created;
    static uint64_t total_processes_blocked;
    static double CPU_usage;
    static int lambda;
    static int tick;
    static void reset();
    static void update();
};

//sleep_for based on the tick picked
void sleep_for(int64_t time);

#endif // UTILS_HPP
