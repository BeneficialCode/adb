#pragma once

#include "udp_socket.h"
#include "error.h"
#include "ip_address.h"

namespace openscreen {
    namespace discovery {

        class MdnsMessage;

        class MdnsSender {
        public:
            // MdnsSender does not own |socket| and expects that its lifetime exceeds the
            // lifetime of MdnsSender.
            explicit MdnsSender(UdpSocket* socket);
            MdnsSender(const MdnsSender& other) = delete;
            MdnsSender(MdnsSender&& other) noexcept = delete;
            virtual ~MdnsSender();

            MdnsSender& operator=(const MdnsSender& other) = delete;
            MdnsSender& operator=(MdnsSender&& other) noexcept = delete;

            virtual Error SendMulticast(const MdnsMessage& message);
            virtual Error SendMessage(const MdnsMessage& message,
                const IPEndpoint& endpoint);

            void OnSendError(UdpSocket* socket, Error error);

        private:
            UdpSocket* const socket_;
        };

    }  // namespace discovery
}  // namespace openscreen