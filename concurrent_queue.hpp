/*
    2024
    This file contains the declaration and definition of the class ConcurrentQueue.
    whit a QueueInterface for a VectorAdapter, QueueAdapter, HeapAdapter
*/
#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include <process.hpp>

class QueueInterface
{
public:
    virtual ~QueueInterface() {}

    virtual void push(const Process& item) noexcept = 0;

    virtual const Process& top() const noexcept = 0;

    virtual Process pop() noexcept = 0;

    virtual bool empty() const noexcept = 0;

    virtual size_t size() const noexcept = 0;

    virtual std::list<Process> toList() const = 0;
};

/*
    The QueueAdapter will be use in the algorithms
        First Come First Served
        Round Robin
*/
class QueueAdapter : public std::queue<Process>,
                     public QueueInterface
{
    using Base = std::queue<Process>;
    using Base::Base;

public:
    void push(const Process& item) noexcept override
    {
        Base::push(item);
    }

    const Process& top() const noexcept override
    {
        return Base::front();
    }

    Process pop() noexcept override
    {
        Process item = this->top();
        Base::pop();
        return item;
    }
    bool empty() const noexcept override
    {
        return Base::empty();
    }
    size_t size() const noexcept override
    {
        return Base::size();
    }
    std::list<Process> toList() const override
    {
        std::list<Process> listForm;
        auto it = Base::c.begin();
        while (it != Base::c.end())
        {
            listForm.push_back(*it);
            ++it;
        }
        return listForm;
    }
};

/*
    The HeapAdapter will be use for the algorithms
        Shortest Job First
        Priority Non-Expulsive
        Priority Expulsive
        Shortest Remaining Time First
*/
template <typename Cmp>
class HeapAdapter : public std::priority_queue<Process, std::vector<Process>, Cmp>,
                    public QueueInterface
{
    using Base = std::priority_queue<Process, std::vector<Process>, Cmp>;
    using Base::Base;

public:
    void push(const Process& item) noexcept override
    {
        Base::push(item);
    }

    const Process& top() const noexcept override
    {
        return Base::top();
    }

    Process pop() noexcept override
    {
        Process item = this->top();
        Base::pop();
        return item;
    }
    bool empty() const noexcept override
    {
        return Base::empty();
    }
    size_t size() const noexcept override
    {
        return Base::size();
    }
    std::list<Process> toList() const override
    {
        std::list<Process> listForm;
        auto it = Base::c.begin();
        while (it != Base::c.end())
        {
            listForm.push_back(*it);
            ++it;
        }
        return listForm;
    }
};

/*
    The Vector Adapter will be use for the Algorithm
        Random Selection
    In here the method pop will pop a random element in the vector
*/
class VectorAdapter : public std::vector<Process>,
                      public QueueInterface
{
    using Base = std::vector<Process>;
    using Base::Base;
public:
    void push(const Process& item) noexcept override
    {
        Base::push_back(item);
    }
    const Process& top() const noexcept override
    {
        return Base::front();
    }

    Process pop() noexcept override
    {
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> id(0, Base::size() - 1);
        size_t i = id(gen);
        Base::iterator it = Base::begin() + i;
        Process item = Base::at(i);
        Base::erase(it);
        return item;
    }

    bool empty() const noexcept override
    {
        return Base::empty();
    }

    size_t size() const noexcept override
    {
        return Base::size();
    }

    std::list<Process> toList() const override
    {
        std::list<Process> listForm;
        for (const auto& elem : *this)
        {
            listForm.push_back(elem);
        }

        return listForm;
    }
private:
    std::random_device rd;
};

/*

    The ConcurrentQueue class is the main class that uses a queue adapter (QueueInterface)
    to provide push, pop, get first element, get size, and check for empty operations concurrently and safely.
*/
class ConcurrentQueue
{
public:
    ConcurrentQueue(std::shared_ptr<QueueInterface> adapter) noexcept
        : queue{adapter} {}

    const Process& push(const Process& item)
    {
      std::lock_guard<std::mutex> lck{mtx};
      queue->push(item);
      cond_var.notify_one();
      return queue->top();
    }

    const Process& push(Process&& item)
    {
      std::lock_guard<std::mutex> lck{mtx};
      queue->push(std::move(item));
      cond_var.notify_one();
      return queue->top();
    }

    Process top()
    {
      std::unique_lock<std::mutex> lck{mtx};
      cond_var.wait(lck, [this]
                    { return !queue->empty(); });
      return queue->top();
    }

    Process pop()
    {
      std::unique_lock<std::mutex> lck{mtx};
      cond_var.wait(lck, [this]
                    { return !queue->empty(); });
      return queue->pop();
    }

    size_t size() const
    {
      std::lock_guard<std::mutex> lck(const_cast<std::mutex &>(mtx));
      return queue->size();
    }

    bool empty() const
    {
      std::lock_guard<std::mutex> lck(const_cast<std::mutex &>(mtx));
      return queue->empty();
    }
    std::list<Process> toList() const
    {
        std::lock_guard<std::mutex> lck(const_cast<std::mutex &>(mtx));
        return queue->toList();
    }

private:
    std::shared_ptr<QueueInterface> queue;
    std::mutex mtx;
    std::condition_variable cond_var;
};

#endif // CONCURRENT_QUEUE_HPP
