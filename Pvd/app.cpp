#include "app.h"
#include "configmanager.h"
App::App():str_stream("")
{
    string dev_name= cm.get_config().get_string("device_name");;
    cout<< "device name is : "<<dev_name <<endl;

    static Tcpserver server(stream_cmd,
                            cm.get_config().get_int("server_port"),
                            bind(&App::process_data,
                                 this,placeholders::_1,
                                 placeholders::_2,
                                 placeholders::_3));
    static Tcpserver server_output(stream_output,
                                   cm.get_config().get_int("output_port"),
                                   bind(&App::process_data_from_output,
                                        this,placeholders::_1,
                                        placeholders::_2,
                                        placeholders::_3));
}

