#include "logger.h"
//  #include "../datasystems.h"
//#include <QFileInfo>

    long logger::cnt = 0;

    std::string logger::CreateLogName(){        

        return Num::fn;       
        //return "log_debug_"+ datetime::utcExample()+ ".txt";
    }

    std::string logger::CreateLogName2(){

        //return Num::fn;
        //return "log_debug_"+ datetime::utcExample()+ ".txt";
        //return "_"+ datetime::utcExample()+ ".txt";
        return "_"+ datetime::utcExample()+ "_";
    }

    std::string logger::time(){

        //return Num::fn;
        //return "log_debug_"+ datetime::utcExample()+ ".txt";
        //return "_"+ datetime::utcExample()+ ".txt";
        return datetime::utcExample();
    }


    void logger::ClearLog(std::string fn){
       fn=CreateLogName();
       std::cout<<"<<<<<<<<<<<Clear log>>>>>>>>>>>"<<"\n";
       std::ofstream out;    
       //out.open(fn,std::ios::app);
       fn=Num::fn;
       out.open(fn);    
       out.close();
    }

    void logger::WriteMsgConsole(std::string _msg){
        std::cout<<(cnt++)<<" | "<<datetime::utcExample()<<"| message: "<<_msg<<"  "<<"\n";
    }

    void logger::WriteMsg(std::string msg, std::string fn){
       //return;
       //fn=CreateLogName();
    std::ofstream out;
    out.open(fn,std::ios::app);
    //out.open(fn);
    if(out.is_open())
    {        
        out<<(cnt++)<<" | "<<datetime::utcExample()<<"| message: "<<msg<<"  "<<"\n";        
    }
    out.close();
    }

    void logger::WriteMsgError(std::string msg, std::string fn){        
        fn=CreateLogName();
    std::ofstream out;    
    out.open(fn,std::ios::app);
    //out.open(fn);
    if(out.is_open())
    {   
        out<<(cnt++)<<" | "<<
        datetime::utcExample()
        <<" | ******* | Error information: "<<msg<<"  "<<"\n";        
    }
    out.close();
    }

    void logger::SaveEndMsgNumber(std::string fn){        
        fn=CreateLogName();
    std::ofstream out;    
    //out.open(fn,std::ios::app);
    out.open(fn);
    if(out.is_open())
    {        
        out<<cnt<<"\n";        
    }
    out.close();
    }

    void logger::ReadEndMsgNumber(std::string fn)
    {        
    
    }
