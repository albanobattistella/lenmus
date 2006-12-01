//--------------------------------------------------------------------------------------
//    LenMus project: free software for music theory and language
//    Copyright (c) 2002-2006 Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the 
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 2 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this 
//    program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, 
//    Fifth Floor, Boston, MA  02110-1301, USA.
//
//    For any comment, suggestion or feature request, please contact the manager of 
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __LT_MAINFRAME_H__        //to avoid nested includes
#define __LT_MAINFRAME_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


class ltMainFrame : public wxFrame
{
public:
    // ctor(s)
    ltMainFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnInstaller(wxCommandEvent& event);
    void OnSplitFile(wxCommandEvent& WXUNUSED(event));
    void OnConvertToHtml(wxCommandEvent& WXUNUSED(event));
    void OnGeneratePO(wxCommandEvent& WXUNUSED(event));
    void OnCompileBook(wxCommandEvent& WXUNUSED(event));
    void OnMergePO(wxCommandEvent& WXUNUSED(event));

private:
    void PutContentIntoFile(wxString sPath, wxString sContent);
    void GenerateLanguage(int i);

    DECLARE_EVENT_TABLE()
};

    
#endif    // __LT_MAINFRAME_H__