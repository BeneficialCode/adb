#pragma once

#include <string>

#include <openssl/evp.h>

#include "key_type.pb.h"

namespace adb {
    namespace crypto {

        // Class that represents a public/private key pair.
        class Key {
        public:
            explicit Key(bssl::UniquePtr<EVP_PKEY>&& pkey, adb::proto::KeyType type)
                : pkey_(std::move(pkey)), key_type_(type) {}
            Key(Key&&) = default;
            Key& operator=(Key&&) = default;

            EVP_PKEY* GetEvpPkey() const { return pkey_.get(); }
            adb::proto::KeyType GetKeyType() const { return key_type_; }
            static std::string ToPEMString(EVP_PKEY* pkey);

        private:
            bssl::UniquePtr<EVP_PKEY> pkey_;
            adb::proto::KeyType key_type_;
        };  // Key

    }  // namespace crypto
}  // namespace adb