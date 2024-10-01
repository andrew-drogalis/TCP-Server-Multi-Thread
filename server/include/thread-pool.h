// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_THREAD_POOL_H
#define DRO_THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace dro
{

class ThreadPool
{
  public:
    ThreadPool() = default;

    void Start();

    void QueueJob(std::function<void()> const& job);

    void Stop();

    bool Busy();

  private:
    void ThreadLoop();

    std::vector<std::thread> threads;
    bool should_terminate = false;
    std::mutex queue_mutex;
    std::condition_variable mutex_condition;
    std::queue<std::function<void()>> jobs;
};

}// namespace dro
#endif
