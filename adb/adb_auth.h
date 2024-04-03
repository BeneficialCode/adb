#pragma once

#include "adb.h"

#include <deque>
#include <memory>

#include <openssl/rsa.h>

/* AUTH packets first argument */
/* Request */
#define ADB_AUTH_TOKEN         1
/* Response */
#define ADB_AUTH_SIGNATURE     2
#define ADB_AUTH_RSAPUBLICKEY  3

void adb_auth_init();

int adb_auth_keygen(const char* filename);
int adb_auth_pubkey(const char* filename);
std::string adb_auth_get_userkey();
bssl::UniquePtr<EVP_PKEY> adb_auth_get_user_privkey();
std::deque<std::shared_ptr<RSA>> adb_auth_get_private_keys();

void send_auth_response(const char* token, size_t token_size, atransport* t);

int adb_tls_set_certificate(SSL* ssl);
void adb_auth_tls_handshake(atransport* t);