/*
    2024
    This file contains the definition of the class Process.
*/
#include <process.hpp>

long Process::counter = 1;

Process Process::build_random_process(std::mt19937& gen) noexcept
{
    std::uniform_int_distribution<size_t> time1_dist(1000,2800);
    std::uniform_int_distribution<size_t> time2_dist(100, 800);
    std::uniform_int_distribution<size_t> priority_dist(1, 10);

    size_t random_burst_time = time1_dist(gen);
    size_t random_io_burst_time = time2_dist(gen);

    size_t random_priority = priority_dist(gen);

    Process new_process(random_burst_time, random_io_burst_time, random_priority);
    return new_process;
}


Process::Process(int64_t _burst_time, int64_t _io_burst_time, size_t _priority) noexcept
    : id{counter++}, time{_burst_time + _io_burst_time}, priority{_priority}, status{STATUS::CREATED}, burst_time{_burst_time}, io_burst_time{_io_burst_time},
    creation_time{std::chrono::system_clock::now().time_since_epoch().count()} {}
    
long Process::get_id() const noexcept
{
    return id;
}

int64_t Process::get_time() const noexcept
{
    return time;
}

size_t Process::get_priority() const noexcept
{
    return priority;
}

STATUS Process::get_status() const noexcept
{
    return status;
}

void Process::update_status(STATUS status) noexcept
{
    this->status = status;
}

int64_t Process::get_wait_time() const noexcept
{
    return time;
}

void Process::update_wait_time(int64_t t) noexcept
{
    wait_time = creation_time - t;
}

void Process::update_creation_time(int64_t t) noexcept
{
    creation_time = t;
}

void Process::update_time(int64_t t) noexcept
{
    time = t;
}

std::string enum_to_string(STATUS s)
{
    std::string result = "";
    switch (s)
    {
    case STATUS::CREATED:
        result = "CREATED";
        break;
    case STATUS::READY:
        result = "READY";
        break;
    case STATUS::COMPLETED:
        result = "COMPLETED";
        break;
    case STATUS::IN_EXECUTION:
        result = "IN_EXECUTION";
        break;
    case STATUS::BLOCKED:
        result = "BLOCKED";
        break;
    default:
        result = "FAILED";
        break;
    }
    return result;
}

std::ostream& operator<<(std::ostream& out,const Process& p)
{

    out << "Process ID: "<< ((p.id < 10) ? "0" : "") << p.id << " Current Status: " <<  enum_to_string(p.status);
    return out;
}
