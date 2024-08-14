// message.h
#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>

const int MAX_BODY_LENGTH = 1024; // 定义消息体的最大长度

#pragma pack(push, 1)
struct Message {
    uint16_t length;
    char message_type;
    char body[MAX_BODY_LENGTH]; // 使用常量定义消息体长度
};
#pragma pack(pop)

// 定义一些消息类型常量
const char MESSAGE_TYPE_HELLO = 'H';
const char MESSAGE_TYPE_HEARTBEAT = 'B';

#endif // MESSAGE_H
