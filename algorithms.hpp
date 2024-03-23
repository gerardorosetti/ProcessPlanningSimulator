#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <QWidget>

#include "process.hpp"
#include "concurrent_queue.hpp"
#include <queue>
#include <thread>
#include <chrono>

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
    // Algorithm() = default;
    Algorithm(std::shared_ptr<QueueInterface>) noexcept;
    virtual void process_algorithm(/*bool *going*/) = 0;
    ConcurrentQueue& get_process_queue();
    ConcurrentQueue& get_blocked_queue();
    const Process& get_current_process();
protected:
    Process current_process;
    ConcurrentQueue process_queue;
    ConcurrentQueue blocked_queue;
};

class FirstComeFirstServed: public Algorithm
{
public:
    FirstComeFirstServed();
    void process_algorithm(/*bool *going*/) override;
};

class ShortestJobFirst: public Algorithm
{
public:
    ShortestJobFirst();
    void process_algorithm(/*bool *going*/) override;
};

class RandomSelection: public Algorithm
{
public:
    RandomSelection();
    void process_algorithm(/*bool *going*/) override;
};

class PrioritySelectionNonExpulsive: public Algorithm
{
public:
    PrioritySelectionNonExpulsive();
    void process_algorithm(/*bool *going*/) override;
};

class PrioritySelectionExpulsive: public Algorithm
{
public:
    PrioritySelectionExpulsive();
    void process_algorithm(/*bool *going*/) override;
};

class RoundRobin: public Algorithm
{
public:
    RoundRobin();
    void process_algorithm(/*bool *going*/) override;
};


class ShortestRemainingTimeFirst: public Algorithm
{
public:
    ShortestRemainingTimeFirst();
    void process_algorithm(/*bool *going*/) override;
};

#endif // ALGORITHMS_HPP
