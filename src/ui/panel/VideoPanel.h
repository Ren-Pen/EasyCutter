#ifndef EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H
#define EASY_CUTTER_SRC_UI_PANEL_VIDEO_PANEL_H

#include <wx/event.h>
#include <wx/frame.h>
#include <wx/panel.h>

extern "C" {
struct AVCodecContext;
struct AVFormatContext;
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
    void SetFrame(const wxBitmap& bmp);
    void StartTimer(int intervalMs);
    void StopTimer();
    void CloseVideo();

    wxFrame* m_pParentFrame;
    wxBitmap m_frame;
    wxString m_overlay = "Waiting video";
    wxTimer m_timer;

    AVFormatContext* m_pFormatContext = nullptr;
    AVCodecContext* m_pCodecContext = nullptr;
    int m_videoStreamIndex = -1;
};

} // namespace slimenano::ui

#endif
