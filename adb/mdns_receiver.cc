#include "mdns_receiver.h"

#include <utility>

#include "mdns_reader.h"

namespace openscreen {
    namespace discovery {

        MdnsReceiver::ResponseClient::~ResponseClient() = default;

        MdnsReceiver::MdnsReceiver(Config config) : config_(std::move(config)) {}

        MdnsReceiver::~MdnsReceiver() {
            if (state_ == State::kRunning) {
                Stop();
            }

        }

        void MdnsReceiver::SetQueryCallback(
            std::function<void(const MdnsMessage&, const IPEndpoint&)> callback) {
            // This check verifies that either new or stored callback has a target. It
            // will fail in case multiple objects try to set or clear the callback.
            query_callback_ = callback;
        }

        void MdnsReceiver::AddResponseCallback(ResponseClient* callback) {
            auto it =
                std::find(response_clients_.begin(), response_clients_.end(), callback);

            response_clients_.push_back(callback);
        }

        void MdnsReceiver::RemoveResponseCallback(ResponseClient* callback) {
            auto it =
                std::find(response_clients_.begin(), response_clients_.end(), callback);

            response_clients_.erase(it);
        }

        void MdnsReceiver::Start() {
            state_ = State::kRunning;
        }

        void MdnsReceiver::Stop() {
            state_ = State::kStopped;
        }

        void MdnsReceiver::OnRead(UdpSocket* socket,
            ErrorOr<UdpPacket> packet_or_error) {
            if (state_ != State::kRunning || packet_or_error.is_error()) {
                return;
            }

            UdpPacket packet = std::move(packet_or_error.value());


            MdnsReader reader(config_, packet.data(), packet.size());
            const ErrorOr<MdnsMessage> message = reader.Read();
            if (message.is_error()) {
                if (message.error().code() == Error::Code::kMdnsNonConformingFailure) {
                }
                else {
                }
                return;
            }

            if (message.value().type() == MessageType::Response) {
                for (ResponseClient* client : response_clients_) {
                    client->OnMessageReceived(message.value());
                }
                if (response_clients_.empty()) {
                   
                }
            }
            else {
                if (query_callback_) {
                    query_callback_(message.value(), packet.source());
                }
                else {
                    
                }
            }
        }

    }  // namespace discovery
}  // namespace openscreen