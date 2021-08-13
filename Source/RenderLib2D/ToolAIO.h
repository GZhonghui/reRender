#pragma once

// Uniform
#pragma warning(disable:26451)

// Construction
#pragma warning(disable:26495)

#pragma warning(disable:6386)

#include<algorithm>
#include<cstdint>
#include<cstdarg>
#include<cstdio>
#include<string>
#include<vector>
#include<memory>
#include<cmath>
#include<ctime>

#include<unordered_set>
#include<unordered_map>

#include<STB/stb_image.h>
#include<STB/stb_image_write.h>

inline double Limit(double x, double Left, double Right)
{
    return std::max(std::min(x, Right), Left);
}

const bool G_ENABLE_OUTPUT = true;

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
        case pType::ERROR:
            printf("[ ERROR ]");
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