#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "pvd.h"
class PvdObject
{
public:
    PvdObject();
    virtual void decode_data()=0;
    DataPacket get_data()
    {
        return DataPacket();
    }

private:
    void encode_data()
    {

    }
};
template<typename TP>
class BaseConfig{
public:
    BaseConfig()
    {

    }

    void decode_data()
    {

    }

    void encode_data()
    {

    }
    DataPacket get_data()
    {
        return DataPacket();
    }

private:

};

#endif // PVDOBJECT_H
