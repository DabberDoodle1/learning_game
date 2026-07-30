#include "video_encoder.hpp"

#include <glad/glad.h> 

VideoEncoder::VideoEncoder(const char* output_name, int width, int height, int framerate): width(width), height(height), frame_buffer(width * height * 3), frame_flipped_buffer(width * height * 3) {
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    codec_context = avcodec_alloc_context3(codec);

    codec_context->width = width;
    codec_context->height = height;
    codec_context->pix_fmt = AV_PIX_FMT_YUV420P;
    codec_context->time_base = {1, framerate};
    codec_context->framerate = {framerate, 1};
    codec_context->gop_size = framerate;
    codec_context->max_b_frames = 2;
    codec_context->bit_rate = 5'000'000;

    AVDictionary* options = nullptr;
    av_dict_set(&options, "movflags", "faststart", 0);
    av_dict_set(&options, "level", "4.0", 0);
    avcodec_open2(codec_context, codec, &options);
    av_dict_free(&options);

    avformat_alloc_output_context2(&format_context, nullptr, nullptr, output_name);
    stream = avformat_new_stream(format_context, codec);
    avcodec_parameters_from_context(stream->codecpar, codec_context);
    stream->time_base = codec_context->time_base;

    avio_open(&format_context->pb, output_name, AVIO_FLAG_WRITE);
    int ehh = avformat_write_header(format_context, nullptr);

    frame = av_frame_alloc();
    frame->format = AV_PIX_FMT_YUV420P;
    frame->width = width;
    frame->height = height;
    av_frame_get_buffer(frame, 0);

    sws_context = sws_getContext(width, height, AV_PIX_FMT_RGB24, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
    packet = av_packet_alloc();
}

VideoEncoder::~VideoEncoder()
{
    avcodec_send_frame(codec_context, nullptr);
    while (avcodec_receive_packet(codec_context, packet) == 0)
    {
        av_packet_rescale_ts(packet, codec_context->time_base, stream->time_base);
        packet->stream_index = stream->index;
        av_interleaved_write_frame(format_context, packet);
        av_packet_unref(packet);
    }

    av_write_trailer(format_context);
    sws_freeContext(sws_context);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_context);
    avio_closep(&format_context->pb);
    avformat_free_context(format_context);
}

void VideoEncoder::add_frame()
{
    static unsigned int frame_index = 1;
    const unsigned int  rgb_width   = width * 3;

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer.data());

    for (int i = 0; i < height; ++i) {
        memcpy(frame_flipped_buffer.data() + i * rgb_width,
               frame_buffer.data() + frame_buffer.capacity() - i * rgb_width,
               rgb_width);
    }

    const uint8_t* rgb_data[] = { frame_flipped_buffer.data() };
    const int      linesize[] = { (int) rgb_width };

    sws_scale(sws_context, rgb_data, linesize, 0, height, frame->data, frame->linesize);
    frame->pts = frame_index++;

    avcodec_send_frame(codec_context, frame);
    while (avcodec_receive_packet(codec_context, packet) == 0)
    {
        av_packet_rescale_ts(packet, codec_context->time_base, stream->time_base);
        packet->stream_index = stream->index;
        av_interleaved_write_frame(format_context, packet);
        av_packet_unref(packet);
    }
}
