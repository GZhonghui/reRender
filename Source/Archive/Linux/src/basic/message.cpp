#include"message.h"

std::unordered_set<unsigned int> printed;

void Message::print(MessageType message_type,const char *message,
    bool enable_file,bool enable_time)
{
    char log_file_path[message_size];
    time_t now_time=time(nullptr);
    FILE *log_file(nullptr);
    tm *local_time=localtime(&now_time);
    local_time->tm_mon+=1;

    sprintf(log_file_path,"./logs/%04d-%02d-%02d.txt",
        1900+local_time->tm_year,local_time->tm_mon,local_time->tm_mday);

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
        {
            printf("\033[32m[MESSAGE]\033[0m");
            if(enable_file&&log_file)
            {
                fprintf(log_file,"[MESSAGE]");
            }
        }
        break;

        case MessageType::ERROR:
        {
            printf("\033[31m[ ERROR ]\033[0m");
            if(enable_file&&log_file)
            {
                fprintf(log_file,"[ ERROR ]");
            }
        }
        break;

        case MessageType::WARNING:
        {
            printf("\033[33m[WARNING]\033[0m");
            if(enable_file&&log_file)
            {
                fprintf(log_file,"[WARNING]");
            }
        }
        break;
    }
    if(enable_time)
    {
        printf(" \033[34m[%04d-%02d-%02d %02d:%02d:%02d]\033[0m",
            1900+local_time->tm_year,local_time->tm_mon,local_time->tm_mday,
            local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
        if(enable_file&&log_file)
        {
            fprintf(log_file," [%04d-%02d-%02d %02d:%02d:%02d]",
                1900+local_time->tm_year,local_time->tm_mon,local_time->tm_mday,
                local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
        }
    }
    printf(" %s\n",message);
    if(enable_file&&log_file)
    {
        fprintf(log_file," %s\n",message);
        fclose(log_file);
    }
    fflush(stdout);
}

void Message::print_once(unsigned int id,const char *message)
{
    if(!printed.count(id))
    {
        printed.insert(id);

        time_t now_time=time(nullptr);
        tm *local_time=localtime(&now_time);
        local_time->tm_mon+=1;

        printf("\033[33m[ DEBUG ]\033[0m");
        printf(" \033[34m[%04d-%02d-%02d %02d:%02d:%02d]\033[0m",
            1900+local_time->tm_year,local_time->tm_mon,local_time->tm_mday,
            local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
        printf(" %s\n",message);
        fflush(stdout);
    }
}

void Message::print_bar(int now_time,bool last,int width)
{
    now_time=std::min(100,std::max(0,now_time));
    width=std::min(100,std::max(20,width));

    char message[message_size];
    int real_width=width-12;
    int blocks=std::max(1,(int)(now_time*width/100.0));
    for(int i=0;i<=width;i+=1)
    {
        message[i]=(i==width?0:(i==blocks-1?'>':(i>=blocks?' ':'=')));
    }

    printf("\r\033[32m[ #%03d%% ]\033[0m [%s]",now_time,message);
    if(last) puts("");
    fflush(stdout);
}
