/*
    2024
    This file contains the definition of the class Algorithm.
*/

#include <algorithms.hpp>

Algorithm::Algorithm(std::shared_ptr<QueueInterface> adapter) noexcept
    : process_queue{adapter}, blocked_queue{std::make_shared<QueueAdapter>()}
{}
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


//sleep_for based on the tick picked
void sleep_for(ulong time)
{
    auto tp1 = std::chrono::high_resolution_clock::now();
    uint64_t time_counter = 0;
    while(GlobalVariables::going)
    {
        auto tp2 = std::chrono::high_resolution_clock::now();
        time_counter += std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
        if(time_counter >= time)
        {
            break;
        }
        tp1 = tp2;
        std::this_thread::sleep_for(std::chrono::milliseconds(GlobalVariables::tick));
    }
}

//Comparator for the heap adapter, based on the time that the process have
struct TIME_CMP
{
    bool operator() (const Process& a, const Process& b)
    {
        return a.get_time() > b.get_time();
    }
};

//Comparator for the heap adapter, based on the priority that the process have
struct PRIORITY_CMP
{
    bool operator() (const Process& a, const Process& b)
    {
        return a.get_priority() > b.get_priority();
    }
};


//Non-expulsive Algorithms

FirstComeFirstServed::FirstComeFirstServed()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void FirstComeFirstServed::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            sleep_for(current_process.get_time());

            GlobalVariables::total_executed_time += current_process.get_time();

            current_process.update_status(STATUS::COMPLETED);}
    }
}

ShortestJobFirst::ShortestJobFirst()
    : Algorithm{std::make_shared<HeapAdapter<TIME_CMP>>()}{}

void ShortestJobFirst::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if (!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            sleep_for(current_process.get_time());

            GlobalVariables::total_executed_time += current_process.get_time();

            current_process.update_status(STATUS::COMPLETED);
        }
    }
}


RandomSelection::RandomSelection()
    : Algorithm{std::make_shared<VectorAdapter>()}{}

void RandomSelection::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            sleep_for(current_process.get_time());


            GlobalVariables::total_executed_time += current_process.get_time();

            current_process.update_status(STATUS::COMPLETED);
        }
    }
}

PrioritySelectionNonExpulsive::PrioritySelectionNonExpulsive()
    : Algorithm{std::make_shared<HeapAdapter<PRIORITY_CMP>>()}{}

void PrioritySelectionNonExpulsive::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            sleep_for(current_process.get_time());

            GlobalVariables::total_executed_time += current_process.get_time();

            current_process.update_status(STATUS::COMPLETED);
        }
    }
}

// Expulsive Algorithms

PrioritySelectionExpulsive::PrioritySelectionExpulsive()
    : Algorithm{std::make_shared<HeapAdapter<PRIORITY_CMP>>()}{}

void PrioritySelectionExpulsive::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            auto tp1 = std::chrono::high_resolution_clock::now();
            uint64_t time_counter = 0, delta = 0;
            auto process_time_duration = current_process.get_time();
            auto process_priority = current_process.get_priority();

            while(GlobalVariables::going)
            {
                delta = process_time_duration - time_counter;
                auto tp2 = std::chrono::high_resolution_clock::now();
                time_counter += std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
                if(time_counter >= process_time_duration)
                {
                    current_process.update_status(STATUS::COMPLETED);

                    GlobalVariables::total_executed_time += current_process.get_time();

                    break;
                }
                if (!process_queue.empty())
                {
                    if(process_queue.top().get_priority() < process_priority)
                    {

                        GlobalVariables::total_executed_time += delta;

                        current_process.update_status(STATUS::READY);
                        current_process.update_time(delta);
                        process_queue.push(current_process);
                        break;
                    }
                }
                tp1 = tp2;
                std::this_thread::sleep_for(std::chrono::milliseconds(GlobalVariables::tick));
            }
        }
    }
}

RoundRobin::RoundRobin()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void RoundRobin::process_algorithm()
{
    size_t quantum = 2000;
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            if(current_process.get_time() > quantum)
            {
                sleep_for(quantum);

                GlobalVariables::total_executed_time += quantum;

                current_process.update_status(STATUS::READY);
                current_process.update_time(current_process.get_time() - quantum);
                process_queue.push(current_process);
            }else
            {
                sleep_for(current_process.get_time());

                GlobalVariables::total_executed_time += current_process.get_time();

                current_process.update_status(STATUS::COMPLETED);
            }
        }
    }
}

ShortestRemainingTimeFirst::ShortestRemainingTimeFirst()
    : Algorithm{std::make_shared<HeapAdapter<TIME_CMP>>()}{}

void ShortestRemainingTimeFirst::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            auto tp1 = std::chrono::high_resolution_clock::now();
            uint64_t time_counter = 0, delta = 0;
            auto process_time_duration = current_process.get_time();
            while(GlobalVariables::going)
            {
                delta = process_time_duration - time_counter;
                auto tp2 = std::chrono::high_resolution_clock::now();
                time_counter += std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
                if(time_counter >= process_time_duration)
                {

                    GlobalVariables::total_executed_time += current_process.get_time();

                    current_process.update_status(STATUS::COMPLETED);
                    break;
                }
                if (!process_queue.empty())
                {
                    if(process_queue.top().get_time() < delta)
                    {


                        GlobalVariables::total_executed_time += delta;

                        current_process.update_status(STATUS::READY);
                        current_process.update_time(delta);
                        process_queue.push(current_process);
                        break;
                    }
                }
                tp1 = tp2;
                std::this_thread::sleep_for(std::chrono::milliseconds(GlobalVariables::tick));
            }
        }
    }
}
