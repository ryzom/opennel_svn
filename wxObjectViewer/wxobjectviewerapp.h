/////////////////////////////////////////////////////////////////////////////
// Name:        wxobjectviewerapp.h
// Purpose:     
// Author:      Matt Raykowski
// Modified by: 
// Created:     Wed 14 Nov 2007 11:04:08 CST
// RCS-ID:      
// Copyright:   Copyright, 2007 Matt Raykowski
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXOBJECTVIEWERAPP_H_
#define _WXOBJECTVIEWERAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "frmobjectviewer.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * WxObjectViewerApp class declaration
 */

class WxObjectViewerApp: public wxApp
{    
    DECLARE_CLASS( WxObjectViewerApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    WxObjectViewerApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin WxObjectViewerApp event handler declarations

////@end WxObjectViewerApp event handler declarations

////@begin WxObjectViewerApp member function declarations

////@end WxObjectViewerApp member function declarations

////@begin WxObjectViewerApp member variables
////@end WxObjectViewerApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(WxObjectViewerApp)
////@end declare app

#endif
    // _WXOBJECTVIEWERAPP_H_
