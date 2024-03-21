#include "algorithms.hpp"

Algorithm::Algorithm(std::shared_ptr<QueueInterface> adapter) noexcept
    : process_queue{adapter}, blocked_queue{std::make_shared<QueueAdapter>()}
{}

// non-expulsive algorithms
ConcurrentQueue& Algorithm::get_process_queue()
{
    return process_queue;
}

ConcurrentQueue& Algorithm::get_blocked_queue()
{
    return blocked_queue;
}

const Process& Algorithm::get_current_process()
{
    return current_process;
}

FirstComeFirstServed::FirstComeFirstServed()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void FirstComeFirstServed::process_algorithm(bool *going)
{
    while(*going)
    {
        //Process current_process = process_queue.pop();
        //current_process_process = current_process;
        current_process = process_queue.pop();
        //std::cout << current_process << std::endl;

        // printf("Id: %ld - Status: %d", current_process.get_id(), current_process.get_status());
        current_process.update_status(STATUS::IN_EXECUTION);
        //std::cout << current_process << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds{current_process.get_time()});
        // printf("Id: %ld - Status: %d", current_process.get_id(), current_process.get_status());
        current_process.update_status(STATUS::COMPLETED);
        //std::cout << current_process << std::endl;
        // printf("Id: %ld - Status: %d", current_process.get_id(), current_process.get_status());
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
        current_process = process_queue.pop();
        //std::cout << current_process << std::endl;
        current_process.update_status(STATUS::IN_EXECUTION);
        std::this_thread::sleep_for(std::chrono::milliseconds{current_process.get_time()});
        //std::cout << current_process << std::endl;
        current_process.update_status(STATUS::COMPLETED);
        //std::cout << current_process << std::endl;
    }
}


RandomSelection::RandomSelection()
    : Algorithm{std::make_shared<VectorAdapter>()}{}

void RandomSelection::process_algorithm(bool *going)
{
    //std::cout << "ENTRE MGGVASEAAAAAAAA" << std::endl;
    while(*going)
    {
        current_process = process_queue.pop();
        std::cout << current_process << std::endl;
        current_process.update_status(STATUS::IN_EXECUTION);
        std::cout << current_process << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(current_process.get_time()));
        current_process.update_status(STATUS::COMPLETED);
        std::cout << current_process << std::endl;
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
        current_process = process_queue.pop();
        //current_process.update_status(STATUS::READY);
        //std::cout << current_process << std::endl;
        current_process.update_status(STATUS::IN_EXECUTION);
        //std::cout << current_process << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(current_process.get_time()));
        current_process.update_status(STATUS::COMPLETED);
        //std::cout << current_process << std::endl;
    }
}

// expulsive algorithms

PrioritySelectionExpulsive::PrioritySelectionExpulsive()
    : Algorithm{std::make_shared<HeapAdapter<PS_CMP>>()}{}

void PrioritySelectionExpulsive::process_algorithm(bool *going)
{
    while(*going)
    {
        current_process = process_queue.pop();
        //current_process.update_status(STATUS::READY);
        //std::cout << current_process << std::endl;
        current_process.update_status(STATUS::IN_EXECUTION);
        //std::cout << current_process << std::endl;
        Process next = process_queue.top();

        if(current_process.get_priority() > next.get_priority())
        {
            current_process.update_status(STATUS::BLOCKED);
            //std::cout << current_process << std::endl;
            //process_queue.push(current_process);
            blocked_queue.push(current_process);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(current_process.get_time()));
            current_process.update_status(STATUS::COMPLETED);
            //std::cout << current_process << std::endl;
        }
    }
}

RoundRobin::RoundRobin()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void RoundRobin::process_algorithm(bool *going)
{
    size_t quantum = 2000;
    while(*going)
    {
        current_process = process_queue.pop();
        //current_process.update_status(STATUS::READY);
        //std::cout << current_process << std::endl;
        current_process.update_status(STATUS::IN_EXECUTION);
        //std::cout << current_process << std::endl;
        if(current_process.get_time() > quantum)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(quantum));
            current_process.update_status(STATUS::BLOCKED);
            //std::cout << current_process << std::endl;
            current_process.update_time(current_process.get_time() - quantum);
            blocked_queue.push(current_process);
        }else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(current_process.get_time()));
            current_process.update_status(STATUS::COMPLETED);
            //std::cout << current_process << std::endl;
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
    size_t quantum = 1000;
    bool flag = 0;
    while(*going)
    {
        flag = 0;
        current_process = process_queue.pop();
        //current_process.update_status(STATUS::READY);
        //std::cout << current_process << std::endl;
        current_process.update_status(STATUS::IN_EXECUTION);
        //std::cout << current_process << std::endl;

        Process next;
        while( current_process.get_time() > 0 )
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(quantum));
            current_process.update_time( current_process.get_time() - quantum);
            next = process_queue.top();
            if(next.get_time() < current_process.get_time())
            {
                current_process.update_status(STATUS::BLOCKED);
                //std::cout << current_process << std::endl;
                blocked_queue.push(current_process);
                flag = 1;
                break;
            }
        }
        if (flag) continue;
        std::this_thread::sleep_for(std::chrono::milliseconds(current_process.get_time()));
        current_process.update_status(STATUS::COMPLETED);
        //std::cout << current_process << std::endl;
    }
}
