#ifndef WX_WXHEAD_H
#define WX_WXHEAD_H

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <cstdint>

#include <wx/wx.h>

// The following are not pulled in by wx.h

// for some reason, mingw32 wx.h doesn't pull in listctrl by default
#include <wx/config.h>
#include <wx/display.h>
#include <wx/fileconf.h>
#include <wx/listctrl.h>
#include <wx/stdpaths.h>
#include <wx/treectrl.h>
#include <wx/xrc/xmlres.h>
// filehistory.h is separate only in 2.9+
#include <wx/docview.h>

// This is necessary to build with gcc on Fedora.
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::int8_t;
using std::int16_t;
using std::int32_t;

#ifndef NO_OGL
// glcanvas must be included before SFML for MacOSX
// originally, this was confined to drawing.h.
#include <wx/glcanvas.h>
// besides that, other crap gets #defined
#ifdef Status
#undef Status
#endif
#ifdef BadRequest
#undef BadRequest
#endif
#endif

// compatibility with wx-2.9
// The only reason I use wxTRANSLATE at all is to get wxT as a side effect.
#if wxCHECK_VERSION(2, 9, 0)
#undef wxTRANSLATE
#define wxTRANSLATE wxT
#endif

// wxGTK (2.8.8+, at least) doesn't store the actual menu item text in m_text.
// This breaks GetText, SetText, GetAccel, SetAccel, and GetLabel;
// GetItemLabel() works, though.
// GetText, SetText, and GetLabel are deprecated, so that's not a problem
// GetAccel is inefficent anyway (often I don't want to convert to wxAccEnt)
// This is a working replacement for SetAccel, at least.

#include "wxutil.h"

// wxrc helpers (for dynamic strings instead of constant)
#define XRCID_D(str) wxXmlResource::GetXRCID(str)
//#define XRCCTRL_D(win, id, type) (wxStaticCast((win).FindWindow(XRCID_D(id)), type))
//#define XRCCTRL_I(win, id, type) (wxStaticCast((win).FindWindow(id), type))
// XRCCTRL is broken.
// In debug mode, it uses wxDynamicCast, which numerous wx classes fail on
// due to not correctly specifying parents in CLASS() declarations
// In standard mode, it does a static cast, which is unsafe for user input
// So instead I'll always do a (slow, possibly unportable) dynamic_cast().
// If your compiler doesn't support rtti, there are other pieces of code where
// I bypassed wx's stuff to use real dynamic_cast as well, so get a better
// compiler.
#undef XRCCTRL
#define XRCCTRL_I(win, id, type) (dynamic_cast<type*>((win).FindWindow(id)))
#define XRCCTRL(win, id, type) XRCCTRL_I(win, XRCID(id), type)
#define XRCCTRL_D(win, id, type) XRCCTRL_I(win, XRCID_D(id), type)

// Keep a single entry point for converting wxString to UTF8.
// Use this function whenever we want to get
static inline const wxCharBuffer UTF8(wxString str)
{
    return str.mb_str(wxConvUTF8);
}

#endif /* WX_WXHEAD_H */
