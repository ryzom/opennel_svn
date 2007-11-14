/////////////////////////////////////////////////////////////////////////////
// Name:        wxobjectviewerapp.cpp
// Purpose:     
// Author:      Matt Raykowski
// Modified by: 
// Created:     Wed 14 Nov 2007 11:04:08 CST
// RCS-ID:      
// Copyright:   Copyright, 2007 Matt Raykowski
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wxobjectviewerapp.h"

////@begin XPM images
////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( WxObjectViewerApp )
////@end implement app


/*!
 * WxObjectViewerApp type definition
 */

IMPLEMENT_CLASS( WxObjectViewerApp, wxApp )


/*!
 * WxObjectViewerApp event table definition
 */

BEGIN_EVENT_TABLE( WxObjectViewerApp, wxApp )

////@begin WxObjectViewerApp event table entries
////@end WxObjectViewerApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for WxObjectViewerApp
 */

WxObjectViewerApp::WxObjectViewerApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void WxObjectViewerApp::Init()
{
////@begin WxObjectViewerApp member initialisation
////@end WxObjectViewerApp member initialisation
}

/*!
 * Initialisation for WxObjectViewerApp
 */

bool WxObjectViewerApp::OnInit()
{    
////@begin WxObjectViewerApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	frmObjectViewer* mainWindow = new frmObjectViewer( NULL, ID_FRMOBJECTVIEWER );
	mainWindow->Show(true);
////@end WxObjectViewerApp initialisation

    return true;
}


/*!
 * Cleanup for WxObjectViewerApp
 */

int WxObjectViewerApp::OnExit()
{    
////@begin WxObjectViewerApp cleanup
	return wxApp::OnExit();
////@end WxObjectViewerApp cleanup
}

