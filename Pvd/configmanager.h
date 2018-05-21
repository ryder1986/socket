#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <iostream>
#include "tool.h"
using namespace std;
class FileDatabase
{
private:

    void save_file(const string data)
    {
        FILE *f=fopen(name.data(),"wb");
        int comp=fwrite(data.data(),1,data.size(),f);
        if(comp!=data.size())
            prt(info,"error in saving file");
        fclose(f);
    }
    bool load_file(string &data)
    {
        data.clear();
        FILE *f=fopen(name.data(),"rb");
        if(f==NULL)
            return false;
        static char ss[2];
        while(fread(ss,1,1,f)==1)
        {
            ss[1]=0;
            data.append(ss);
        }
        fclose(f);
        return true;
    }

public:
    FileDatabase(string file_name);
    void save(string data)
    {
        save_file(data);
    }
    bool load(string &data)
    {
        return load_file(data);
    }
    string load()
    {
        string data;
        load_file(data);
        return data;
    }
private:
    string name;
    string config;
};
class ConfigManager
{
public:
    ConfigManager();
};

#endif // CONFIGMANAGER_H
