#ifndef MESSAGE_H
#define MESSAGE_H

enum MessageType
{
    MESSAGE,
    ERROR,
    WARNING
};

class Message
{
public:
    Message()=default;
    ~Message()=default;
public:
    static void print(MessageType message_type,const char *message,
        bool enable_file=true,bool enable_time=true);
};

#endif