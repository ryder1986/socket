SUBDIR=.
SOURCE=$(foreach n, $(SUBDIR),$(wildcard $(n)/*.c))

INCDIR=include
INCLUDE_FILE=$(foreach n, $(INCDIR),$(wildcard $(n)/*.h))
DBG_ENABLE=1
#exclude_files:=client_config_main.c
#SOURCE:=$(filter-out $(exclude_files),$(SOURCE))
SOURCE:= cam_alg.c         cam_net.c             common.c             sig_service.c\
cam_codec.c       client_obj.c              csocket.c  h264_stream_file.c  tcp_server.c \
camera_service.c    file_op.c  main.c client_file.c client_net.c 
OBJS:=$(SOURCE:%.c=%.o)

#CFLGS +=-I./
#LFLGS +=-L/usr/lib -lpthread  
#-DUSE_FILE -DPLAY_BACK
#exclude_dirs=bin lib include config rtp server $(ALG_SRC)
#dirs:=$(shell find . -maxdepth 1 -type d)
#dirs:=$(basename $(patsubst ./%,%,$(dirs)))
#dirs:=$(filter-out $(exclude_dirs),$(dirs))
CFLGS +=-I./include       #  -DHW_DECODE       #  -DUSE_FILE  -DPLAY_BACK
CFLGS +=-I/fvd_extra_include  -I./alg-mutilthread/
CFLGS +=-I./include/fvd_extra_include/GL
CFLGS +=-I./include/fvd_extra_include/
CFLGS += -fopenmp
CFLGS_EXTRA +=-I../fvd_extra_include  -I../fvd_extra_include/GL 
LIBS_EXTRA=./lib
LFLGS +=     \
    -Wl,--rpath=$(LIBS_EXTRA)/va/lib    \
	-Wl,--rpath=$(LIBS_EXTRA)/ffmpeg_2_5_3/lib \
	-Wl,--rpath=./ \
	-Wl,--rpath=./lib \
	-L$(LIBS_EXTRA)/ffmpeg_2_5_3/lib \
	-lavcodec -lavformat -lavutil -lswresample \
    -export-dynamic -w    \
 	-lpthread -L./lib -lzenalg \
 	-Wl,--rpath=./lib/opencv2  -L./lib/va/lib  -L./lib/opencv2 -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_video 
LFLGS += -lgomp
ifeq (1, ${DBG_ENABLE})
	CFLGS += -D_DEBUG -O0 -g -DDEBUG=1 -w
endif
all: zenith 
fvd-config:
	g++  file_op.c  client_config_main.c  -w  common.c -lpthread csocket.c  client_file.c client_obj.c -o fvd-config
zenith:$(OBJS) $(INCLUDE_FILE)
#	make -C alg-mutilthread/
#	cp alg-mutilthread/libzenalg.so lib
	g++ $(OBJS) -o $@   $(LFLGS) ./lib/libNet.a

$(OBJS): %.o: %.c
#	g++ -c $(CFLAGS) $< -o $@ 	$(CFLGS)  $(CFLGS_EXTRA)   -DHANHUI
	g++ -c $(CFLAGS) $< -o $@ 	$(CFLGS)  $(CFLGS_EXTRA)   
clean:
	rm -rf *.o 

install:
	mkdir .install
	cp -r fvd .install/zenith 
	cp server/ipsearch .install
	cp bin/wardens .install
	cp bin/wardens.service .install
	cp bin/set_up_fedora20.sh .install
	tar cvzf  .install/lib.tar.gz lib
	cp -r cfg  .install
	mkdir .install/log
