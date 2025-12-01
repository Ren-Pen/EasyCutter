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
    wxPanel(pParentFrame, ID_PANEL_VIDEO_PANEL), m_pParentFrame(pParentFrame) {

    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(EVT_OPEN_VIDEO, &VideoPanel::OnOpenVideo, this);
    Bind(wxEVT_PAINT, &VideoPanel::OnPaint, this);
    Bind(wxEVT_TIMER, &VideoPanel::OnTimer, this);
}

void VideoPanel::StartTimer(int intervalMs) {
    m_timer.Start(intervalMs);
}

void VideoPanel::StopTimer() {
    m_timer.Stop();
}

void VideoPanel::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    Refresh();
}

void VideoPanel::OnPaint([[maybe_unused]] wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    if (m_frame.IsOk()) {
        dc.DrawBitmap(m_frame, 0, 0, false);
    }

    if (!m_overlay.IsEmpty()) {
        dc.SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        dc.SetTextForeground(*wxRED);
        dc.DrawText(m_overlay, 10, 10);
    }
}

void VideoPanel::SetFrame(const wxBitmap& bmp) {
    m_frame = bmp;
    Refresh();
    Update();
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

    SetOverlay(wxString::Format(
        "File: %s\n"
        "Format: %s\n"
        "Duration: %lld us\n"
        "Resolution %d x %d\n",
        path,
        m_pFormatContext->iformat->long_name,
        m_pFormatContext->duration,
        codecpar->width,
        codecpar->height
    ));
}

void VideoPanel::CloseVideo() {

    StopTimer();

    m_videoStreamIndex = -1;

    if (m_pCodecContext) {
        avcodec_free_context(&m_pCodecContext);
    }

    if (m_pFormatContext) {
        avformat_close_input(&m_pFormatContext);
    }
}

} // namespace slimenano::ui
