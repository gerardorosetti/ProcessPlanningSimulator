#include "algorithms.hpp"

Algorithm::Algorithm(std::shared_ptr<QueueInterface> adapter) noexcept
    : process_queue{adapter}
{}

// non-expulsive algorithms
ConcurrentQueue& Algorithm::get_process_queue()
{
    return process_queue;
}

FirstComeFirstServed::FirstComeFirstServed()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void FirstComeFirstServed::process_algorithm(bool *going)
{
    while(*going)
    {
        std::this_thread::sleep_for(std::chrono::seconds{1});
        Process current = process_queue.pop();
        std::cout << current << std::endl;
        // printf("Id: %ld - Status: %d", current.get_id(), current.get_status());
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;
        // printf("Id: %ld - Status: %d", current.get_id(), current.get_status());
        current.update_status(STATUS::COMPLETED);
        std::cout << current << std::endl;
        // printf("Id: %ld - Status: %d", current.get_id(), current.get_status());
    }
}

struct SJF_CMP
{
    bool operator() (const Process& a, const Process& b)
    {
        return a.get_time() > b.get_time();
    }
};

ShortestJobFirst::ShortestJobFirst()
    : Algorithm{std::make_shared<HeapAdapter<SJF_CMP>>()}{}

void ShortestJobFirst::process_algorithm(bool *going)
{
    while(*going)
    {
        Process current = process_queue.pop();
        std::cout << current << std::endl;
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;
        current.update_status(STATUS::COMPLETED);
        std::cout << current << std::endl;
    }
}


RandomSelection::RandomSelection()
    : Algorithm{std::make_shared<VectorAdapter>()}{}

void RandomSelection::process_algorithm(bool *going)
{
    while(*going)
    {
        Process current = process_queue.pop();
        std::cout << current << std::endl;
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        current.update_status(STATUS::COMPLETED);
        std::cout << current << std::endl;
    }
}

struct PS_CMP
{
    bool operator() (const Process& a, const Process& b)
    {
        return a.get_priority() > b.get_priority();
    }
};
PrioritySelectionNonExpulsive::PrioritySelectionNonExpulsive()
    : Algorithm{std::make_shared<HeapAdapter<PS_CMP>>()}{}

void PrioritySelectionNonExpulsive::process_algorithm(bool *going)
{
    while(*going)
    {
        Process current = process_queue.pop();
        current.update_status(STATUS::READY);
        std::cout << current << std::endl;
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(current.get_time()));
        current.update_status(STATUS::COMPLETED);
        std::cout << current << std::endl;
    }
}

// expulsive algorithms

PrioritySelectionExpulsive::PrioritySelectionExpulsive()
    : Algorithm{std::make_shared<HeapAdapter<PS_CMP>>()}{}

void PrioritySelectionExpulsive::process_algorithm(bool *going)
{
    while(*going)
    {
        Process current = process_queue.pop();
        current.update_status(STATUS::READY);
        std::cout << current << std::endl;
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;
        Process next = process_queue.top();

        if(current.get_priority() > next.get_priority())
        {
            current.update_status(STATUS::BLOCKED);
            std::cout << current << std::endl;
            process_queue.push(current);
        } else
        {
            std::this_thread::sleep_for(std::chrono::seconds(current.get_time()));
            current.update_status(STATUS::COMPLETED);
            std::cout << current << std::endl;
        }
    }
}

RoundRobin::RoundRobin()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void RoundRobin::process_algorithm(bool *going)
{
    size_t quantum = 2;
    while(*going)
    {
        Process current = process_queue.pop();
        current.update_status(STATUS::READY);
        std::cout << current << std::endl;
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;
        if(current.get_time() > quantum)
        {
            std::this_thread::sleep_for(std::chrono::seconds(quantum));
            current.update_status(STATUS::BLOCKED);
            std::cout << current << std::endl;
            current.update_time(current.get_time() - quantum);
            process_queue.push(current);
        }else
        {
            std::this_thread::sleep_for(std::chrono::seconds(current.get_time()));
            current.update_status(STATUS::COMPLETED);
            std::cout << current << std::endl;
        }
    }
}

struct SRTF_CMP
{
    bool operator() (const Process& a, const Process& b)
    {
        return a.get_time() > b.get_time();
    }
};

ShortestRemainingTimeFirst::ShortestRemainingTimeFirst()
    : Algorithm{std::make_shared<HeapAdapter<SRTF_CMP>>()}{}

void ShortestRemainingTimeFirst::process_algorithm(bool *going)
{
    size_t quantum = 1;
    bool flag = 0;
    while(*going)
    {
        flag = 0;
        Process current = process_queue.pop();
        current.update_status(STATUS::READY);
        std::cout << current << std::endl;
        current.update_status(STATUS::IN_EXECUTION);
        std::cout << current << std::endl;

        Process next;
        while( current.get_time() > 0 )
        {
            std::this_thread::sleep_for(std::chrono::seconds(quantum));
            current.update_time( current.get_time() - quantum);
            next = process_queue.top();
            if(next.get_time() < current.get_time())
            {
                current.update_status(STATUS::BLOCKED);
                std::cout << current << std::endl;
                process_queue.push(current);
                flag = 1;
                break;
            }
        }
        if (flag) continue;
        std::this_thread::sleep_for(std::chrono::seconds(current.get_time()));
        current.update_status(STATUS::COMPLETED);
        std::cout << current << std::endl;
    }
}
