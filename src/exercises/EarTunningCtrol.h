//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2007 Cecilio Salmeron
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

#ifndef __EARTUNNINGCTROL_H__        //to avoid nested includes
#define __EARTUNNINGCTROL_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "EarTunningCtrol.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "EarIntvalConstrains.h"
#include "../score/Score.h"
#include "ScoreAuxCtrol.h"
#include "UrlAuxCtrol.h"
#include "CountersCtrol.h"

//--------------------------------------------------------------------------------
// class for the exercise of un-tunned pitches
//--------------------------------------------------------------------------------

class lmEarTunningCtrol : public lmCompareMidiCtrol    
{
   DECLARE_DYNAMIC_CLASS(lmEarTunningCtrol)

public:

    // constructor and destructor    
    lmEarTunningCtrol(wxWindow* parent, wxWindowID id,
               lmEarIntervalsConstrains* pConstrains, 
               const wxPoint& pos = wxDefaultPosition, 
               const wxSize& size = wxDefaultSize, int style = 0);

    ~lmEarTunningCtrol();

    // event handlers
    void OnSize(wxSizeEvent& event);
    void OnRespButton(wxCommandEvent& event);
    void OnPlay(wxCommandEvent& event);
    void OnNewProblem(wxCommandEvent& event);
    void OnResetCounters(wxCommandEvent& event);
    void OnDisplaySolution(wxCommandEvent& event);
    void OnSettingsButton(wxCommandEvent& event);

    // event handlers related to debugging
    void OnDebugShowSourceScore(wxCommandEvent& event);
    void OnDebugDumpScore(wxCommandEvent& event);
    void OnDebugShowMidiEvents(wxCommandEvent& event);

    // event handlers related with playing a score
    void OnEndOfPlay(lmEndOfPlayEvent& WXUNUSED(event));
    void OnTimerEvent(wxTimerEvent& WXUNUSED(event));


private:
    void EnableButtons(bool fEnable);
    void Play();
    void PlayInterval(int nIntv);
    void NewProblem();
    void DisplaySolution();
    void ResetExercise();
    void ResetCounters();
    void DoStopSounds();

        // member variables


    DECLARE_EVENT_TABLE()
};



#endif  // __EARTUNNINGCTROL_H__