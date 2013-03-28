#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "RingBuffer.h"
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool();
  ~ThreadPool();
  void push(std::function<void(void)>&&);

 private:
  void worker();
  RingBuffer<std::function<void(void)>, 127> _buffer;
  std::vector<std::thread> _threads;
  std::mutex _mutex;
  std::condition_variable _buffer_not_empty;
  volatile bool _stopped = false;
};

#endif  // THREADPOOL_H_
