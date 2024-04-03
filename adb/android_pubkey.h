#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <openssl/rsa.h>

#ifdef __cplusplus
extern "C" {
#endif

	// Size of an RSA modulus such as an encrypted block or a signature.
#define ANDROID_PUBKEY_MODULUS_SIZE (2048 / 8)

// Size of an encoded RSA key.
#define ANDROID_PUBKEY_ENCODED_SIZE \
  (3 * sizeof(uint32_t) + 2 * ANDROID_PUBKEY_MODULUS_SIZE)

/* Allocates a new RSA |key| object, decodes a public RSA key stored in
 * Android's custom binary format from |key_buffer| and sets the key parameters
 * in |key|. |size| specifies the size of the key buffer and must be at least
 * |ANDROID_PUBKEY_ENCODED_SIZE|. The resulting |*key| can be used with the
 * standard BoringSSL API to perform public operations.
 *
 * Returns true if successful, in which case the caller receives ownership of
 * the |*key| object, i.e. needs to call RSA_free() when done with it. If there
 * is an error, |key| is left untouched and the return value will be false.
 */
	bool android_pubkey_decode(const uint8_t* key_buffer, size_t size, RSA** key);

	/* Encodes |key| in the Android RSA public key binary format and stores the
	 * bytes in |key_buffer|. |key_buffer| should be of size at least
	 * |ANDROID_PUBKEY_ENCODED_SIZE|.
	 *
	 * Returns true if successful, false on error.
	 */
	bool android_pubkey_encode(const RSA* key, uint8_t* key_buffer, size_t size);

#ifdef __cplusplus
} // extern "C"
#endif