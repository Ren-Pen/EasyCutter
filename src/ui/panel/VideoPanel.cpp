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
    Refresh();
    Update();
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

        m_pSwsContext = sws_getContext(
            pCodecContext.width,
            pCodecContext.height,
            pCodecContext.pix_fmt,
            pCodecContext.width,
            pCodecContext.height,
            AV_PIX_FMT_RGB24,
            SWS_LANCZOS,
            nullptr,
            nullptr,
            nullptr
        );

        videoDecoder->get().SetFrameCallback(std::bind(&VideoPanel::ConvertToBitmap, this, std::placeholders::_1, std::placeholders::_2));

        auto fps = av_q2d(videoDecoder->get().avg_frame_rate);
        if (fps <= 1e-3) {
            fps = 30.0;
        }

        SetOverlay(
            wxString::Format(
                "File: %s\n"
                "Resolution %d x %d\n"
                "Fps: %f\n",
                path,
                pCodecContext.width,
                pCodecContext.height,
                fps
            )
        );

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

    int Wt = videoRenderSize.width;
    int Ht = videoRenderSize.height;

    if (Wt == 0 || Ht == 0) {
        return;
    }

    int w = decoder.GetCodecContext().width;
    int h = decoder.GetCodecContext().height;

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

    const auto Ws = img.GetWidth();
    const auto Hs = img.GetHeight();

    double Sw = (double)Wt / Ws;
    double Sh = (double)Ht / Hs;
    double S = std::min(Sw, Sh);
    if (std::abs(S - 1) > 1e-6) {
        const int Wr = static_cast<int>(Ws * S);
        const int Hr = static_cast<int>(Hs * S);
        img = img.Scale(Wr, Hr, wxIMAGE_QUALITY_HIGH);
    }

    FrameReadyEvent event(img);

    wxPostEvent(this, event);
}

} // namespace slimenano::ui
