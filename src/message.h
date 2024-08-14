#pragma once

#include <cstdint>

const int MAX_BODY_LENGTH = 1024;

#pragma pack(push, 1)
struct Message
{
    uint16_t length;
    char message_type;
    char body[MAX_BODY_LENGTH];
};
#pragma pack(pop)

const char MESSAGE_TYPE_HELLO = 'H';
const char MESSAGE_TYPE_HEARTBEAT = 'B';
