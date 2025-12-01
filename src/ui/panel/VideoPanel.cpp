#include "../../pch.h" // IWYU pragma: keep

#include "../IDs.h"
#include "../../app/Events.h"
#include "VideoPanel.h"

namespace slimenano::ui {

void ShowFFmpegError(const wxString& title, int errNum) {
    char errbuf[1024]{0};
    av_strerror(errNum, errbuf, sizeof(errbuf));

    wxString errMsg = wxString::FromUTF8(errbuf);

    wxMessageBox(
        wxString::Format(_("Open video failed: %d (%s)"), errNum, errMsg),
        title,
        wxOK | wxICON_ERROR
    );
}

VideoPanel::VideoPanel(wxFrame* pParentFrame) :
    wxPanel(pParentFrame, ID_PANEL_VIDEO_PANEL),
    m_pParentFrame(pParentFrame),
    m_timer(this) {

    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(EVT_OPEN_VIDEO, &VideoPanel::OnOpenVideo, this);
    Bind(wxEVT_PAINT, &VideoPanel::OnPaint, this);
    Bind(wxEVT_TIMER, &VideoPanel::OnTimer, this);
}

void VideoPanel::StartTimer(int intervalMs) {
    m_startClock = wxGetUTCTimeMillis().ToDouble() / 1000.0;
    m_timer.Start(intervalMs);
}

void VideoPanel::StopTimer() {
    m_timer.Stop();
}

void VideoPanel::OnTimer([[maybe_unused]] wxTimerEvent& event) {

    // double clock = wxGetUTCTimeMillis().ToDouble() / 1000.0 - m_startClock;

    // if (m_pts != 0 && clock < (m_pFrame->best_effort_timestamp * m_timeBase - m_pts)) {
    //     return;
    // }

    if (DecodeNextFrame()) {
        ConvertToBitmap(m_pFrame);
        Refresh();
    } else {
        CloseVideo();
    }
}

void VideoPanel::OnPaint([[maybe_unused]] wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    if (m_bitmap.IsOk()) {
        dc.DrawBitmap(m_bitmap, 0, 0, false);
    }

    if (!m_overlay.IsEmpty()) {
        dc.SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        dc.SetTextForeground(*wxRED);
        dc.DrawText(m_overlay, 10, 10);
    }
}

void VideoPanel::SetOverlay(const wxString& text) {
    m_overlay = text;
    Refresh();
    Update();
}

void VideoPanel::OnOpenVideo([[maybe_unused]] const wxCommandEvent& event) {

    CloseVideo();

    std::string path = event.GetString().ToUTF8().data();

    int errNum = avformat_open_input(&m_pFormatContext, path.c_str(), nullptr, nullptr);
    if (errNum != 0) {
        ShowFFmpegError(_("Error on Open Video"), errNum);
        CloseVideo();
        return;
    }

    errNum = avformat_find_stream_info(m_pFormatContext, nullptr);
    if (errNum != 0) {
        ShowFFmpegError(_("Error on Open Video"), errNum);
        CloseVideo();
        return;
    }

    for (unsigned int i = 0; i < m_pFormatContext->nb_streams; ++i) {
        if (m_pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStreamIndex = i;
            break;
        }
    }

    if (m_videoStreamIndex < 0) {
        wxMessageBox(
            wxString::Format(_("Open video failed: 0 (%s)"), errNum, "Not found video stream."),
            _("Error on Open Video"),
            wxOK | wxICON_ERROR
        );
        CloseVideo();
        return;
    }

    auto* codecpar = m_pFormatContext->streams[m_videoStreamIndex]->codecpar;
    auto codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) {
        wxMessageBox(
            wxString::Format(_("Open video failed: 0 (%s)"), errNum, "Not found decoder"),
            _("Error on Open Video"),
            wxOK | wxICON_ERROR
        );
        CloseVideo();
        return;
    }

    m_pCodecContext = avcodec_alloc_context3(codec);
    errNum = avcodec_parameters_to_context(m_pCodecContext, codecpar);
    if (errNum != 0) {
        ShowFFmpegError(_("Error on Open Video"), errNum);
        CloseVideo();
        return;
    }

    errNum = avcodec_open2(m_pCodecContext, codec, nullptr);
    if (errNum != 0) {
        ShowFFmpegError(_("Error on Open Video"), errNum);
        CloseVideo();
        return;
    }

    m_pPacket = av_packet_alloc();
    m_pFrame = av_frame_alloc();

    m_pSwsContext = sws_getContext(
        m_pCodecContext->width,
        m_pCodecContext->height,
        m_pCodecContext->pix_fmt,
        m_pCodecContext->width,
        m_pCodecContext->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        nullptr,
        nullptr,
        nullptr
    );

    m_fps = av_q2d(m_pFormatContext->streams[m_videoStreamIndex]->avg_frame_rate);
    if (m_fps <= 1e-3) {
        m_fps = 30.0;
    }

    m_timeBase = av_q2d(m_pFormatContext->streams[m_videoStreamIndex]->time_base);

    SetOverlay(
        wxString::Format(
            "File: %s\n"
            "Format: %s\n"
            "Duration: %lld us\n"
            "Resolution %d x %d\n"
            "Fps: %f\n",
            path,
            m_pFormatContext->iformat->long_name,
            m_pFormatContext->duration,
            codecpar->width,
            codecpar->height,
            m_fps
        )
    );

    StartTimer(0);
}

void VideoPanel::CloseVideo() {

    StopTimer();

    m_videoStreamIndex = -1;

    if (m_pSwsContext) {
        sws_freeContext(m_pSwsContext);
        m_pSwsContext = nullptr;
    }

    if (m_pPacket) {
        av_packet_free(&m_pPacket);
    }

    if (m_pFrame) {
        av_frame_free(&m_pFrame);
    }

    if (m_pCodecContext) {
        avcodec_free_context(&m_pCodecContext);
    }

    if (m_pFormatContext) {
        avformat_close_input(&m_pFormatContext);
    }
}

bool VideoPanel::DecodeNextFrame() {

    int readRet = 0;
    do {
        readRet = av_read_frame(m_pFormatContext, m_pPacket);
        if (m_pPacket->stream_index == m_videoStreamIndex) {
            auto sendRet = avcodec_send_packet(m_pCodecContext, m_pPacket);
            while (sendRet == AVERROR(EAGAIN)) {
                sendRet = avcodec_send_packet(m_pCodecContext, m_pPacket);
            }
            if (sendRet == 0) {
                auto recvRet = avcodec_receive_frame(m_pCodecContext, m_pFrame);
                while (recvRet == AVERROR(EAGAIN)) {
                    recvRet = avcodec_receive_frame(m_pCodecContext, m_pFrame);
                }
                if (recvRet == 0) {
                    av_packet_unref(m_pPacket);
                    return true;
                }
            }
        }
        av_packet_unref(m_pPacket);
    } while (readRet >= 0);

    return false;
}

void VideoPanel::ConvertToBitmap(AVFrame* frame) {
    int w = m_pCodecContext->width;
    int h = m_pCodecContext->height;

    wxImage img(w, h);
    uint8_t* dest = img.GetData(); // RGB24 buffer

    uint8_t* dstSlice[1] = {dest};
    int dstStride[1] = {w * 3};

    sws_scale(
        m_pSwsContext,
        frame->data,
        frame->linesize,
        0,
        h,
        dstSlice,
        dstStride
    );

    m_bitmap = wxBitmap(img);
}

} // namespace slimenano::ui
