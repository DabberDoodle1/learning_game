#pragma once

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <vector>

class VideoEncoder {
public:
    VideoEncoder(const char* output_name, int width, int height, int framerate);
    VideoEncoder(const VideoEncoder& other) = delete;
    VideoEncoder(VideoEncoder&& other) = delete;
    VideoEncoder& operator=(const VideoEncoder&& other) = delete;
    VideoEncoder& operator=(VideoEncoder&& other) = delete;
    ~VideoEncoder();

    void add_frame();
private:
    AVCodecContext* codec_context;
    AVFormatContext* format_context;
    AVStream* stream;
    AVFrame* frame;
    AVPacket* packet;
    SwsContext* sws_context;
    const int width;
    const int height;

    std::vector<unsigned char> frame_buffer;
    std::vector<unsigned char> frame_flipped_buffer;
};
