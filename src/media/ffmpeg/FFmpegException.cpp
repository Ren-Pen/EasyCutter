#include "../pch.h" // IWYU pragma: keep

#include "FFmpegException.h"

namespace slimenano::media {

std::string GetFFmpegError(int errCode) {
    char errbuf[1024]{0};
    av_strerror(errCode, errbuf, sizeof(errbuf));
    return errbuf;
}

FFmpegException::FFmpegException(const int errCode, const std::string errMsg) :
    m_errCode(errCode), m_errMsg(errMsg) {
}

FFmpegException::FFmpegException(const int errCode) :
    m_errCode(errCode), m_errMsg(GetFFmpegError(errCode)) {
}

const std::string& FFmpegException::Message() const {
    return m_errMsg;
}

int FFmpegException::ErrorCode() const {
    return m_errCode;
}

} // namespace slimenano::media
