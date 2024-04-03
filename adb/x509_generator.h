#pragma once

#include <openssl/x509v3.h>
#include <string>

namespace adb {
	namespace crypto {

		// Generate a X.509 certificate based on the key |pkey|.
		bssl::UniquePtr<X509> GenerateX509Certificate(EVP_PKEY* pkey);

		// Convert X509* to PEM string format
		std::string X509ToPEMString(X509* x509);

	}  // namespace crypto
}  // namespace adb