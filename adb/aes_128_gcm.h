#pragma once

#include <stdint.h>

#include <optional>
#include <vector>

#include <openssl/aead.h>

namespace adb {
    namespace pairing {

        class Aes128Gcm {
        public:
            explicit Aes128Gcm(const uint8_t* key_material, size_t key_material_len);

            // Encrypt a block of data in |in| of length |in_len|, this consumes all data
            // in |in| and places the encrypted data in |out| if |out_len| indicates that
            // there is enough space. The data contains information needed for
            // decryption that is specific to this implementation and is therefore only
            // suitable for decryption with this class.
            // The method returns the number of bytes placed in |out| on success and a
            // negative value if an error occurs.
            std::optional<size_t> Encrypt(const uint8_t* in, size_t in_len, uint8_t* out, size_t out_len);
            // Decrypt a block of data in |in| of length |in_len|, this consumes all data
            // in |in_len| bytes of data. The decrypted output is placed in the |out|
            // buffer of length |out_len|. On successful decryption the number of bytes in
            // |out| will be placed in |out_len|.
            // The method returns the number of bytes consumed from the |in| buffer. If
            // there is not enough data available in |in| the method returns zero. If
            // an error occurs the method returns a negative value.
            std::optional<size_t> Decrypt(const uint8_t* in, size_t in_len, uint8_t* out, size_t out_len);

            // Return a safe amount of buffer storage needed to encrypt |size| bytes.
            size_t EncryptedSize(size_t size);
            // Return a safe amount of buffer storage needed to decrypt |size| bytes.
            size_t DecryptedSize(size_t size);

        private:
            bssl::ScopedEVP_AEAD_CTX context_;
            // Sequence numbers to use as nonces in the encryption scheme
            uint64_t dec_sequence_ = 0;
            uint64_t enc_sequence_ = 0;
        };

    }  // namespace pairing
}  // namespace adb