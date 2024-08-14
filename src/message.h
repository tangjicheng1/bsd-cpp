#include <cstdint>
#include <cstring>

constexpr size_t MAX_MESSAGE_BODY_LENGTH = 1024;

#pragma pack(push, 1)
struct Message {
    uint16_t length;
    char message_type;
    char body[MAX_MESSAGE_BODY_LENGTH];
};
#pragma pack(pop)
