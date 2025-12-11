#ifndef EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H
#define EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H

#include <atomic>

#include <wx/event.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include "../../media/ffmpeg/decode/FFmpegDemuxer.h"

extern "C" {
struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;
}

namespace slimenano::ui {
class FrameReadyEvent;
struct VideoRenderSize {
    int width;
    int height;
};

class VideoPanel : public wxPanel {
public:
    VideoPanel(wxFrame* pParentFrame);

    void OnOpenVideo([[maybe_unused]] const wxCommandEvent& event);

protected:
    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnFrameReady(FrameReadyEvent& event);

private:
    void SetOverlay(const wxString& text);
    void CloseVideo();
    void ConvertToBitmap(media::FFmpegDecoder& decoder, media::AVFramePtr frame);

    std::unique_ptr<media::FFmpegDemuxer> m_pDemuxer = nullptr;

    wxFrame* m_pParentFrame;
    wxBitmap m_bitmap;
    wxString m_overlay = "Waiting video";

    std::atomic<VideoRenderSize> m_videoRenderSize;

    SwsContext* m_pSwsContext = nullptr;
};

} // namespace slimenano::ui

#endif
