#ifndef READWRITELOCK_H_
#define READWRITELOCK_H_

#include <atomic>
#include <mutex>

class ReadWriteLock {
 public:
  class Read {
   public:
    Read(ReadWriteLock& lock)
      : _lock(lock) {
    }

    void lock() {
      _lock._no_waiting.lock();
      if(_lock._readers++ == 0)
        _lock._reading.lock();
      _lock._no_waiting.unlock();
    }

    void unlock() {
      if(--_lock._readers == 0)
        _lock._reading.unlock();
    }

   private:
    ReadWriteLock& _lock;
  };

  class Write {
   public:
    Write(ReadWriteLock& lock)
      : _lock(lock) {
    }

    void lock() {
      _lock._no_waiting.lock();
      _lock._reading.lock();
      _lock._no_waiting.unlock();
    }

    void unlock() {
      _lock._reading.unlock();
    }

   private:
    ReadWriteLock& _lock;
  };

  ReadWriteLock()
    : _readers(0), _read(*this), _write(*this) {
  }

  Read& read_lock() {
    return _read;
  }

  Write& write_lock() {
    return _write;
  }

 private:
  std::atomic<unsigned> _readers;
  std::mutex _no_waiting, _reading;
  Read _read;
  Write _write;
};

#endif  // READWRITELOCK_H_
