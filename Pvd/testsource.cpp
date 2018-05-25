#include "testsource.h"

TestSource::TestSource()
{
    VideoSource src("rtsp://192.168.1.95:554/av0_1");
    Mat frame;
    while(1){
    if(src.get_frame(frame)){
        imshow("11",frame);
        waitKey(20);
    }

    }
}

