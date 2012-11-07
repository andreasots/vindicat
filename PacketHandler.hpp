#include "NetworkMap.hpp"
#include "CryptoIdentity.hpp"
#include <stdint.h>


class PacketHandler {
public:
	PacketHandler(NetworkMap&, CryptoIdentity&);
    void operator()(TransportSocket* ts, std::string p);
private:
    NetworkMap& _nm;
    CryptoIdentity& _crypto_identity;
    // LinkProposal _createLinkProposal(DeviceBusinesscard,LinkStatus,LinkTransport);
    // LinkPromise _signLink(LinkProposal);
};
