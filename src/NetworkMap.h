#ifndef NETWORKMAP_H_
#define NETWORKMAP_H_

#include "Transport.h"
#include "Device.h"
#include "PrefixMap.h"
#include "ReadWriteLock.h"
class Link;

#include <lemon/list_graph.h>

#include <memory>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <set>

typedef std::vector< std::tuple<
      std::shared_ptr<Link>, std::shared_ptr<Device>
    > > Path;

// THREE RESPONSIBILITIES
// 1) Maintain information containers for the $k$ closest devices
// 2) Maintain a many-to-one mapping between device id-s and devices
// 3) Mainain a one-to-one mapping between tsockets and devices behind them

class NetworkMap {
 public:
  NetworkMap(std::shared_ptr<Device>&&);
  NetworkMap(const NetworkMap&) = delete;
  const NetworkMap& operator= (const NetworkMap&) = delete;

  void add(std::shared_ptr<Device>&&);
  bool add(std::shared_ptr<Link>&&);

  std::shared_ptr<Device> device(const std::string&);
  std::shared_ptr<Device> device(const TransportSocket&);
  Device& our_device();
  std::shared_ptr<Link> link_to(const std::string&);
  std::shared_ptr<Link> link_between(const std::string&, const std::string&);
  std::vector< std::shared_ptr<Device> > devices();
  std::vector< std::shared_ptr<Device> > neighbors();
  
  typedef std::tuple< std::shared_ptr<Device>, std::shared_ptr<Link>,
                      std::shared_ptr<Device> > EdgeWithEnds;
  std::vector<EdgeWithEnds> shake();
  // Return the links and the respective devices that were added to the map
  // Remove and discard all that we tried to add but had no room for

  Path path_to(const Device&);
  // Return the "best" path to the specified Device. Empty if none.

 private:
  ReadWriteLock _lock;
  lemon::ListGraph _graph;
  lemon::ListGraph::NodeMap<std::shared_ptr<Device> > _g_device;
  lemon::ListGraph::EdgeMap<std::shared_ptr<Link> > _g_link;
  std::set<lemon::ListGraph::Edge> _new_edges;
  PrefixMap<lemon::ListGraph::Node> _node_by_id;
  std::unordered_map<TransportSocket, lemon::ListGraph::Node> _node_by_socket;

  lemon::ListGraph::Node _our_node;
};


#endif // NETWORKMAP_H_
