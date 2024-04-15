#pragma once

#include <openssl/base.h>
#include <optional>
#include <string>

// These APIs is used to embed adbd's known public keys into client-allowed CA
// issuer list that can indicate to the client which key to use.
namespace adb {
	namespace tls {

		// Takes an encoded public key and generates a X509_NAME that can be used in
		// TlsConnection::SetClientCAList(), to allow the client to figure out which of
		// its keys it should try to use in the TLS handshake. This is guaranteed to
		// return a valid X509_NAME, given a non-empty key.
		bssl::UniquePtr<X509_NAME> CreateCAIssuerFromEncodedKey(std::string_view key);

		// Parses a CA issuer and returns the encoded key, if any. On failure, returns
		// nullopt.
		std::optional<std::string> ParseEncodedKeyFromCAIssuer(X509_NAME* issuer);

		// Converts SHA256 bits to a hex string representation. |sha256| must be exactly
		// |SHA256_DIGEST_LENGTH| in size.
		std::string SHA256BitsToHexString(std::string_view sha256);

		// Converts a valid SHA256 hex string to the actual bits. Returns nullopt on
		// failure.
		std::optional<std::string> SHA256HexStringToBits(std::string_view sha256_str);

	}  // namespace tls
}  // namespace adb