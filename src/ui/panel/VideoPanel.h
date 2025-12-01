#ifndef EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H
#define EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H

#include <wx/event.h>
#include <wx/frame.h>
#include <wx/panel.h>

extern "C" {
struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;
}

namespace slimenano::ui {

class VideoPanel : public wxPanel {
public:
    VideoPanel(wxFrame* pParentFrame);

    void OnOpenVideo([[maybe_unused]] const wxCommandEvent& event);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);

private:
    void SetOverlay(const wxString& text);
    void StartTimer(int intervalMs);
    void StopTimer();
    void CloseVideo();
    bool DecodeNextFrame();
    void ConvertToBitmap(AVFrame* frame);

    wxFrame* m_pParentFrame;
    wxBitmap m_bitmap;
    wxString m_overlay = "Waiting video";
    wxTimer m_timer;

    AVFormatContext* m_pFormatContext = nullptr;
    AVCodecContext* m_pCodecContext = nullptr;
    int m_videoStreamIndex = -1;
    double m_fps = 0;
    double m_pts = 0;
    double m_timeBase = 0;
    double m_startClock = 0;
    AVPacket* m_pPacket = nullptr;
    AVFrame* m_pFrame = nullptr;
    SwsContext* m_pSwsContext = nullptr;
};

} // namespace slimenano::ui

#endif
