#ifndef TOOLAIO_H
#define TOOLAIO_H

#include<unordered_set>
#include<unordered_map>
#include<algorithm>
#include<iostream>
#include<cstdint>
#include<cstdarg>
#include<cstring>
#include<string>
#include<chrono>
#include<memory>
#include<vector>
#include<cstdio>
#include<ctime>

#include <QObject>

#include <QtNetwork>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include <QByteArray>

#include <QImage>

const char True[]  = "True";
const char False[] = "False";

#define DEBUG_MESSAGE

//Enable Output
const bool G_ENABLE_CON_OUTPUT = false;
const bool G_ENABLE_WIN_OUTPUT = false;

enum class pType
{
    MESSAGE, WARNING, ERROR
};

class Out
{
public:
    Out() = default;
    ~Out() = default;

private:
    static void printTime();

public:
    static void Log(pType Type, const char* Format, ...);
};

#ifdef TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION
std::vector<std::string> allLogs;
std::string thisLog;

char printBuffer[512];

void Out::printTime()
{
    time_t nowTime = time(nullptr);
    tm* ltm = localtime(&nowTime);

    if (G_ENABLE_CON_OUTPUT)
    {
        printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    }
    if (G_ENABLE_WIN_OUTPUT)
    {
        snprintf(printBuffer, sizeof(printBuffer), "[%02d:%02d:%02d]",
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

        thisLog += printBuffer;
    }
}

void Out::Log(pType Type, const char* Format, ...)
{
#ifndef DEBUG_MESSAGE
    return;
#endif

    thisLog.clear();

    switch (Type)
    {
    case pType::MESSAGE:
        if (G_ENABLE_CON_OUTPUT) printf("[MESSAGE]");
        if (G_ENABLE_WIN_OUTPUT) thisLog += "[MESSAGE]";
        break;
    case pType::WARNING:
        if (G_ENABLE_CON_OUTPUT) printf("[WARNING]");
        if (G_ENABLE_WIN_OUTPUT) thisLog += "[WARNING]";
        break;
    case pType::ERROR:
        if (G_ENABLE_CON_OUTPUT) printf("[ ERROR ]");
        if (G_ENABLE_WIN_OUTPUT) thisLog += "[ ERROR ]";
        break;
    }
    if (G_ENABLE_CON_OUTPUT) printf(" ");
    if (G_ENABLE_WIN_OUTPUT) thisLog += " ";

    printTime();

    if (G_ENABLE_CON_OUTPUT) printf(" >>");
    if (G_ENABLE_WIN_OUTPUT) thisLog += " >>";

    va_list args;

    va_start(args, Format);
    if (G_ENABLE_CON_OUTPUT)
    {
        vprintf(Format, args);
    }
    if (G_ENABLE_WIN_OUTPUT)
    {
        vsnprintf(printBuffer, sizeof(printBuffer), Format, args);
        thisLog += printBuffer;
    }
    va_end(args);

    if (G_ENABLE_CON_OUTPUT) puts("");

    if (G_ENABLE_WIN_OUTPUT)
    {
        // Max Size = 32
        if (allLogs.size() >= 32)
        {
            allLogs.erase(allLogs.begin());
        }

        allLogs.push_back(thisLog);
    }
}
#else
// FROM ToolAIO
extern std::vector<std::string> allLogs;
// FROM ToolAIO
#endif// TOOL_ALL_IN_ONE_OUT_IMPLEMENTATION

#endif // TOOLAIO_H
