#ifndef APP_H
#define APP_H

#include "server.h"
#include "tool.h"

class JsonStr{
public:

    static  inline bool get_valid_buf(string &src,string &dst)
    {
        if(try_get_obj_buf(src,dst)){
            //src.remove(0,dst.size());
            src=src.substr(dst.size(),src.size());
            return true;
        }
        return false;
    }
private:
    static  inline bool try_get_obj_buf(string src,string &dst)
    {
        int ret=false;
        int stack=0;
        const char *p_tmp=src.data();
        bool flg=false;
        dst.clear();
        dst.append(src);
        int i;
        if(count_begin_symbol(src)>0){
            for(i=0;i<src.size();i++){
                if(p_tmp[i]=='{')
                {
                    stack++;
                    flg=true;
                }
                if(p_tmp[i]=='}')
                    stack--;
                if(stack==0&&flg)
                {
                    break;
                }

            }
            if(i<src.size()){
                ret=true;
                if(src[i+1]=='\n')
                    //   dst.truncate(i+2);
                    dst.substr(0,i+2);
                else
                    // dst.truncate(i+i);
                    dst.substr(0,i+1);
            }
        }
        return ret;
    }
    static inline int count_begin_symbol(string ba)
    {
        const char *tmp=ba.data();
        int sz=ba.size();
        int ret=0;
        int i;
        for( i=0;i<sz;i++){
            if(tmp[i]=='{'){
                ret++;
            }
        }
        return ret;
    }
};

class App
{
public:
    App();
    void start()
    {
        prt(info,"App start");
    }
private:

    void process_data(Session *clt,char *data,int len)
    {
        json_str.append(string(data,len));
        prt(info,"string is %s",json_str.data());
#if 1
        string valid_buf;
        valid_buf.clear();

        while(JsonStr::get_valid_buf(json_str,valid_buf)) {//Get valid json object, TODO:we only check {} matches, we should check json grammar
            prt(info,"process string   %s",valid_buf.data());
            prt(info,"left string   %s",json_str.data());

            //clt->send("hi,i get json",10);
        }
#endif
    }
private:
    Tcpserver server;
    string json_str;
};

#endif // APP_H
