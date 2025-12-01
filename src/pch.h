#ifndef EASY_CUTTER_SRC_PCH_H
#define EASY_CUTTER_SRC_PCH_H

#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <wx/defs.h>
#include <wx/event.h>
#include <wx/translation.h>
#include <wx/timer.h>
#include <wx/dcbuffer.h>

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include <wx/menu.h>
#include <wx/menuitem.h>

#include <wx/msgdlg.h>
#include <wx/filedlg.h>

#endif
