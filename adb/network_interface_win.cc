#include "network_interface.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

namespace openscreen {

    std::vector<InterfaceInfo> GetAllInterfaces() {
        constexpr size_t INITIAL_BUFFER_SIZE = 15000;
        ULONG outbuflen = INITIAL_BUFFER_SIZE;
        std::vector<unsigned char> charbuf(INITIAL_BUFFER_SIZE);
        DWORD ret = NO_ERROR;
        constexpr int MAX_RETRIES = 5;

        for (int i = 0; i < MAX_RETRIES; ++i) {
            // TODO: This does not include the loopback interface. Decide if we need it.
            ret = GetAdaptersAddresses(AF_UNSPEC /* get both v4/v6 addrs */,
                GAA_FLAG_INCLUDE_PREFIX,
                NULL,
                reinterpret_cast<IP_ADAPTER_ADDRESSES*>(charbuf.data()),
                &outbuflen);
            if (ret == ERROR_BUFFER_OVERFLOW) {
                charbuf.resize(outbuflen);
                continue;
            }
            break;
        }

        if (ret != NO_ERROR) {
            return std::vector<InterfaceInfo>();
        }

        std::vector<InterfaceInfo> infos;
        auto pcurraddrs = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(charbuf.data());
        while (pcurraddrs != nullptr) {

            // Ignore interfaces that are down
            if (pcurraddrs->OperStatus == IfOperStatusDown) {
                pcurraddrs = pcurraddrs->Next;
                continue;
            }

            infos.emplace_back();
            InterfaceInfo& info = infos.back();

            info.index = pcurraddrs->IfIndex;
            std::memcpy(info.hardware_address.data(), pcurraddrs->PhysicalAddress,
                std::min((unsigned long)sizeof(info.hardware_address),
                    pcurraddrs->PhysicalAddressLength));
            info.name = pcurraddrs->AdapterName;

            // Determine the interface type
            switch (pcurraddrs->IfType) {
            case IF_TYPE_ETHERNET_CSMACD:
                info.type = InterfaceInfo::Type::kEthernet;
                break;
            case IF_TYPE_IEEE80211:
                info.type = InterfaceInfo::Type::kWifi;
                break;
            case IF_TYPE_SOFTWARE_LOOPBACK:
                info.type = InterfaceInfo::Type::kLoopback;
                break;
            default:
                info.type = InterfaceInfo::Type::kOther;
                break;
            }

            auto punicast = pcurraddrs->FirstUnicastAddress;
            if (punicast != nullptr) {
                for (int i = 0; punicast != nullptr; ++i) {
                    if (punicast->Address.lpSockaddr->sa_family == AF_INET) {
                        sockaddr_in* sa_in = (sockaddr_in*)punicast->Address.lpSockaddr;
                        char buff[100];
                        DWORD bufflen = 100;

                        IPAddress ip(IPAddress::Version::kV4,
                            reinterpret_cast<uint8_t*>(&(sa_in->sin_addr.s_addr)));
                        info.addresses.emplace_back(ip, punicast->OnLinkPrefixLength);
                    }
                    else if (punicast->Address.lpSockaddr->sa_family == AF_INET6) {
                        sockaddr_in6* sa_in6 = (sockaddr_in6*)punicast->Address.lpSockaddr;
                        char buff[100];
                        DWORD bufflen = 100;

                        IPAddress ip(IPAddress::Version::kV6,
                            reinterpret_cast<uint8_t*>(&(sa_in6->sin6_addr.s6_addr)));
                        info.addresses.emplace_back(ip, punicast->OnLinkPrefixLength);
                    }
                    else {
                        
                    }
                    punicast = punicast->Next;
                }
            }
            pcurraddrs = pcurraddrs->Next;
        }
        return infos;
    }

}  // namespace openscreen