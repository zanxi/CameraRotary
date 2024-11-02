#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>


#include "datetime.h"
#include "data.h"

class logger
{    
    public:
        
    static long cnt;
    static std::string CreateLogName();
    static std::string CreateLogName2();
    static std::string time();
    static void ClearLog(std::string fn=Num::fn);
    static void WriteMsgConsole(std::string msg);
    static void WriteMsg(std::string msg, std::string fn=Num::fn);
    static void WriteMsgError(std::string msg, std::string fn=Num::fn);
    static void SaveEndMsgNumber(std::string fn=Num::fn_logger_count_number);
    static void ReadEndMsgNumber(std::string fn=Num::fn_logger_count_number);
    static void WriteSqlScript(std::string msg, std::string fn=Num::fn);

};

#endif
