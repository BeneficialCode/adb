#pragma once

#include <functional>

#include "config.h"
#include "udp_socket.h"
#include "error.h"
#include "udp_packet.h"

namespace openscreen {
    namespace discovery {

        class MdnsMessage;

        class MdnsReceiver {
        public:
            class ResponseClient {
            public:
                virtual ~ResponseClient();

                virtual void OnMessageReceived(const MdnsMessage& message) = 0;
            };

            // MdnsReceiver does not own |socket| and |delegate|
            // and expects that the lifetime of these objects exceeds the lifetime of
            // MdnsReceiver.
            explicit MdnsReceiver(Config config);
            MdnsReceiver(const MdnsReceiver& other) = delete;
            MdnsReceiver(MdnsReceiver&& other) noexcept = delete;
            MdnsReceiver& operator=(const MdnsReceiver& other) = delete;
            MdnsReceiver& operator=(MdnsReceiver&& other) noexcept = delete;
            ~MdnsReceiver();

            void SetQueryCallback(
                std::function<void(const MdnsMessage&, const IPEndpoint& src)> callback);
            void AddResponseCallback(ResponseClient* callback);
            void RemoveResponseCallback(ResponseClient* callback);

            // The receiver can be started and stopped multiple times.
            // Start and Stop are both synchronous calls. When MdnsReceiver has not yet
            // been started, OnRead callbacks it receives from the task runner will be
            // no-ops.
            void Start();
            void Stop();

            void OnRead(UdpSocket* socket, ErrorOr<UdpPacket> packet);

        private:
            enum class State {
                kStopped,
                kRunning,
            };

            std::function<void(const MdnsMessage&, const IPEndpoint& src)>
                query_callback_;
            State state_ = State::kStopped;

            std::vector<ResponseClient*> response_clients_;

            Config config_;
        };

    }  // namespace discovery
}  // namespace openscreen