#ifndef TOOLAIO_H
#define TOOLAIO_H

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cstdarg>
#include <cstring>
#include <string>
#include <chrono>
#include <memory>
#include <vector>
#include <cstdio>
#include <ctime>
#include <queue>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QObject>

#include <QtNetwork>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include <QByteArray>
#include <QVector3D>

#include <QMouseEvent>
#include <QKeyEvent>

// Network Message Type
enum class NMType
{
    CheckRequest = 1,
    CheckResponse,
    SceneRequest,
    StartRenderRequest,
    RenderPixelResponse
};

class UniformMessage
{
public:
    int32_t MessageType;
    int32_t MessageLen;
    std::vector<uint8_t> MessageBuffer;
};

inline bool MessageToBytes(UniformMessage& Message, std::vector<uint8_t>& TargetBuffer)
{
    TargetBuffer.resize(Message.MessageBuffer.size() + 8);

    memcpy(TargetBuffer.data() + 0, &Message.MessageType, 4);
    memcpy(TargetBuffer.data() + 4, &Message.MessageLen, 4);
    memcpy(TargetBuffer.data() + 8, Message.MessageBuffer.data(), Message.MessageLen);

    return true;
}

// 
inline bool BytesToMessage(std::vector<uint8_t>& SourceBuffer, UniformMessage& Message)
{
    if(SourceBuffer.size() < 8) return false;

    memcpy(&Message.MessageType, SourceBuffer.data() + 0, 4);
    memcpy(&Message.MessageLen, SourceBuffer.data() + 4, 4);

    if(SourceBuffer.size() >= static_cast<uint64_t>(Message.MessageLen + 8))
    {
        Message.MessageBuffer.resize(Message.MessageLen);
        memcpy(Message.MessageBuffer.data(), SourceBuffer.data() + 8, Message.MessageLen);

        std::vector<uint8_t> LastBytes(SourceBuffer.size() - Message.MessageLen - 8);
        memcpy(LastBytes.data(), SourceBuffer.data() + Message.MessageLen + 8, LastBytes.size());

        SourceBuffer = LastBytes;

        return true;
    }
    else return false;
}

const bool G_ENABLE_OUTPUT = true;

enum class pType
{
    MESSAGE, WARNING, CRASH
};

class Out
{
public:
    Out() = default;
    ~Out() = default;

private:
    static void printTime()
    {
        time_t nowTime = time(nullptr);
        tm* ltm = localtime(&nowTime);

        printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    }

public:
    static void Log(pType Type, const char* Format, ...)
    {
        if (!G_ENABLE_OUTPUT) return;

        switch (Type)
        {
        case pType::MESSAGE:
            printf("[MESSAGE]");
            break;
        case pType::WARNING:
            printf("[WARNING]");
            break;
        case pType::CRASH:
            printf("[ CRASH ]");
            break;
        }
        printf(" "); printTime(); printf(" >>");

        va_list args;

        va_start(args, Format);
        vprintf(Format, args);
        va_end(args);

        puts("");
    }
};

inline bool Inside(int x, int Left, int Right)
{
    return Left <= x && x <= Right;
}

#endif // TOOLAIO_H
