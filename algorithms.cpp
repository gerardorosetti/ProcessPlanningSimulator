/*
    2024
    This file contains the definition of the class Algorithm.
*/

#include <algorithms.hpp>

Algorithm::Algorithm(std::shared_ptr<QueueInterface> adapter) noexcept
    : process_queue{adapter}, blocked_queue{std::make_shared<QueueAdapter>()}, completed_process{std::make_shared<QueueAdapter>()}
{}
ConcurrentQueue& Algorithm::get_process_queue()
{
    return process_queue;
}

ConcurrentQueue& Algorithm::get_blocked_queue()
{
    return blocked_queue;
}

ConcurrentQueue& Algorithm::get_completed_process()
{
    return completed_process;
}

const Process& Algorithm::get_current_process()
{
    return current_process;
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

/*
    Non-expulsive Algorithms works the same, depends of the kind of QueueInterface
        1- Pop the process at the top of the queue
        2- Check if the process its mean to be blocked,
            3-if it is we send it to the blocked queue
            3-if it is not we process it normally
        At the same time this is happening we updates some variables of GlobalVariables
*/
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
            int64_t until_blocked = current_process.get_until_blocked_time();
            int64_t time = current_process.get_time();
            if(until_blocked)
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(until_blocked);

                current_process.update_time(time - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;

                current_process.update_status(STATUS::BLOCKED);
                current_process.update_until_blocked_time();

                blocked_queue.push(current_process);            }
            else
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(current_process.get_time());


                GlobalVariables::total_executed_time += current_process.get_time();
                ++GlobalVariables::total_processes_compleated;

                current_process.update_time(0);
                current_process.update_status(STATUS::COMPLETED);
                completed_process.push(current_process);
            }
        }
    }
}

ShortestJobFirst::ShortestJobFirst()
    : Algorithm{std::make_shared<HeapAdapter<TIME_CMP>>()}{}

void ShortestJobFirst::process_algorithm()
{
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());
            int64_t until_blocked = current_process.get_until_blocked_time();
            int64_t time = current_process.get_time();
            if(until_blocked)
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(until_blocked);

                current_process.update_time(time - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;

                current_process.update_status(STATUS::BLOCKED);
                current_process.update_until_blocked_time();

                blocked_queue.push(current_process);
            }
            else
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(current_process.get_time());

                GlobalVariables::total_executed_time += current_process.get_time();
                ++GlobalVariables::total_processes_compleated;

                current_process.update_time(0);
                current_process.update_status(STATUS::COMPLETED);
                completed_process.push(current_process);
            }
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
            int64_t until_blocked = current_process.get_until_blocked_time();
            int64_t time = current_process.get_time();
            if(until_blocked)
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(until_blocked);

                current_process.update_time(time - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;

                current_process.update_status(STATUS::BLOCKED);
                current_process.update_until_blocked_time();

                blocked_queue.push(current_process);
            }
            else
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(current_process.get_time());

                GlobalVariables::total_executed_time += current_process.get_time();
                ++GlobalVariables::total_processes_compleated;

                current_process.update_time(0);
                current_process.update_status(STATUS::COMPLETED);
                completed_process.push(current_process);
            }
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
            int64_t until_blocked = current_process.get_until_blocked_time();
            int64_t time = current_process.get_time();
            if(until_blocked)
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(until_blocked);

                current_process.update_time(time - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;

                current_process.update_status(STATUS::BLOCKED);
                current_process.update_until_blocked_time();

                blocked_queue.push(current_process);
            }
            else
            {
                GlobalVariables::total_waited_time += current_process.get_wait_time();

                current_process.update_status(STATUS::IN_EXECUTION);
                sleep_for(current_process.get_time());

                GlobalVariables::total_executed_time += current_process.get_time();
                ++GlobalVariables::total_processes_compleated;

                current_process.update_time(0);
                current_process.update_status(STATUS::COMPLETED);
                completed_process.push(current_process);
            }
        }
    }
}

// Expulsive Algorithms

/*
    The Round Robin Algorithm works the following way
        A "Quantum" its defined each process can acces the "CPU" the time of the quantum
        if it doesnt end in this time, will be expulsed and send back to the process queue
        if it does will end
    At the same time this is happening we updates some variables of GlobalVariables
*/
RoundRobin::RoundRobin()
    : Algorithm{std::make_shared<QueueAdapter>()}{}

void RoundRobin::process_algorithm()
{
    int64_t quantum = 2000;
    while(GlobalVariables::going)
    {
        if(!process_queue.empty())
        {
            current_process = process_queue.pop();
            current_process.update_wait_time(std::chrono::system_clock::now().time_since_epoch().count());
            int64_t until_blocked = current_process.get_until_blocked_time();

            GlobalVariables::total_waited_time += current_process.get_wait_time();

            current_process.update_status(STATUS::IN_EXECUTION);
            if(until_blocked)
            {
                sleep_for(until_blocked);
                current_process.update_time(current_process.get_time() - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;
                current_process.update_until_blocked_time();
                current_process.update_status(STATUS::BLOCKED);
                blocked_queue.push(current_process);
            }
            else
            {
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
                    ++GlobalVariables::total_processes_compleated;

                    current_process.update_time(0);
                    current_process.update_status(STATUS::COMPLETED);
                    completed_process.push(current_process);
                }
            }
        }
    }
}
/*
    Priority Selection Expulsive and Shortest Remaining Time First works the same their, differences remains in the
    Comparator for arrange their Heap
    This algorithms works
        Pop the process at the top of the queue, after that we constantly check is a new process in the queue
        should be prioritized and expulse the current process, if it is none new process should be prioritized
        will end its procesing eventually
    At the same time this is happening we updates some variables of GlobalVariables
*/

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
            int64_t until_blocked = get_current_process().get_until_blocked_time();

            if(current_process.get_until_blocked_time())
            {
                sleep_for(until_blocked);

                current_process.update_time(current_process.get_time() - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;

                current_process.update_status(STATUS::BLOCKED);
                current_process.update_until_blocked_time();

                blocked_queue.push(current_process);
            }
            else
            {
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

                        GlobalVariables::total_executed_time += current_process.get_time();

                        ++GlobalVariables::total_processes_compleated;

                        current_process.update_time(0);
                        current_process.update_status(STATUS::COMPLETED);
                        completed_process.push(current_process);
                        break;
                    }
                    if (!process_queue.empty())
                    {
                        if(process_queue.top().get_priority() < process_priority)
                        {

                            GlobalVariables::total_executed_time += time_counter;

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
            int64_t until_blocked = current_process.get_until_blocked_time();
            if(until_blocked)
            {
                sleep_for(until_blocked);

                current_process.update_time(current_process.get_time() - until_blocked);
                GlobalVariables::total_executed_time += until_blocked;

                current_process.update_status(STATUS::BLOCKED);
                current_process.update_until_blocked_time();

                blocked_queue.push(current_process);
            }
            else
            {
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
                        ++GlobalVariables::total_processes_compleated;

                        current_process.update_time(0);
                        current_process.update_status(STATUS::COMPLETED);
                        completed_process.push(current_process);

                        break;
                    }
                    if (!process_queue.empty())
                    {
                        if(process_queue.top().get_time() < delta)
                        {
                            GlobalVariables::total_executed_time += time_counter;

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
}
