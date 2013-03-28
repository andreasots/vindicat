#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <array>
#include <cassert>

template <class T, std::size_t N> class RingBuffer {
 public:
  T pop() {
    assert(!empty());
    T ret = std::move(_array[_start]);
    _start = (_start + 1) % (N+1);
    return ret;
  }

  void push(const T& value) {
    _array[_end] = value;
    if(full())
      _start = (_start + 1) % (N+1);
    _end = (_end + 1) % (N+1);
  }

  void push(T&& value) {
    _array[_end] = std::move(value);
    if(full())
      _start = (_start + 1) % (N+1);
    _end = (_end + 1) % (N+1);
  }

  bool empty() const {
    return _start == _end;
  }

  bool full() const {
    return (_end + 1) % (N+1) == _start;
  }

 private:
  std::array<T, N+1> _array;
  std::size_t _start = 0, _end = 0;
};

#endif  // RINGBUFFER_H_
