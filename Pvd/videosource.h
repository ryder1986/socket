#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include "tool.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
}
#endif
class MediaSource{
    typedef struct codec_common {
        AVFrame * p_av_frame;
        AVCodecContext * p_av_codec_ctx;
        AVCodec * p_av_codec;
        AVPacket av_pkt;
        AVFormatContext *fmt;
        unsigned char *oubuf;
        unsigned char *oubufu;
        unsigned char *oubufv;
        int frames;
    } m_codec_common;
public:

    MediaSource(string name )
    {
        open_url(name);
        //open_h264_decoder();
    }
    ~MediaSource()
    {
        close_url();
        //  close_h264_decoder();
    }

    bool get_frame(Mat &frame)
    {
        AVPacket h264_pkt;
        int ret=get_file_264buf(codec_info.fmt,&h264_pkt);
        if(!ret){
            prt(info,"sz %d",h264_pkt.size);
            prt(info,"frame pts %d",h264_pkt.pts);
            prt(info,"frame dts %d",h264_pkt.dts);
            prt(info,"frame offset %d",  codec_info.fmt->dump_separator);

            prt(info,"frame offset %d",  codec_info.fmt->ts_id);
            prt(info,"frame offset %d",  h264_pkt.pos);

            return true;
        }else{
            return false;
        }
    }


    int open_h264_decoder()
    {
        m_codec_common *codec_common=&codec_info;

        AVFrame ** p_p_av_frame=&codec_common->p_av_frame;
        AVCodecContext ** p_p_av_codec_ctx=&codec_common->p_av_codec_ctx;
        AVCodec ** p_p_av_codec=&codec_common->p_av_codec;

        av_register_all();
        *p_p_av_codec = avcodec_find_decoder(CODEC_ID_H264);
        *p_p_av_codec_ctx = avcodec_alloc_context3(*p_p_av_codec);
        if (avcodec_open2(*p_p_av_codec_ctx, *p_p_av_codec, NULL) >= 0)
            *p_p_av_frame= avcodec_alloc_frame();
        else {
            //	H264DeocderRelease(de);
            return -1;
        }
        return 0;
    }
    int close_h264_decoder()
    {
        m_codec_common *common=&codec_info;

        if (common->p_av_frame) {
            av_free(common->p_av_frame);
            common->p_av_frame=NULL;
        }
        if (common->p_av_codec_ctx) {
            avcodec_close(common->p_av_codec_ctx);
            common->p_av_codec_ctx=NULL;
        }

        return 1;
    }
    int open_url(string url  )
    {
        av_register_all();
        codec_info.fmt   = avformat_alloc_context();
        if (avformat_open_input(&     codec_info.fmt , url.data(), NULL, 0) != 0) {
            //  exit_program()
            ;
        } else if (avformat_find_stream_info(codec_info.fmt, NULL) < 0) {
            //  exit_program()
            ;
        }
        codec_info.p_av_codec = avcodec_find_decoder(CODEC_ID_H264);
        codec_info.p_av_codec_ctx = avcodec_alloc_context3(codec_info.p_av_codec);
        if (avcodec_open2(codec_info.p_av_codec_ctx ,   codec_info.p_av_codec, NULL) >= 0)
            codec_info.p_av_frame = avcodec_alloc_frame();
        else {
            return -1;
        }
        return 0;
    }
    void close_url( )
    {
        avcodec_close(codec_info.p_av_codec_ctx);
        avformat_close_input(&codec_info.fmt);
    }

    int get_file_264buf(AVFormatContext *av_format_context,AVPacket *h264_pkt)
    {
        av_init_packet(h264_pkt);
        if ((av_read_frame(av_format_context, h264_pkt) >= 0)) {
            prt(info,"read frm");
            return 0;
        }
        return -1;
    }
    int ffmeg_decode_video1(AVPacket *pkt,AVCodecContext *m_AVCodeContext, AVFrame *m_AVFrame)
    {
        int got_picture = 0;
        int len = 0;
        av_init_packet(pkt);
        while (pkt->size > 0) {
            len = avcodec_decode_video2(m_AVCodeContext, m_AVFrame, &got_picture, pkt);
            if (len < 0) {
                return -1;
            }
            if (got_picture) {
                return 0;
                break;
            }
            pkt->size -= len;
            pkt->data += len;
        }
        return 1;
    }
private:
    m_codec_common codec_info;
};

class VideoSource
{
public:

    VideoSource(string path);
    ~VideoSource();
    inline string get_url()
    {
        return url;
    }
    bool get_size(int &w,int &h)
    {
        bool ret=false;
        if(vcap.isOpened()){
            ret=true;
            w=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
            h=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
        }
        return ret;
    }
    bool get_frame(Mat &frame)
    {
        int ret=false;
      //  Mat frame1;
        lock.lock();
        if(frame_list.size()>0){
            frame_list.front().copyTo(frame);
            frame_list.pop_front();
            ret=true;
        }else{
            ret=false;
        }
        lock.unlock();
        return ret;
    }


private:
    void run();
    void check_point()
    {
        if(vcap.isOpened()){
            double w= vcap.get(CV_CAP_PROP_POS_FRAMES);
            prt(info,"%s src frames %lf",url.data(),w);
        }
    }

private:
    VideoCapture vcap;
    list <Mat> frame_list;
    int frame_wait_time;
    mutex lock;
    int frame_rate;
    string url;
    volatile bool quit_flg;
    Timer1 t1;

};

#endif // VIDEOSOURCE_H
