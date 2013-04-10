#ifndef CONNECTIONPOOL_H_
#define CONNECTIONPOOL_H_

#include "PrefixMap.h"
#include "ReadWriteLock.h"
#include <memory>

class Connection;
/** \brief Maps device identifiers to our connections to them */
class ConnectionPool {
 public:
  std::pair<PrefixMap<std::shared_ptr<Connection>>::iterator, bool>
  insert(const std::pair<const std::string, std::shared_ptr<Connection>>& value) {
    std::lock_guard<ReadWriteLock::Write> lock(_lock.write_lock());
    return _map.insert(value);
  }

  std::shared_ptr<Connection> find(const std::string& prefix) {
    std::lock_guard<ReadWriteLock::Read> lock(_lock.read_lock());
    auto iterator = _map.find(prefix);
    if (iterator == _map.end())
      return nullptr;
    return iterator->second;
  }

  std::size_t erase(const std::string& prefix) {
    std::lock_guard<ReadWriteLock::Write> lock(_lock.write_lock());
    return _map.erase(prefix);
  }

 private:
  PrefixMap<std::shared_ptr<Connection>> _map;
  ReadWriteLock _lock;
};

#endif // CONNECTIONPOOL_H_
