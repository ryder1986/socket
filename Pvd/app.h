#ifndef APP_H
#define APP_H

#include "server.h"
#include "tool.h"

#include "pvd.h"
class App
{
public:
    App();
    void start()
    {
        cout << "app start" << endl;
    }
private:

    void process_data(Session *clt,char *data,int len)
    {
        str_stream.append(string(data,len));
        prt(info,"string is %s",str_stream.data());
#if 1
        string valid_buf;
        valid_buf.clear();

        while(JsonStr::get_valid_buf(str_stream,valid_buf)) {//Get valid json object, TODO:we only check {} matches, we should check json grammar
            prt(info,"process string   %s",valid_buf.data());
            prt(info,"left string   %s",str_stream.data());
            DataPacket data(valid_buf);
            string cmd=data.get_string("cmd");
            if(cmd=="set config"){
                cm.set_config(data.get_string("data"));
                //restart_all();
            }

            if(cmd=="get config"){
               clt->send(cm.get_config().data().data(),cm.get_config().data().length());
            }


            //clt->send("hi,i get json",10);
        }
#endif
    }
    //TODO,we dont need clients message, so result can be send by udp one day.
    void process_data_from_output(Session *clt,char *data,int len)
    {
        //ignore request now, we just send all clts with results
    }
private:
    vector <Session*> *stream_cmd;//clients who connected to our server
    vector <Session*> *stream_output;//client who request our video alg result
    string str_stream;//
    ConfigManager cm;
};

#endif // APP_H
