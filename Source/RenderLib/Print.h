#pragma once

#include<ctime>
#include<cstdio>
#include<cstdarg>

//enum class
enum class PrintType
{
	MESSAGE,
	WARNING,
	ERROR,
	CONFIG,
	COUNT
};

inline void printTime()
{
	time_t now = time(nullptr);
	tm* ltm = localtime(&now);

	printf("[%02d:%02d:%02d]", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

inline void Print(PrintType Type, const char* Format, ...)
{
	//Head
	switch (Type)
	{
	case PrintType::MESSAGE:
		printf("[MESSAGE]");
		break;
	case PrintType::WARNING:
		printf("[WARNING]");
		break;
	case PrintType::ERROR:
		printf("[ ERROR ]");
		break;
	case PrintType::CONFIG:
		printf("[ CNFIG ]");
		break;
	case PrintType::COUNT:
		printf("[ COUNT ]");
		break;
	}
	printf(" ");
	printTime();
	printf(" ");
	printf(">>");

	va_list args;

	va_start(args, Format);

	vprintf(Format, args);

	va_end(args);

	//Tail
	printf("\n");
}