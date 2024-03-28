/*
    2024
    This file contains the declaration of the class Algorithm.
*/
#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <QWidget>

#include <process.hpp>
#include <concurrent_queue.hpp>

#include <queue>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

enum AlgorithmType{
    FCFS,
    RS,
    SJF,
    PNE,
    RR,
    SRTF,
    PE
};
class Algorithm
{
public:
    /*
        It receives a Queue interface, the type of queue will depend on the kind of algorithm
        we overwrite the process_algorithm method depending of the algorithm
        getters for the atributes
    */
    Algorithm(std::shared_ptr<QueueInterface>) noexcept;
    virtual void process_algorithm() = 0;
    ConcurrentQueue& get_process_queue();
    ConcurrentQueue& get_blocked_queue();
    const Process& get_current_process();
protected:
    //Process in the cpu
    //List of process ready
    //List of process ready
    Process current_process;
    ConcurrentQueue process_queue;
    ConcurrentQueue blocked_queue;
};

//Non Expulsive Algorithms

class FirstComeFirstServed: public Algorithm
{
public:
    FirstComeFirstServed();
    void process_algorithm() override;
};

class ShortestJobFirst: public Algorithm
{
public:
    ShortestJobFirst();
    void process_algorithm() override;
};

class RandomSelection: public Algorithm
{
public:
    RandomSelection();
    void process_algorithm() override;
};

class PrioritySelectionNonExpulsive: public Algorithm
{
public:
    PrioritySelectionNonExpulsive();
    void process_algorithm() override;
};

//Expulsive Algorithms

class PrioritySelectionExpulsive: public Algorithm
{
public:
    PrioritySelectionExpulsive();
    void process_algorithm() override;
};

class RoundRobin: public Algorithm
{
public:
    RoundRobin();
    void process_algorithm() override;
};

class ShortestRemainingTimeFirst: public Algorithm
{
public:
    ShortestRemainingTimeFirst();
    void process_algorithm() override;
};

#endif // ALGORITHMS_HPP
