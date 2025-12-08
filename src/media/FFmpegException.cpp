#include "../pch.h" // IWYU pragma: keep

#include "FFmpegException.h"

namespace slimenano::media {

FFmpegException::FFmpegException(const int errCode, const std::string errMsg) :
    m_errCode(errCode), m_errMsg(errMsg) {
}

const std::string& FFmpegException::Message() const {
    return m_errMsg;
}

int FFmpegException::ErrorCode() const {
    return m_errCode;
}

} // namespace slimenano::media
