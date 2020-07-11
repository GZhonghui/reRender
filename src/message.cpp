#include"message.h"

#include<cstdio>
#include<ctime>

void Message::print(MessageType message_type,const char *message,
    bool enable_file,bool enable_time)
{
    const int file_name_size=32;
    char log_file_path[file_name_size];
    time_t now=time(nullptr);
    tm *ltm=localtime(&now);
    sprintf(log_file_path,"./logs/%04d-%02d-%02d.txt",1900+ltm->tm_year,ltm->tm_mon,ltm->tm_mday);
    log_file_path[file_name_size-1]=0;
    FILE *log_file=nullptr;
    if(enable_file)
    {
        log_file=fopen(log_file_path,"a");
        if(!log_file)
        {
            Message::print(MessageType::ERROR,"Write log file failed.",false);
        }
    }
    switch(message_type)
    {
        case MessageType::MESSAGE:
        printf("\033[32m[MESSAGE from C++]\033[0m");
        if(enable_file&&log_file)
        {
            fprintf(log_file,"[MESSAGE from C++]");
        }
        break;
        case MessageType::ERROR:
        printf("\033[31m[ERROR   from C++]\033[0m");
        if(enable_file&&log_file)
        {
            fprintf(log_file,"[ERROR   from C++]");
        }
        break;
        case MessageType::WARNING:
        printf("\033[33m[WARNING from C++]\033[0m");
        if(enable_file&&log_file)
        {
            fprintf(log_file,"[WARNING from C++]");
        }
        break;
    }
    if(enable_time)
    {
        printf(" \033[34m[%04d-%02d-%02d %02d:%02d:%02d]\033[0m",1900+ltm->tm_year,ltm->tm_mon,
            ltm->tm_mday,ltm->tm_hour,ltm->tm_min,ltm->tm_sec);
        if(enable_file&&log_file)
        {
            fprintf(log_file," [%04d-%02d-%02d %02d:%02d:%02d]",1900+ltm->tm_year,ltm->tm_mon,
                ltm->tm_mday,ltm->tm_hour,ltm->tm_min,ltm->tm_sec);
        }
    }
    printf(" %s\n",message);
    if(enable_file&&log_file)
    {
        fprintf(log_file," %s\n",message);
        fclose(log_file);
    }
}
