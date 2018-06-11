#ifndef PVDPROCESSOR_H
#define PVDPROCESSOR_H
#include "tool.h"
#include "videoprocessor.h"
#include "c4common.h"
#include "pvd.h"
#include "track/Ctracker.h"

#include "pvdobject.h"
#define KALMAN_LOST_FRAME_THREDHOLD 5
#define KALMAN_TRACE_LEN 100

#define C4FILE "res/combined.txt.model"
#define alg_c4_file2 "res/combined2.txt.model"
#define config_file "res/config.json"

#include "pvdobject.h"

//template<typename TP>
//class C4Config:public BaseConfig<TP>
//{
//public:
//    C4Config(TP)
//    {

//    }
//};

#include "pvd.h"
template<typename TP>
class JsonDataDealer{
//    DataPaket data;
    TP *p_arg;
public:
    JsonDataDealer(DataPacket data,TP &arg)
    {
        trans(arg,data);
        p_arg=&arg;
    }
    void trans(DataPacket &data,TP arg)
    {

    }
    void trans(TP &arg,DataPacket data)
    {

    }

    DataPacket get()
    {
        DataPacket ret;
        trans(ret,*p_arg);
        return ret;
    }
};
class PvdC4Processor : public VideoProcessor
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
        int flow;
        vector <Rect> rects;
    }m_result;

public:
    void decode_data()
    {

    }

    PvdC4Processor(JsonValue jv):VideoProcessor()
    {
        loaded=false;
        set_config(jv);
        p_scanner=new DetectionScanner(HUMAN_height,HUMAN_width,HUMAN_xdiv,
                                       HUMAN_ydiv,256,arg.scale_ratio);
        //        tracker=new CTracker(0.2f, 0.1f, 60.0f, 5, 100);
        tracker=new CTracker(0.2f, 0.1f, 60.0f,
                             KALMAN_LOST_FRAME_THREDHOLD,
                             KALMAN_TRACE_LEN);

    }

    PvdC4Processor(DataPacket pkt):VideoProcessor()
    {
        loaded=false;

        arg.scale_ratio=atof((pkt.get_string("ratio").data()));
        arg.scan_step=pkt.get_int("step");
        //        arg.no=pkt.get_int("channel_id");
        //        channel_id= arg.no;
        vector <DataPacket> area=pkt.get_array_packet("detect_area");
        arg.area=area_2_rect(area);

        p_scanner=new DetectionScanner(HUMAN_height,HUMAN_width,HUMAN_xdiv,
                                       HUMAN_ydiv,256,arg.scale_ratio);
        //        tracker=new CTracker(0.2f, 0.1f, 60.0f, 5, 100);
        tracker=new CTracker(0.2f, 0.1f, 60.0f,
                             KALMAN_LOST_FRAME_THREDHOLD,
                             KALMAN_TRACE_LEN);

    }
    PvdC4Processor(JsonValue jv,int cid):VideoProcessor()
    {
        loaded=false;
        set_config(jv);
        p_scanner=new DetectionScanner(HUMAN_height,HUMAN_width,HUMAN_xdiv,HUMAN_ydiv,256,arg.scale_ratio);
        //        tracker=new CTracker(0.2f, 0.1f, 60.0f, 5, 100);
        tracker=new CTracker(0.2f, 0.1f, 60.0f,KALMAN_LOST_FRAME_THREDHOLD, KALMAN_TRACE_LEN);

    }


    ~PvdC4Processor()
    {
        delete p_scanner;
        delete tracker;
    }

    void set_config(JsonValue jv)
    {
        DataPacket pkt(jv);
        arg.scale_ratio=atof((pkt.get_string("ratio").data()));
        arg.scan_step=pkt.get_int("step");
        //        arg.no=pkt.get_int("channel_id");
        //        channel_id= arg.no;
        vector <JsonValue> area=pkt.get_array("detect_area");
        arg.area=area_2_rect(area);
    }
    string get_rst()
    {
        return alg_rst;
    }

    bool process(Mat img_src)
    {
        Mat img=img_src(arg.area);
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
        if(real_process(img,r)){
            ret=true;
        }else
            ret=false;


        vector<DataPacket> ja;
        for (Rect rct:r.rects) {
            DataPacket pkt_rct;
            pkt_rct.set_value("x",rct.x+arg.area.x);
            pkt_rct.set_value("y",rct.y+arg.area.y);
            pkt_rct.set_value("w",rct.width);
            pkt_rct.set_value("h",rct.height);
            ja.push_back(pkt_rct);
        }
        return ret;
    }
    bool process(Mat img_src,vector<Rect> &rects)
    {
        Mat img=img_src(arg.area);
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
        if(real_process(img,r)){
            ret=true;
        }else
            ret=false;

        rects.clear();
        rects=r.rects;
        return ret;
    }

    virtual bool process(Mat img_src,vector<Rect> &rects,Rect detect_area)
    {
        bool ret=false;
        m_result r;

        r.width=img_src.cols;
        r.height=img_src.rows;

        Mat img=img_src(detect_area);
        if(real_process(img,r)){
            ret=true;
        }else
            ret=false;
        rects.clear();
        rects=r.rects;
        return  ret;
    }
