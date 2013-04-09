#ifndef READWRITELOCK_H_
#define READWRITELOCK_H_

#include <atomic>
#include <mutex>

class ReadWriteLock {
 public:
  typedef ReadWriteLock Read;
  typedef std::mutex Write;
  ReadWriteLock()
    : _readers(0) {
  }

  Read& read_lock() {
    return *this;
  }

  Write& write_lock() {
    return _write_lock;
  }

  void lock() {
    if (++_readers == 1)
      _write_lock.lock();
  }
  
  void unlock() {
    if (--_readers == 0)
      _write_lock.unlock();
  }

 private:
  std::atomic<unsigned> _readers;
  std::mutex _write_lock;
};

#endif  // READWRITELOCK_H_
