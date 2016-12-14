cmake_minimum_required(VERSION 2.8)

set(TOPDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OUTDIR ${TOPDIR}/out/)
set(BUILDDIR ${TOPDIR}/build/)
set(COMPILE_DEFINITIONS -Werror)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(EXECUTABLE_OUTPUT_PATH ${OUTDIR})

add_executable(muxing muxing.c)
add_executable(decoding_encoding decoding_encoding.c)
add_executable(remuxing remuxing.c)
add_executable(metadata metadata.c)
add_executable(avio_reading avio_reading.c)
add_executable(ffmpeg_recv ffmpeg_recv.c)
add_executable(ffmpeg_stream ffmpeg_stream.c)

#SET(AV_LIB pthread avdevice avfilter postproc avformat avcodec xcb-shm xcb X11 asound x264 pthread rtmp gnutls mp3lame freetype fdk-aac z swresample swscale avutil rt m) 

SET(AV_LIB avfilter avformat avdevice avcodec swscale avutil swresample X11 vdpau lzma pthread dl rt m z)

target_link_libraries(muxing ${AV_LIB}) 
target_link_libraries(decoding_encoding ${AV_LIB})
target_link_libraries(remuxing ${AV_LIB})
target_link_libraries(metadata ${AV_LIB})
target_link_libraries(avio_reading ${AV_LIB})
target_link_libraries(ffmpeg_recv ${AV_LIB})
target_link_libraries(ffmpeg_stream ${AV_LIB})
