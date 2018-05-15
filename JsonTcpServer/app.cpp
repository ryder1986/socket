#include "app.h"

App::App():
    server(50001,bind(&App::process_data,
                             this,placeholders::_1,
                             placeholders::_2,
                             placeholders::_3)),
    json_str("")
{

}

