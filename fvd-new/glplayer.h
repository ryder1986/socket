#ifndef __GLPLAYER__
#define __GLPLAYER__
#define WIDTH 640
#define HEIGHT 480

typedef struct gl_common {
	int window_id;
	unsigned char bf_src[WIDTH*HEIGHT*3/2];
	unsigned char bf_dst[WIDTH*HEIGHT*3];
} m_gl_common;

void init_gl();

void start_gl_window(int *p_w);
void stop_gl_window(int *p_w);

void diaplay_frames(unsigned char *buffer,int wid ,int hei,unsigned char  *buffer_convert,int w);
void copy_frame(unsigned char *picy, unsigned char *picu, unsigned char *picv,int wid,int hei, unsigned char *bf);

#endif
