//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2009 LenMus project
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program. If not, see <http://www.gnu.org/licenses/>.
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __LM_TIMESIGNATURE_H__        //to avoid nested includes
#define __LM_TIMESIGNATURE_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "TimeSignature.cpp"
#endif

class lmContext;

//------------------------------------------------------------------------------------------------
// lmTimeSignature object
//------------------------------------------------------------------------------------------------

class lmTimeSignature: public lmStaffObj
{
public:
    //constructors and destructor

        //constructor for types eTS_Common, eTS_Cut and eTS_SenzaMisura
    lmTimeSignature(lmETimeSignatureType nType, lmVStaff* pVStaff, long nID,
                    bool fVisible = true);
        //constructor for type eTS_SingleNumber
    lmTimeSignature(int nSingleNumber, lmVStaff* pVStaff, long nID, bool fVisible = true);
        //constructors for type eTS_Normal
    lmTimeSignature(int nBeats, int nBeatType, lmVStaff* pVStaff, long nID,
                    bool fVisible = true);
    lmTimeSignature(lmETimeSignature nTimeSign, lmVStaff* pVStaff, long nID,
                    bool fVisible = true);
        //constructor for type eTS_Composite
    lmTimeSignature(int nNumBeats, int nBeats[], int nBeatType, lmVStaff* pVStaff,
                    long nID, bool fVisible = true);
        //constructor for type eTS_Multiple
    lmTimeSignature(int nNumFractions, int nBeats[], int nBeatType[], lmVStaff* pVStaff,
                    long nID, bool fVisible = true);

    ~lmTimeSignature() {}

    //implementation of virtual methods defined in abstract base class lmStaffObj
    lmLUnits LayoutObject(lmBox* pBox, lmPaper* pPaper, lmUPoint uPos, wxColour colorC);
	wxString GetName() const { return _T("time signature"); }
    void StoreOriginAndShiftShapes(lmLUnits uxShift, int nShapeIdx=0);

    //    debugging
    wxString Dump();
    wxString SourceLDP(int nIndent, bool fUndoData);
    wxString SourceXML(int nIndent);

    //sound related methods
    void AddMidiEvent(lmSoundManager* pSM, float rMeasureStartTime, int nMeasure);
    int GetNumPulses();

    // access to information
    int GetNumBeats() { return m_nBeats; }
    int GetBeatType() { return m_nBeatType; }
    float GetMeasureDuration();

	//context management
    inline void SetContext(int nStaff, lmContext* pContext)
                            { wxASSERT(nStaff > 0); m_pContext[nStaff-1] = pContext; }
    inline lmContext* GetContext(int nStaff) 
                        { wxASSERT(nStaff > 0); return m_pContext[nStaff-1]; }
    void RemoveCreatedContexts();


private:
    void Create();
    lmShape* CreateShape(int nShapeIdx, lmBox* pBox, lmPaper* pPaper, wxColour colorC,
                         wxString& sTopGlyphs, lmLUnits uxPosTop, lmLUnits uyPosTop,
                         wxString& sBottomGlyphs,
                         lmLUnits uxPosBottom, lmLUnits uyPosBottom);

    // member variables

    lmContext*			m_pContext[lmMAX_STAFF];    //ptr to current context for each staff

	// attributes
    int				m_nBeats;
    int				m_nBeatType;
    int				m_nNumFractions;    // for type eTS_Multiple and eTS_Normal

    lmETimeSignatureType    m_nType;	//type of time signature
        //eTS_Normal = 1,       // it is a single fraction
        //eTS_Common,           // it is 4/4 but represented by a C symbol
        //eTS_Cut,              // it is 2/4 but represented by a C/ simbol
        //sTS_SingleNumber,     // it is a single number with an implied denominator
        //eTS_Multiple,         // multiple fractions, i.e.: 2/4 + 3/8
        //eTS_Composite,        // composite fraction, i.e.: 3+2/8
        //eTS_SenzaMisura       // no time signature is present


};

//
// global functions related to TimeSignatures, and some definitions used by them
//
#define lmNOT_ON_BEAT    -1

extern int lmGetNumPulsesForTimeSignature(lmETimeSignature nTimeSign);
extern int GetBeatTypeFromTimeSignType(lmETimeSignature nTimeSign);
extern int lmGetNumBeatsFromTimeSignType(lmETimeSignature nTimeSign);
extern float GetBeatDuration(lmETimeSignature nTimeSign);
extern float GetBeatDuration(int nBeatType);
extern float GetMeasureDuration(lmETimeSignature nTimeSign);
//extern bool lmIsCompoundMeter(lmETimeSignature nTimeSign);
extern int AssignVolume(float rTimePos, int nBeats, int nBeatType);
extern int GetNoteBeatPosition(float rTimePos, int nBeats, int nBeatType);
extern int GetBeatPosition(float rTimePos, float rDuration, int nBeats, int nBeatType);

#endif    // __LM_TIMESIGNATURE_H__

