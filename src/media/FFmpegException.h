
#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_EXCEPTION_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_EXCEPTION_H

#include <string>
#include <exception>

namespace slimenano::media {

class FFmpegException : public std::exception {

public:
    FFmpegException(const int errCode, const std::string errMsg);

    const std::string& Message() const;
    int ErrorCode() const;

private:
    const int m_errCode;
    const std::string m_errMsg;
};

} // namespace slimenano::media

#endif
