#include "../../pch.h" // IWYU pragma: keep

#include "../IDs.h"
#include "../../app/Events.h"
#include "../../media/ffmpeg/FFmpegException.h"
#include "VideoPanel.h"

namespace slimenano::ui {

wxDEFINE_EVENT(EVT_DECODE_FRAME_READY, FrameReadyEvent);

class FrameReadyEvent : public wxCommandEvent {
public:
    FrameReadyEvent(const wxImage& img) :
        wxCommandEvent(EVT_DECODE_FRAME_READY, wxID_ANY) {
        m_image = img.Copy();
    }

    wxImage m_image;

    // 必须实现 Clone()
    virtual wxEvent* Clone() const override {
        return new FrameReadyEvent(*this);
    }
};

void ShowFFmpegError(const wxString& title, const media::FFmpegException& ex) {

    wxString errMsg = wxString::FromUTF8(ex.Message());

    wxMessageBox(
        wxString::Format(_("Open video failed: %d (%s)"), ex.ErrorCode(), errMsg),
        title,
        wxOK | wxICON_ERROR
    );
}

VideoPanel::VideoPanel(wxFrame* pParentFrame) :
    wxPanel(pParentFrame, ID_PANEL_VIDEO_PANEL),
    m_pParentFrame(pParentFrame) {

    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(EVT_OPEN_VIDEO, &VideoPanel::OnOpenVideo, this);
    Bind(wxEVT_PAINT, &VideoPanel::OnPaint, this);
    Bind(wxEVT_SIZE, &VideoPanel::OnSize, this);
    Bind(EVT_DECODE_FRAME_READY, &VideoPanel::OnFrameReady, this);
}

void VideoPanel::OnSize([[maybe_unused]] wxSizeEvent& event) {

    m_videoRenderSize.store({GetClientSize().GetWidth(), GetClientSize().GetHeight()});
}

void VideoPanel::OnFrameReady(FrameReadyEvent& event) {
    m_bitmap = wxBitmap(event.m_image);
    Refresh(false);
}

void VideoPanel::OnPaint([[maybe_unused]] wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    const auto Wt = GetClientSize().GetWidth();
    const auto Ht = GetClientSize().GetHeight();

    dc.SetBackground(wxBrush(*wxBLACK));
    dc.Clear();

    if (m_bitmap.IsOk()) {
        const int OffsetX = (Wt - m_bitmap.GetWidth()) / 2;
        const int OffsetY = (Ht - m_bitmap.GetHeight()) / 2;
        dc.DrawBitmap(m_bitmap, OffsetX, OffsetY, false);
    }

    m_overlay = m_baseInfo + wxString::Format("%d\n", m_fps.load());

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
    try {

        m_pDemuxer = std::make_unique<media::FFmpegDemuxer>(path);

        auto videoDecoder = m_pDemuxer->GetActivityVideoDecoder();
        if (!videoDecoder.has_value()) {
            wxMessageBox(
                wxString::Format(_("Open video failed: Not a video files")),
                "Open",
                wxOK | wxICON_ERROR
            );
            CloseVideo();
            return;
        }

        auto& pCodecContext = videoDecoder->get().GetCodecContext();

        videoDecoder->get().SetFrameCallback(std::bind(&VideoPanel::ConvertToBitmap, this, std::placeholders::_1, std::placeholders::_2));

        auto fps = av_q2d(videoDecoder->get().avg_frame_rate);
        if (fps <= 1e-3) {
            fps = 30.0;
        }

        m_baseInfo = wxString::Format(
            "File: %s\n"
            "Resolution %d x %d\n"
            "Fps: ",
            path,
            pCodecContext.width,
            pCodecContext.height
        );

        frames = 0;
        m_startTime = std::chrono::steady_clock::now();

        m_pDemuxer->Play();
    } catch (media::FFmpegException ex) {
        ShowFFmpegError("Error on load file", ex);
        CloseVideo();
    }
}

void VideoPanel::CloseVideo() {

    m_pDemuxer = nullptr;

    if (m_pSwsContext) {
        sws_freeContext(m_pSwsContext);
        m_pSwsContext = nullptr;
    }
}

void VideoPanel::ConvertToBitmap(media::FFmpegDecoder& decoder, media::AVFramePtr frame) {

    auto videoRenderSize = m_videoRenderSize.load();

    if (videoRenderSize.width == 0 || videoRenderSize.height == 0) {
        return;
    }

    if (!m_pSwsContext) {
        m_pSwsContext = sws_getContext(
            decoder.GetCodecContext().width,
            decoder.GetCodecContext().height,
            decoder.GetCodecContext().pix_fmt,
            videoRenderSize.width,
            videoRenderSize.height,
            AV_PIX_FMT_RGB24,
            SWS_FAST_BILINEAR,
            nullptr,
            nullptr,
            nullptr
        );
    }

    wxImage img(videoRenderSize.width, videoRenderSize.height);
    uint8_t* dest = img.GetData(); // RGB24 buffer

    uint8_t* dstSlice[1] = {dest};
    int dstStride[1] = {img.GetWidth() * 3};

    sws_scale(
        m_pSwsContext,
        frame->data,
        frame->linesize,
        0,
        decoder.GetCodecContext().height,
        dstSlice,
        dstStride
    );

    ++frames;

    auto now = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration<double>(now - m_startTime).count();
    if (seconds > 0.0) {
        m_fps = static_cast<int>(frames / seconds);
    }

    FrameReadyEvent event(img);

    wxPostEvent(this, event);
}

} // namespace slimenano::ui
