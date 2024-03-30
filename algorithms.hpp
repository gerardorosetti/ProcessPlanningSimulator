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

enum AlgorithmType
{
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
        -It receives a Queue interface, the type of queue will depend on the kind of algorithm
        -We overwrite the process_algorithm method depending of the algorithm
        -Getters for the atributes
    */
    Algorithm(std::shared_ptr<QueueInterface>) noexcept;
    virtual void process_algorithm() = 0;
    ConcurrentQueue& get_process_queue();
    ConcurrentQueue& get_blocked_queue();
    ConcurrentQueue& get_completed_process();
    const Process& get_current_process();
protected:
    /*
        -Process in the cpu
        -Concurrentqueue of process ready
        -Concurrentqueue of process ready
    */
    Process current_process;
    ConcurrentQueue process_queue;
    ConcurrentQueue blocked_queue;
    ConcurrentQueue completed_process;
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

class RoundRobin: public Algorithm
{
public:
    RoundRobin();
    void process_algorithm() override;
};

class PrioritySelectionExpulsive: public Algorithm
{
public:
    PrioritySelectionExpulsive();
    void process_algorithm() override;
};

class ShortestRemainingTimeFirst: public Algorithm
{
public:
    ShortestRemainingTimeFirst();
    void process_algorithm() override;
};

#endif // ALGORITHMS_HPP
