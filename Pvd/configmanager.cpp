#include "configmanager.h"

#include "pvd.h"
DataPacket ConfigManager::get_config()
{
    string data;
    if(db.load(data)){
        return DataPacket(data);
    }else{
        return JsonValue();
    }
}
