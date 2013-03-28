#include "ThreadPool.h"

ThreadPool::ThreadPool() {
  for(unsigned i = 0; i < std::thread::hardware_concurrency(); i++)
    _threads.push_back(
        std::thread(std::bind(std::mem_fn(&ThreadPool::worker), this)));
}

ThreadPool::~ThreadPool() {
  _stopped = true;
  _buffer_not_empty.notify_all();
  for(auto& worker : _threads)
    worker.join();
}

void ThreadPool::push(std::function<void(void)>&& task) {
  std::lock_guard<std::mutex> lock(_mutex);
  _buffer.push(task);
  _buffer_not_empty.notify_one();
}

void ThreadPool::worker() {
  std::unique_lock<std::mutex> lock(_mutex);
  while(true) {
    while(_buffer.empty() && !_stopped)
      _buffer_not_empty.wait(lock);

    if(_stopped)
      return;
    
    _buffer.pop()();
  }
}
