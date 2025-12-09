target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/decode/FFmpegDecoder.cpp)
target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/decode/FFmpegDemuxer.cpp)
target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/utils/FFmpegAVPacket.cpp)
target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/FFmpegException.cpp)