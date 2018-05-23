#include <iostream>
#include "videosource.h"
using namespace std;

int main()
{
    cout << " start !" << endl;

     // VideoSource src("rtsp://192.168.1.95:554/av0_1");
  VideoSource src("rtsp://192.168.1.216:8555/test1");


    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    return 0;
}

