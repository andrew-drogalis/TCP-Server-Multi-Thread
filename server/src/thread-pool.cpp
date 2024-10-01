// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "thread-pool.h"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>// for thread
#include <vector>

namespace dro
{

// Future Optimization
void ThreadPool::Start()
{
    int const maxThreads = std::thread::hardware_concurrency();
    for (int i {}; i < maxThreads; ++i) { threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this)) }
}

void ThreadPool::ThreadLoop()
{
    while (true)
    {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] { return ! jobs.empty() || should_terminate; });
            if (should_terminate)
            {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void ThreadPool::QueueJob(std::function<void()> const& job)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::Busy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = ! jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::Stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) { active_thread.join(); }
    threads.clear();
}

}// namespace dro
