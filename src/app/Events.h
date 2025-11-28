
#ifndef EASY_CUTTER_SRC_APP_EVENTS_H
#define EASY_CUTTER_SRC_APP_EVENTS_H

#include <wx/event.h>

namespace slimenano {

wxDECLARE_EVENT(EVT_OPEN_VIDEO, wxCommandEvent);
wxDECLARE_EVENT(EVT_PLAY_VIDEO, wxCommandEvent);
wxDECLARE_EVENT(EVT_STOP_VIDEO, wxCommandEvent);

} // namespace slimenano

#endif
