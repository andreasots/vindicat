#include "ThreadPool.h"

#include <iostream>

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
  std::cout << "notifying... " << std::flush;
  _buffer_not_empty.notify_one();
  std::cout << "done" << std::endl;
}

void ThreadPool::worker() {
  std::cout << "Starting thread " << std::this_thread::get_id() << std::endl;
  std::unique_lock<std::mutex> lock(_mutex);
  while(true) {
    while(_buffer.empty() && !_stopped)
      _buffer_not_empty.wait(lock);

    if(_stopped)
      return;
    
    std::cout << "Thread " << std::this_thread::get_id() << " running." << std::endl;
    _buffer.pop()();
  }
}
