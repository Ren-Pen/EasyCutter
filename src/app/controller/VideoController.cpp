#include <string_view>

#include "../Events.h"
#include "VideoController.h"

void OpenVideo([[maybe_unused]] std::string_view path) {
}

namespace slimenano {

VideoController::VideoController(ui::VideoPanel* pVideoPanel) :
    m_pVideoPanel(pVideoPanel) {
    Bind(EVT_OPEN_VIDEO, &VideoController::OnOpenVideo, this);
    Bind(EVT_PLAY_VIDEO, &VideoController::OnPlayVideo, this);
    Bind(EVT_STOP_VIDEO, &VideoController::OnStopVideo, this);
}

void VideoController::OnOpenVideo(wxCommandEvent& event) {
    auto path = event.GetString();
    OpenVideo(path.ToStdString());
}
void VideoController::OnPlayVideo([[maybe_unused]] wxCommandEvent& event) {
}
void VideoController::OnStopVideo([[maybe_unused]] wxCommandEvent& event) {
}

} // namespace slimenano
