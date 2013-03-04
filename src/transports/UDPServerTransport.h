#ifndef UDPSERVERTRANSPORT_H_
#define UDPSERVERTRANSPORT_H_

#include "Transport.h"
#include <ev++.h>
#include <set>
#include <utility>
#include <memory>

#include <sys/socket.h>

class UDPServerTransport : public Transport {
public:
	UDPServerTransport(const std::string&, const std::string&);
	virtual ~UDPServerTransport();

	void enable();
	void to_unknown(const std::string&) {};
    bool send(const std::string&, const std::shared_ptr<sockaddr>&, socklen_t);

private:
    void read_cb(ev::io&, int);
    friend ev::io;
	// UDPv6 shares port space with UDPv4
	struct sockaddr* _group[2];
	socklen_t _group_length[2];
	packet_callback _handler;
	int _fd;
    ev::io _read_watcher;
};

#endif // UDPSERVERTRANSPORT_H_