private:
    Rect area_2_rect()
    {
        return Rect(0,0,640,480);
    }
    Rect area_2_rect(vector<JsonValue> area)
    {
        int x_min=10000;
        int y_min=10000;
        int x_max=0;
        int y_max=0;
        for(JsonValue v: area) {
            DataPacket pkt(v);
            int x=pkt.get_int("x");
            int y=pkt.get_int("y");
            //              int x= v.toObject()["x"].toInt();
            //            int y= v.toObject()["y"].toInt();
            if(x<x_min)
                x_min=x;
            if(x>x_max)
                x_max=x;
            if(y<y_min)
                y_min=y;
            if(y>y_max)
                y_max=y;
        }
        return Rect(x_min,y_min,x_max-x_min,y_max-y_min);
    }
    Rect area_2_rect(vector<DataPacket> area)
    {
        int x_min=10000;
        int y_min=10000;
        int x_max=0;
        int y_max=0;
        for(DataPacket pkt: area) {
            int x=pkt.get_int("x");
            int y=pkt.get_int("y");
            //              int x= v.toObject()["x"].toInt();
            //            int y= v.toObject()["y"].toInt();
            if(x<x_min)
                x_min=x;
            if(x>x_max)
                x_max=x;
            if(y<y_min)
                y_min=y;
            if(y>y_max)
                y_max=y;
        }
        return Rect(x_min,y_min,x_max-x_min,y_max-y_min);
    }
    const int HUMAN_height = 108;
    const int HUMAN_width = 36;
    const int HUMAN_xdiv = 9;
    const int HUMAN_ydiv = 4;


    // The detector

    // ---------------------------------------------------------------------
    // Helper functions


    // compute the Sobel image "ct" from "original"

    // A simple post-process (NMS, non-maximal suppression)
    // "result" -- rectangles before merging
    //          -- after this function it contains rectangles after NMS
    // "combine_min" -- threshold of how many detection are needed to survive
    void PostProcess(std::vector<CRect>& result,const int combine_min)
    {
        std::vector<CRect> res1;
        std::vector<CRect> resmax;
        std::vector<int> res2;
        bool yet;
        CRect rectInter;

        for(unsigned int i=0,size_i=result.size(); i<size_i; i++)
        {
            yet = false;
            CRect& result_i = result[i];
            for(unsigned int j=0,size_r=res1.size(); j<size_r; j++)
            {
                CRect& resmax_j = resmax[j];
                if(result_i.Intersect(rectInter,resmax_j))
                {
                    if(  rectInter.Size()>0.6*result_i.Size()
                         && rectInter.Size()>0.6*resmax_j.Size()
                         )
                    {
                        CRect& res1_j = res1[j];
                        resmax_j.Union(resmax_j,result_i);
                        res1_j.bottom += result_i.bottom;
                        res1_j.top += result_i.top;
                        res1_j.left += result_i.left;
                        res1_j.right += result_i.right;
                        res2[j]++;
                        yet = true;
                        break;
                    }
                }
            }
            if(yet==false){
                res1.push_back(result_i);
                resmax.push_back(result_i);
                res2.push_back(1);
            }
        }

        for(unsigned int i=0,size=res1.size(); i<size; i++)
        {
            const int count = res2[i];
            CRect& res1_i = res1[i];
            res1_i.top /= count;
            res1_i.bottom /= count;
            res1_i.left /= count;
            res1_i.right /= count;
        }

        result.clear();
        for(unsigned int i=0,size=res1.size(); i<size; i++)
            if(res2[i]>combine_min)
                result.push_back(res1[i]);
    }

    // If one detection (after NMS) is inside another, remove the inside one
    void RemoveCoveredRectangles(std::vector<CRect>& result)
    {
        std::vector<bool> covered;
        covered.resize(result.size());
        std::fill(covered.begin(),covered.end(),false);
        CRect inter;
        for(unsigned int i=0; i<result.size(); i++)
        {
            for(unsigned int j=i+1; j<result.size(); j++)
            {
                result[i].Intersect(inter,result[j]);
                double isize = inter.Size();
                if(isize>result[i].Size()*0.65)
                    covered[i] = true;
                if(isize>result[j].Size()*0.65)
                    covered[j] = true;
            }
        }
        std::vector<CRect> newresult;
        for(unsigned int i=0; i<result.size(); i++)
            if(covered[i]==false)
                newresult.push_back(result[i]);
        result.clear();
        result.insert(result.begin(),newresult.begin(),newresult.end());
        newresult.clear();
    }

    void LoadCascade(DetectionScanner& ds)
    {
        std::vector<NodeDetector::NodeType> types;
        std::vector<int> upper_bounds;
        std::vector<std::string> filenames;

        types.push_back(NodeDetector::CD_LIN); // first node
        //upper_bounds.push_back(256);	//100

#if 1

        upper_bounds.push_back(100);	//100
        // filenames.push_back("combined.txt.model");
        filenames.push_back(C4FILE);
        types.push_back(NodeDetector::CD_HIK); // second node
        //upper_bounds.push_back(117);	//353

        upper_bounds.push_back(20);	//20   small model ,like 3M
        //      upper_bounds.push_back(50);	//353
        //         // upper_bounds.push_back(353);	//353
        // filenames.push_back("combined2.txt.model");
        filenames.push_back(alg_c4_file2);
#else
        upper_bounds.push_back(100);	//100
        filenames.push_back("combined.txt.model");
        types.push_back(NodeDetector::CD_HIK); // second node
        //upper_bounds.push_back(117);	//353

        upper_bounds.push_back(256);	//353  big model,like 30M
        //      upper_bounds.push_back(50);	//353
        //         // upper_bounds.push_back(353);	//353
        filenames.push_back("combined2.txt.model");
#endif
        ds.LoadDetector(types,upper_bounds,filenames);
        // You can adjust these parameters for different speed, accuracy etc
        //   ds.cascade->nodes[0]->thresh += 0.8;
        ds.cascade->nodes[0]->thresh += arg.scale_ratio;
        ds.cascade->nodes[1]->thresh -= 0.095;
    }





    bool real_process( Mat &src_image,m_result &rst)
    {
        std::vector<cv::Rect>  result_rects;
        std::vector<Point_t>  centers;
        bool ret=false;
        if(!loaded){
            LoadCascade(*p_scanner);
            std::cout<<"Detectors loaded."<<std::endl;
            loaded=true;
        }


        int step_size = 9;
        //int step_size = 2;
        //   float rate = 0.5;
        bool rect_organization = true;
        IntImage<double> original;

        if(result_rects.size() != 0)
        {
            result_rects.clear();
        }

        double start_time;
        start_time = cv::getTickCount();

        if( src_image.empty() )
        {
            return ret;
        }
        cv::Mat detect_region = src_image;

        original.Load( detect_region );
        std::vector<CRect> results;
        p_scanner->FastScan(original, results, step_size);

        if(rect_organization)
        {
            PostProcess(results,2);
            PostProcess(results,0);
            RemoveCoveredRectangles(results);
        }
#if 0
        for(size_t i = 0; i < results.size(); i++)
        {
            cv::Rect real_position;
            real_position.x = results[i].left/2;
            real_position.y = results[i].top/2;
            real_position.width = (results[i].right - results[i].left)/2;
            real_position.height = (results[i].bottom - results[i].top)/2;

            //   cv::rectangle(detect_region, real_position, cv::Scalar(0,255,0), 2);
            result_rects.push_back(real_position);
        }
#else

        for(size_t i = 0; i < results.size(); i++)
        {
            cv::Rect real_position;
            real_position.x = results[i].left;
            real_position.y = results[i].top;
            real_position.width = (results[i].right - results[i].left);
            real_position.height = (results[i].bottom - results[i].top);
            //   cv::rectangle(detect_region, real_position, cv::Scalar(0,255,0), 2);
            result_rects.push_back(real_position);

            centers.push_back(Point_t(real_position.x + ((float)real_position.width/2),
                                      real_position.y + ((float)real_position.height/2)));

        }

        tracker->Update(centers, result_rects, CTracker::RectsDist);






#endif
        double end_time = cv::getTickCount();
        double spend_time;
        spend_time = 1000 * (fabs(end_time - start_time) / cv::getTickFrequency());
        //     std::cout << "time : " << spend_time << " ms" << std::endl;

        if(result_rects.size()>0)
        {
            ret=true;
        }
        rst.rects=result_rects;

#if 1

#endif
        return ret;

    }

private:
    bool loaded;
    //  DetectionScanner scanner;
    DetectionScanner *p_scanner;
    CTracker *tracker;


};
#endif // PVDPROCESSOR_H
