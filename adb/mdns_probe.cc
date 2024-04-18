#include "mdns_probe.h"

#include <utility>

#include "mdns_random.h"
#include "mdns_sender.h"
#include "mdns_constants.h"
#include "task_runner.h"
#include "time.h"

namespace openscreen {
    namespace discovery {

        MdnsProbe::MdnsProbe(DomainName target_name, IPAddress address)
            : target_name_(std::move(target_name)),
            address_(std::move(address)),
            address_record_(CreateAddressRecord(target_name_, address_)) {}

        MdnsProbe::~MdnsProbe() = default;

        MdnsProbeImpl::Observer::~Observer() = default;

        MdnsProbeImpl::MdnsProbeImpl(MdnsSender* sender,
            MdnsReceiver* receiver,
            MdnsRandom* random_delay,
            TaskRunner* task_runner,
            ClockNowFunctionPtr now_function,
            Observer* observer,
            DomainName target_name,
            IPAddress address)
            : MdnsProbe(std::move(target_name), std::move(address)),
            random_delay_(random_delay),
            task_runner_(task_runner),
            now_function_(now_function),
            alarm_(now_function_, task_runner_),
            sender_(sender),
            receiver_(receiver),
            observer_(observer) {


            receiver_->AddResponseCallback(this);
            alarm_.ScheduleFromNow([this]() { ProbeOnce(); },
                random_delay_->GetInitialProbeDelay());
        }

        MdnsProbeImpl::~MdnsProbeImpl() {

            Stop();
        }

        void MdnsProbeImpl::ProbeOnce() {

            if (successful_probe_queries_++ < kProbeIterationCountBeforeSuccess) {
                // MdnsQuerier cannot be used, because probe queries cannot use the cache,
                // so instead directly send the query through the MdnsSender.
                MdnsMessage probe_query(CreateMessageId(), MessageType::Query);
                MdnsQuestion probe_question(target_name(), DnsType::kANY, DnsClass::kIN,
                    ResponseType::kUnicast);
                probe_query.AddQuestion(probe_question);
                probe_query.AddAuthorityRecord(address_record());
                sender_->SendMulticast(probe_query);

                alarm_.ScheduleFromNow([this]() { ProbeOnce(); },
                    kDelayBetweenProbeQueries);
            }
            else {
                Stop();
                observer_->OnProbeSuccess(this);
            }
        }

        void MdnsProbeImpl::Stop() {

            if (is_running_) {
                alarm_.Cancel();
                receiver_->RemoveResponseCallback(this);
                is_running_ = false;
            }
        }

        void MdnsProbeImpl::Postpone(std::chrono::seconds delay) {

            successful_probe_queries_ = 0;
            alarm_.Cancel();
            alarm_.ScheduleFromNow([this]() { ProbeOnce(); }, Clock::to_duration(delay));
        }

        void MdnsProbeImpl::OnMessageReceived(const MdnsMessage& message) {
            

            for (const auto& record : message.answers()) {
                if (record.name() == target_name()) {
                    Stop();
                    observer_->OnProbeFailure(this);
                }
            }
        }

    }  // namespace discovery
}  // namespace openscreen