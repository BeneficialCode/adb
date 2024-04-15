#pragma once

#include <memory>
#include <optional>

#include "key.h"

namespace adb {
	namespace crypto {

		// Create a new RSA2048 key pair.
		std::optional<Key> CreateRSA2048Key();

		// Generates the public key from the RSA private key.
		bool CalculatePublicKey(std::string* out, RSA* private_key);

	}  // namespace crypto
}  // namespace adb