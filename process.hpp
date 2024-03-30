/*
    2024
    This file contains the declaration of the class Process.
*/
#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <utils.hpp>

//Posible Status of the process
enum STATUS
{
    CREATED,
    READY,
    IN_EXECUTION,
    BLOCKED,
    COMPLETED
};

class Process
{
public:

    Process() = default;
    long get_id() const noexcept;
    int64_t get_time() const noexcept;
    int64_t get_wait_time() const noexcept;
    int64_t get_until_blocked_time() const noexcept;
    size_t get_priority() const noexcept;
    STATUS get_status() const noexcept;
    void update_status(STATUS _status) noexcept;
    void update_wait_time(int64_t t) noexcept;
    void update_until_blocked_time() noexcept;
    void update_creation_time(int64_t t) noexcept;
    void update_time(int64_t t) noexcept;
    static Process build_random_process(std::mt19937& generator) noexcept;
    static long counter;
    friend std::ostream& operator<<(std::ostream& out, const Process& p);

private:

    Process(int64_t _burst_time, int64_t _io_burst_time, size_t _priority, size_t _until_blocked_time) noexcept;
    long id;
    int64_t time;
    int64_t burst_time;
    int64_t io_burst_time;
    int64_t until_blocked_time;
    int64_t creation_time;
    int64_t end_time;
    int64_t wait_time;
    size_t priority;
    STATUS status;
};

#endif // PROCESS_HPP
