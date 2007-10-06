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

#ifndef __LM_BOXINSTRSLICE_H__        //to avoid nested includes
#define __LM_BOXINSTRSLICE_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "BoxInstrSlice.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "vector"

#include "../score/Score.h"
#include "BoxSystem.h"

//
// Class lmBoxInstrSlice represents a part of the VStaff of an instrument. 
//

class lmBoxVStaffSlice;

class lmBoxInstrSlice
{
public:
    lmBoxInstrSlice(lmBoxSystem* pParent, int nFirstMeasure, int nLastMeasure,
                    lmInstrument* pInstr, int nInstr);
    ~lmBoxInstrSlice();

    lmLUnits Render(lmPaper* pPaper, int xStartPos, int nNumPage, int nSystem);

    inline lmBoxSystem* GetBoxSystem() const { return m_pSystem; }
    inline int GetNumInstrument() const { return m_nNumInstr; } 
    inline lmLUnits GetXRight() const { return m_xRight; }
    inline lmInstrument* GetInstrument() const { return m_pInstr; }

private:
    void RenderMeasure(int nMeasure, lmPaper* pPaper);

    lmBoxSystem*    m_pSystem;          //parent system
    int             m_nFirstMeasure;    //number of first measure in this slice
    int             m_nLastMeasure;     //number of last measure in this slice
    lmInstrument*   m_pInstr;           //instrument to which this slice belongs
    int             m_nNumInstr;        //instrument number (1..n)

    std::vector<lmBoxVStaffSlice*> m_VStaffSlices;   //collection of VStaff slices

    //position of the staves of this instrument
    lmLUnits        m_xRight;
    lmLUnits        m_xLeft;
    lmLUnits        m_yTop;
    lmLUnits        m_yBottom;

};



#endif  // __LM_BOXINSTRSLICE_H__
