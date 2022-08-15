#ifndef SRM_IC_2023_MODULES_COMMON_PACKET_H_
#define SRM_IC_2023_MODULES_COMMON_PACKET_H_

#include <ostream>

struct ReceivePacket {
};

struct SendPacket {
};

std::ostream &operator<<(std::ostream &str, const ReceivePacket &packet);
std::ostream &operator<<(std::ostream &str, const SendPacket &packet);

#endif  // SRM_IC_2023_MODULES_COMMON_PACKET_H_
