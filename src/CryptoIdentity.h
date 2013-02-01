#ifndef CRYPTOIDENTITY_H_
#define CRYPTOIDENTITY_H_

#include "vindicat.pb.h" // just for algorithm numbers, maybe move them...

#include <ed25519.h>
#include <string>

class CryptoIdentity {
public:
	CryptoIdentity();
	CryptoIdentity(const CryptoIdentity& _) = delete;
	const CryptoIdentity& operator= (const CryptoIdentity& _) = delete;

	bool encrypt(const std::string&, std::string, PkencAlgo, const std::string&, std::string&) const;
	bool open(const std::string&, std::string, const std::string&, PkencAlgo, std::string&) const;
	bool sign(const std::string&, SigAlgo, std::string&) const;

	void our_businesscard(DeviceBusinesscard& card) const;
private:
	ed25519_secret_key _secretkey_edsig;
	ed25519_public_key _verkey_edsig;

	std::string _secretkey_naclbox;
	std::string _enckey_naclbox;
};

#endif // CRYPTOIDENTITY_H_
