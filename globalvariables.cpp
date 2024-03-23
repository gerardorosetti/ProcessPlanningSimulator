#include "globalvariables.hpp"

bool GlobalVariables::going = true;
uint64_t GlobalVariables::total_time = 0;
uint64_t GlobalVariables::total_waited_time = 0;
uint64_t GlobalVariables::total_executed_time = 0;
uint64_t GlobalVariables::total_blocked_time = 0;
uint64_t GlobalVariables::total_processes_compleated = 0;
uint64_t GlobalVariables::total_processes_created = 0;
uint64_t GlobalVariables::total_processes_blocked = 0;