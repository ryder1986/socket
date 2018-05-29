#ifndef PVDHOGPROCESSOR_H
#define PVDHOGPROCESSOR_H
#include "videoprocessor.h"
#define HOGFILE "res/hogcascade_pedestrians.xml"
class PvdHogProcessor : public VideoProcessor
{
    typedef struct args{
        double scale_ratio;
        int scan_step;
        Rect area;
        int no;
    }arg_t;
    arg_t arg;
    typedef struct process_result{
        int width;
        int height;
        bool exist;
        int count;
        int front_count;
        int back_count;
        int other_count;
        int duration;
        vector <Rect> rects;
    }m_result;

public:
    PvdHogProcessor(JsonValue jv,int id):VideoProcessor()
    {
        arg.no=id;
        set_config(jv);
    }
    PvdHogProcessor(DataPacket pkt , vector <Rect> rct):VideoProcessor()
    {

        //  set_config(jv);
    }
    PvdHogProcessor(DataPacket):VideoProcessor()
    {

        //  set_config(jv);
    }
    string get_rst()
    {
        return alg_rst;
    }
    void set_config(JsonValue jv)
    {
        DataPacket pkt(jv);


        //   arg.no=pkt.get_int("channel_id");
        vector <JsonValue> area=pkt.get_array("detect_area");
        //  arg.area=area_2_rect(area);
    }


    bool process(Mat img_src)
    {
        // Mat img=img_src(arg.area);
        Mat img=img_src;
        m_result r;
        r.width=img_src.cols;
        r.height=img_src.rows;
        r.back_count=0;
        r.front_count=0;
        r.count=0;
        r.exist=false;
        r.duration=0;
        r.other_count=0;
        bool ret=false;
        vector <Rect> v;
        if(real_process(img,v)){
            ret=true;
        }else
            ret=false;
        r.rects=v;

        DataPacket pkt;
        pkt.set_value("width",r.width);
        pkt.set_value("height",r.height);
        pkt.set_value("back_count",r.back_count);
        pkt.set_value("front_count",r.front_count);
        pkt.set_value("count",r.count);
        pkt.set_value("exist",r.exist);
        pkt.set_value("duration",r.duration);
        pkt.set_value("other_count",r.other_count);

        //        vector<JsonValue> ve;
        //        foreach (Rect rct,r.rects) {
        //            DataPacket pkt_rct;
        //            pkt_rct.set_value("x",rct.x+arg.area.x);
        //            pkt_rct.set_value("y",rct.y+arg.area.y);
        //            //            pkt_rct.set_value("x",rct.x+arg.area.x);
        //            //            pkt_rct.set_value("y",rct.y+arg.area.y);
        //            pkt_rct.set_value("w",rct.width);
        //            pkt_rct.set_value("h",rct.height);
        //            ve.push_back(pkt_rct.value());
        //            prt(info,"1 person in %d %d %d %d",pkt_rct.get_int("x"),pkt_rct.get_int("y"),pkt_rct.get_int("w"),pkt_rct.get_int("h"));
        //        }

        return ret;
    }

    virtual bool process(Mat img_src,vector<Rect> &rects,Rect detect_area)
    {
        real_process(img_src,rects);

    }
private:

    bool real_process(Mat &mt, std::vector<cv::Rect> &result_rects)
    {
        //  std::vector<cv::Rect> result_rects;
        result_rects.clear();
        CascadeClassifier cascade;
        bool ret=false;
        vector<Rect> objs;
        string cascade_name =HOGFILE;
        if (!cascade.load(cascade_name))
        {
            prt(info,"can't load cascade");
            exit(0);
        }

        Mat frame(mt);
        string str;
        str.clear();

        if (!frame.empty())
        {
            if (1)
            {
              //  resize(frame,frame,Size(frame.cols / 2, frame.rows / 2),CV_INTER_LINEAR);
                cvtColor(frame, gray_frame, CV_BGR2GRAY);
                cascade.detectMultiScale(gray_frame, objs, 1.1, 3);
                vector<Rect>::iterator it = objs.begin();
                while (it != objs.end() && objs.size() != 0)
                {
                    Rect rct = *it;
                    {
                        rectangle(frame, rct, Scalar(0, 255, 0), 2);
                        result_rects.push_back(rct);
                        ret=true;
                    }
                    it++;
                }
                objs.clear();
            }
        }
        else{
            prt(info,"opencv handle frame error !");
        }
        if(result_rects.size()>0)
            ret=true;
        return ret;
    }
private:
    Mat gray_frame;
};


#endif // PVDHOGPROCESSOR_H
