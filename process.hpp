#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>
#include <random>
#include <vector>
#include <chrono>

enum STATUS
{
    CREATED,
    READY,
    IN_EXECUTION,
    BLOCKED,
    COMPLETED,
    FAILED
};

class Process
{
public:

    Process() = default;
    long get_id() const noexcept;
    size_t get_time() const noexcept;
    size_t get_priority() const noexcept;
    STATUS get_status() const noexcept;
    void update_status(STATUS _status) noexcept;
    void update_wait_time(int64_t t) noexcept;
    void update_time(int64_t t) noexcept;
    int64_t get_wait_time() const noexcept;
    static Process build_random_process(std::mt19937& generator) noexcept;
    static long counter;
    friend std::ostream& operator<<(std::ostream& out, const Process& p);

private:

    Process(size_t _time, size_t priority) noexcept;
    long id;
    size_t time;
    int64_t creation_time;
    int64_t end_time; //execution time
    int64_t wait_time;
    int64_t answer_time;
    size_t on_going_time;
    // size_t execution_time;
    // size_t wait_time;
    // size_t answer_time;
    size_t priority;
    STATUS status;
};

#endif // PROCESS_HPP
