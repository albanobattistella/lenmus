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

#ifdef __GNUG__
#pragma implementation "KeySignature.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "Score.h"

//access to error's logger
#include "../app/Logger.h"
extern lmLogger* g_pLogger;



static wxString m_sKeySignatureName[30];
static wxString m_sLDPKeyName[30] = {
        _T("Do"), _T("Sol"), _T("Re"), _T("La"), _T("Mi"), _T("Si"),
        _T("Fas"), _T("Dos"), _T("Dob"), _T("Solb"), _T("Reb"), _T("Lab"),
        _T("Mib"), _T("Sib"), _T("Fa"), _T("Lam"), _T("Mim"), _T("Sim"),
        _T("Fasm"), _T("Dosm"), _T("Solsm"), _T("Resm"), _T("Lasm"), _T("Labm"),
        _T("Mibm"), _T("Sibm"), _T("Fam"), _T("Dom"), _T("Solm"), _T("Rem")
};

//-------------------------------------------------------------------------------------------------
// lmKeySignature object implementation
//-------------------------------------------------------------------------------------------------

//
//constructors and destructor
//

//constructor for traditional key signatures
lmKeySignature::lmKeySignature(int nFifths, bool fMajor, lmVStaff* pVStaff, bool fVisible) :
    lmStaffObj(pVStaff, eSFOT_KeySignature, pVStaff, 1, fVisible, lmDRAGGABLE)
{

    //language dependent strings. Can not be statically initiallized because
    //then they do not get translated
    m_sKeySignatureName[0] = _("C Major");
    m_sKeySignatureName[1] = _("G Major");
    m_sKeySignatureName[2] = _("D Major");
    m_sKeySignatureName[3] = _("A Major");
    m_sKeySignatureName[4] = _("E Major");
    m_sKeySignatureName[5] = _("B Major");
    m_sKeySignatureName[6] = _("F # Major");
    m_sKeySignatureName[7] = _("C # Major");
    m_sKeySignatureName[8] = _("C b Major");
    m_sKeySignatureName[9] = _("G b Major");
    m_sKeySignatureName[10] = _("D b Major");
    m_sKeySignatureName[11] = _("A b Major");
    m_sKeySignatureName[12] = _("E b Major");
    m_sKeySignatureName[13] = _("B b Major");
    m_sKeySignatureName[14] = _("F Major");
    m_sKeySignatureName[15] = _("A minor");
    m_sKeySignatureName[16] = _("E minor");
    m_sKeySignatureName[17] = _("B minor");
    m_sKeySignatureName[18] = _("F # minor");
    m_sKeySignatureName[19] = _("C # minor");
    m_sKeySignatureName[20] = _("G # minor");
    m_sKeySignatureName[21] = _("D # minor");
    m_sKeySignatureName[22] = _("A # minor");
    m_sKeySignatureName[23] = _("A b minor");
    m_sKeySignatureName[24] = _("E b minor");
    m_sKeySignatureName[25] = _("B b minor");
    m_sKeySignatureName[26] = _("F minor");
    m_sKeySignatureName[27] = _("C minor");
    m_sKeySignatureName[28] = _("G minor");
    m_sKeySignatureName[29] = _("D minor");

    m_fHidden = false;
    m_fTraditional = true;
    m_nFifths = nFifths;
    m_fMajor = fMajor;

    SetKeySignatureType();

    g_pLogger->LogTrace(_T("lmKeySignature"),
        _T("[lmKeySignature::lmKeySignature] m_nFifths=%d, m_fMajor=%s, nKey=%d"),
            m_nFifths, (m_fMajor ? _T("yes") : _T("no")), m_nKeySignature );

}

wxString lmKeySignature::Dump()
{
    wxString sDump = wxString::Format(
        _T("%d\tKey Sign. %d %s Key=%d\tTimePos=%.2f\n"),
        m_nId, m_nFifths, (m_fMajor ? _T("major") : _T("minor")), m_nKeySignature, m_rTimePos );
    return sDump;
}

wxString lmKeySignature::SourceLDP()
{
    wxString sSource = _T("         (key ");
    sSource += m_sLDPKeyName[m_nKeySignature];
    if (!m_fVisible) { sSource += _T(" no_visible"); }
    sSource += _T(")\n");
    return sSource;

}

wxString lmKeySignature::SourceXML()
{
//    IPentObj_FuenteXML = sEmpty
    return _T("");
}

void lmKeySignature::DrawObject(bool fMeasuring, lmPaper* pPaper, wxColour colorC,
                                bool fHighlight)
{
    if (m_fHidden) return;

    if (fMeasuring) {
        // get the shift to the staff on which the key signature must be drawn
        lmLUnits yShift = m_pVStaff->GetStaffOffset(m_nStaffNum);

        // store glyph position
        m_uGlyphPos.x = 0;
        m_uGlyphPos.y = yShift;
    }

    DrawKeySignature(fMeasuring, pPaper,
            (m_fSelected ? g_pColors->ScoreSelected() : g_pColors->ScoreNormal() ));

}

// returns the width of the draw (logical units)
lmLUnits lmKeySignature::DrawKeySignature(bool fMeasuring, lmPaper* pPaper, wxColour colorC)
{
#if !defined(__WXGTK__)
//TODO: Linux-dbg: In linux SetTextForegroung fails !!!
    pPaper->SetFont(*m_pFont);
    pPaper->SetTextForeground(colorC);
#endif

    //Key signature is common to all lmVStaff staves, but it is only present, as lmStaffObj, in
    //the first staff. Therefore, for renderization, it is necessary to repeat for
    //each staff
    lmLUnits yOffset = 0;
    lmStaff* pStaff = m_pVStaff->GetFirstStaff();
    for (int nStaff=1; pStaff; pStaff = m_pVStaff->GetNextStaff(), nStaff++) {
        //get current clef
        lmClef* pClef = pStaff->GetCurrentClef();
        EClefType nClef = pClef->GetClefType();

        // Draw the key signature
        lmUPoint uPos = lmUPoint(m_uPaperPos.x, m_uPaperPos.y + yOffset);
        DrawAt(fMeasuring, pPaper, uPos, nClef, nStaff);

        if (nStaff==1 && fMeasuring) {
            //@attention DrawAt() has updated m_uWidth and the after space is included
            // store selection rectangle measures and position (relative to m_uPaperPos)
            m_uSelRect.width = m_uWidth - m_pVStaff->TenthsToLogical(10, nStaff); // substrac after space;
            m_uSelRect.height = m_pVStaff->TenthsToLogical( 40, nStaff );
            m_uSelRect.x = m_uGlyphPos.x;
            m_uSelRect.y = m_uGlyphPos.y + m_pVStaff->TenthsToLogical( 50, nStaff );
        }

        //compute vertical displacement for next staff
        yOffset += pStaff->GetHeight();
        yOffset += pStaff->GetAfterSpace();

    }

    return m_uWidth;

}

lmLUnits lmKeySignature::DrawAccidental(bool fMeasuring, lmPaper* pPaper, EAccidentals nAlter,
        lmLUnits uxLeft, lmLUnits uyTop, int nStaff)
{
    //render the accidental nAlter at position uxLeft, uyTop. Returns its width

    wxString sGlyph;
    lmLUnits nTotalWidth = 0;
    lmLUnits nWidth, nHeight;

    lmLUnits yPos = uyTop - m_pVStaff->TenthsToLogical( 10, nStaff );

    switch(nAlter) {
        case eNatural:
            sGlyph = _T("//");
            break;
        case eSharp:
            sGlyph = _T("#");
            break;
        case eFlat:
            sGlyph = _T("%");
            break;
        case eFlatFlat:
            sGlyph = _T("&");
            break;
        case eDoubleSharp:
            sGlyph = _T("$");
            break;
        case eNaturalFlat:
            sGlyph = _T("//");
            if (!fMeasuring) pPaper->DrawText(sGlyph, uxLeft, yPos);
            pPaper->GetTextExtent(sGlyph, &nWidth, &nHeight);
            nTotalWidth = nWidth;
            sGlyph = _T("%");
            break;
        case eNaturalSharp:
            sGlyph = _T("//");
            if (!fMeasuring) pPaper->DrawText(sGlyph, uxLeft, yPos);
            pPaper->GetTextExtent(sGlyph, &nWidth, &nHeight);
            nTotalWidth = nWidth;
            sGlyph = _T("#");
            break;
        default:
            wxASSERT(false);
    }

    if (!fMeasuring) {
        pPaper->DrawText(sGlyph, uxLeft, yPos);
    }
    pPaper->GetTextExtent(sGlyph, &nWidth, &nHeight);
    nTotalWidth += nWidth;

    return nTotalWidth;

}

lmLUnits lmKeySignature::DrawAt(bool fMeasuring, lmPaper* pPaper, lmUPoint uPos,
                               EClefType nClef, int nStaff, wxColour colorC)
{
    /*
    For rendering the prolog it is necessary to have the rendering code in a common
    method so that it can be shared between DrawProlog() an DrawObject() methods.
    This method returns the width of the draw
    */

    lmLUnits nSharpPos[8];        //orden de aparición de los sostenidos
    lmLUnits nFlatPos[8];        //orden de aparición de los bemoles
    lmLUnits nOneLine;            //space, in microns, for half line

    nOneLine = m_pVStaff->TenthsToLogical( 10, nStaff );
    EKeySignatures nKeySignature = m_nKeySignature;

    //Compute position of sharps and flats. Depends on the clef
    lmLUnits yPos = uPos.y;
    switch(nClef) {
        case eclvSol:
            nSharpPos[1] = yPos - 5 * nOneLine;            //line 5 (Fa)
            nSharpPos[2] = yPos - 3.5 * nOneLine;        //space between lines 3 y 4 (Do)
            nSharpPos[3] = yPos - 5.5 * nOneLine;        //space above line 5 (Sol)
            nSharpPos[4] = yPos - 4 * nOneLine;            //line 4 (Re)
            nSharpPos[5] = yPos - 2.5 * nOneLine;        //space between lines 2 y 3 (La)
            nSharpPos[6] = yPos - 4.5 * nOneLine;        //space between lines 4 y 5 (Mi)
            nSharpPos[7] = yPos - 3 * nOneLine;            //line 3 (Si)

            nFlatPos[1] = yPos - 3 * nOneLine;            //line 3 (Si)
            nFlatPos[2] = yPos - 4.5 * nOneLine;        //space between lines 4 y 5 (Mi)
            nFlatPos[3] = yPos - 2.5 * nOneLine;        //space between lines 2 y 3 (La)
            nFlatPos[4] = yPos - 4 * nOneLine;            //line 4 (Re)
            nFlatPos[5] = yPos - 2 * nOneLine;            //line 2 (Sol)
            nFlatPos[6] = yPos - 3.5 * nOneLine;        //space between lines 3 y 4 (Do)
            nFlatPos[7] = yPos - 1.5 * nOneLine;        //space between lines 1 y 2 (Fa)
            break;

        case eclvFa4:
            nSharpPos[1] = yPos - 4 * nOneLine;            //line 4 (Fa)
            nSharpPos[2] = yPos - 2.5 * nOneLine;        //space between lines 2 y 3 (Do)
            nSharpPos[3] = yPos - 4.5 * nOneLine;        //space between lines 4 y 5 (Sol)
            nSharpPos[4] = yPos - 3 * nOneLine;            //line 3 (Re)
            nSharpPos[5] = yPos - 1.5 * nOneLine;        //line 5 (La)
            nSharpPos[6] = yPos - 3.5 * nOneLine;        //space between lines 3 y 4 (Mi)
            nSharpPos[7] = yPos - 2 * nOneLine;            //space aboveline 5 (Si)

            nFlatPos[1] = yPos - 2 * nOneLine;            //line 2 (Si)
            nFlatPos[2] = yPos - 3.5 * nOneLine;        //space between lines 3 y 4 (Mi)
            nFlatPos[3] = yPos - 1.5 * nOneLine;        //space between lines 1 y 2 (La)
            nFlatPos[4] = yPos - 3 * nOneLine;            //line 3 (Re)
            nFlatPos[5] = yPos - nOneLine;                //line 1 (Sol)
            nFlatPos[6] = yPos - 2.5 * nOneLine;        //space between lines 2 y 3 (Do)
            nFlatPos[7] = yPos - 4 * nOneLine;            //linea 4 (Fa)
            break;

        case eclvFa3:
            wxASSERT(false);        //! @todo Clef Fa3
            break;

        case eclvDo1:
            nSharpPos[1] = yPos - 2.5 * nOneLine;        //space between lines 2 y 3 (Fa)
            nSharpPos[2] = yPos - nOneLine;                //line 1 (Do)
            nSharpPos[3] = yPos - 3 * nOneLine;            //line 3 (Sol)
            nSharpPos[4] = yPos - 1.5 * nOneLine;        //space between lines 1 y 2 (Re)
            nSharpPos[5] = yPos - 3.5 * nOneLine;        //space between lines 3 y 4 (La)
            nSharpPos[6] = yPos - 2 * nOneLine;            //line 2 (Mi)
            nSharpPos[7] = yPos - 4 * nOneLine;            //linea 4 (Si)

            nFlatPos[1] = yPos - 4 * nOneLine;            //linea 4 (Si)
            nFlatPos[2] = yPos - 2 * nOneLine;            //line 2 (Mi)
            nFlatPos[3] = yPos - 3.5 * nOneLine;        //space between lines 3 y 4 (La)
            nFlatPos[4] = yPos - 1.5 * nOneLine;        //space between lines 1 y 2 (Re)
            nFlatPos[5] = yPos - 3 * nOneLine;            //line 3 (Sol)
            nFlatPos[6] = yPos - nOneLine;                //line 1 (Do)
            nFlatPos[7] = yPos - 2.5 * nOneLine;        //space between lines 2 y 3 (Fa)
            break;

        case eclvDo2:
            wxASSERT(false);        //! @todo Clef Do2
            break;
        case eclvDo3:
            wxASSERT(false);        //! @todo Clef Do3
            break;
        case eclvDo4:
            wxASSERT(false);        //! @todo Clef Do4
            break;
        case eclvPercussion:
            nKeySignature = earmDo;    //force not to draw any accidentals
            break;
        default:
            wxASSERT(false);
    }

    // Check if it is necessary to draw sharps or flats, and how many.
    int nNumAccidentals = KeySignatureToNumFifths(nKeySignature);
    bool fDrawFlats = (nNumAccidentals < 0);    //true if flats, false if sharps

    g_pLogger->LogTrace(_T("lmKeySignature"),
        _T("[lmKeySignature::DrawAt] nNumAccidentals=%d, fDrawFlats=%s, m_nFifths=%d, m_fMajor=%s, nKey=%d"),
            nNumAccidentals, (fDrawFlats ? _T("yes") : _T("no")),
            m_nFifths, (m_fMajor ? _T("yes") : _T("no")), nKeySignature );

    nNumAccidentals = abs(nNumAccidentals);

    //Render the required flats / sharps
    lmLUnits nWidth=0;  // nHeight=0;
    if (fDrawFlats) {
        for (int i=1; i <= nNumAccidentals; i++) {
            nWidth +=
                DrawAccidental(fMeasuring, pPaper, eFlat, uPos.x+nWidth, nFlatPos[i], nStaff);
        }
    }
    else {
        for (int i=1; i <= nNumAccidentals; i++) {
            nWidth +=
                DrawAccidental(fMeasuring, pPaper, eSharp, uPos.x+nWidth, nSharpPos[i], nStaff);
        }
    }

    // save total width and increment it in one line for after space
    if (fMeasuring) m_uWidth = nWidth + m_pVStaff->TenthsToLogical(10, nStaff);    //one line space

    return m_uWidth;
}


//! @todo Add code for all following methods

wxBitmap* lmKeySignature::GetBitmap(double rScale)
{
    return (wxBitmap*)NULL;
}

void lmKeySignature::MoveDragImage(lmPaper* pPaper, wxDragImage* pDragImage, lmDPoint& ptOffset,
                        const lmUPoint& ptLog, const lmUPoint& uDragStartPos, const lmDPoint& ptPixels)
{
}

lmUPoint lmKeySignature::EndDrag(const lmUPoint& uPos)
{
    return lmUPoint(0,0);
}


void lmKeySignature::SetKeySignatureType()
{
    if (m_fMajor) {
        switch (m_nFifths) {
            case 0:     m_nKeySignature = earmDo;    break;
            case 1:     m_nKeySignature = earmSol;   break;
            case 2:     m_nKeySignature = earmRe;    break;
            case 3:     m_nKeySignature = earmLa;    break;
            case 4:     m_nKeySignature = earmMi;    break;
            case 5:     m_nKeySignature = earmSi;    break;
            case 6:     m_nKeySignature = earmFas;   break;
            case 7:     m_nKeySignature = earmDos;   break;
            case -1:    m_nKeySignature = earmFa;    break;
            case -2:    m_nKeySignature = earmSib;   break;
            case -3:    m_nKeySignature = earmMib;   break;
            case -4:    m_nKeySignature = earmLab;   break;
            case -5:    m_nKeySignature = earmReb;   break;
            case -6:    m_nKeySignature = earmSolb;  break;
            case -7:    m_nKeySignature = earmDob;   break;
            default:
                wxASSERT(false);
        }
    }
    else {
        switch (m_nFifths) {
            case 0:     m_nKeySignature = earmLam;    break;
            case 1:     m_nKeySignature = earmMim;    break;
            case 2:     m_nKeySignature = earmSim;    break;
            case 3:     m_nKeySignature = earmFasm;   break;
            case 4:     m_nKeySignature = earmDosm;   break;
            case 5:     m_nKeySignature = earmSolsm;  break;
            case 6:     m_nKeySignature = earmResm;   break;
            case 7:     m_nKeySignature = earmLasm;   break;
            case -1:    m_nKeySignature = earmRem;    break;
            case -2:    m_nKeySignature = earmSolm;   break;
            case -3:    m_nKeySignature = earmDom;    break;
            case -4:    m_nKeySignature = earmFam;    break;
            case -5:    m_nKeySignature = earmSibm;   break;
            case -6:    m_nKeySignature = earmMibm;   break;
            case -7:    m_nKeySignature = earmLabm;   break;
            default:
                wxASSERT(false);
        }
    }

}


//---------------------------------------------------------------------------------------
// Global methods related to Key signatures
//---------------------------------------------------------------------------------------

void ComputeAccidentals(EKeySignatures nKeySignature, int nAccidentals[])
{
    /*
    Given a key signature (nKeySignature) this function fills the array
    nAccidentals with the accidentals implied by the key signature.
    Each element of the array refers to one note: 0=Do, 1=Re, 2=Mi, 3=Fa, ... , 6=Si
    and its value can be one of:
         0  = no accidental
        -1  = a flat
         1  = a sharp
    */

    // initialize array: no accidentals
    for (int i=0; i < 7; i++) {
        nAccidentals[i] = 0;
    }

    // accidentals implied by the key signature
    switch (nKeySignature) {
        case earmDo:
        case earmLam:
            //no accidentals
            break;

        //Sharps ---------------------------------------
        case earmSol:
        case earmMim:
            nAccidentals[3] = 1;     //Fa #
            break;
        case earmRe:
        case earmSim:
            nAccidentals[3] = 1;     //Fa #
            nAccidentals[0] = 1;     //Do #
            break;
        case earmLa:
        case earmFasm:
            nAccidentals[3] = 1;     //Fa #
            nAccidentals[0] = 1;     //Do #
            nAccidentals[4] = 1;     //Sol #
             break;
       case earmMi:
        case earmDosm:
            nAccidentals[3] = 1;     //Fa #
            nAccidentals[0] = 1;     //Do #
            nAccidentals[4] = 1;     //Sol #
            nAccidentals[1] = 1;     //Re #
            break;
        case earmSi:
        case earmSolsm:
            nAccidentals[3] = 1;     //Fa #
            nAccidentals[0] = 1;     //Do #
            nAccidentals[4] = 1;     //Sol #
            nAccidentals[1] = 1;     //Re #
            nAccidentals[5] = 1;     //La #
            break;
        case earmFas:
        case earmResm:
            nAccidentals[3] = 1;     //Fa #
            nAccidentals[0] = 1;     //Do #
            nAccidentals[4] = 1;     //Sol #
            nAccidentals[1] = 1;     //Re #
            nAccidentals[5] = 1;     //La #
            nAccidentals[2] = 1;     //Mi #
            break;
        case earmDos:
        case earmLasm:
            nAccidentals[3] = 1;     //Fa #
            nAccidentals[0] = 1;     //Do #
            nAccidentals[4] = 1;     //Sol #
            nAccidentals[1] = 1;     //Re #
            nAccidentals[5] = 1;     //La #
            nAccidentals[2] = 1;     //Mi #
            nAccidentals[6] = 1;     //Si #
             break;

        //Flats -------------------------------------------
        case earmFa:
        case earmRem:
            nAccidentals[6] = -1;         //Si b
            break;
        case earmSib:
        case earmSolm:
            nAccidentals[6] = -1;         //Si b
            nAccidentals[2] = -1;         //Mi b
             break;
       case earmMib:
        case earmDom:
            nAccidentals[6] = -1;         //Si b
            nAccidentals[2] = -1;         //Mi b
            nAccidentals[5] = -1;         //La b
            break;
        case earmLab:
        case earmFam:
            nAccidentals[6] = -1;         //Si b
            nAccidentals[2] = -1;         //Mi b
            nAccidentals[5] = -1;         //La b
            nAccidentals[1] = -1;         //Re b
             break;
        case earmReb:
        case earmSibm:
            nAccidentals[6] = -1;         //Si b
            nAccidentals[2] = -1;         //Mi b
            nAccidentals[5] = -1;         //La b
            nAccidentals[1] = -1;         //Re b
            nAccidentals[4] = -1;         //Sol b
            break;
        case earmSolb:
        case earmMibm:
            nAccidentals[6] = -1;         //Si b
            nAccidentals[2] = -1;         //Mi b
            nAccidentals[5] = -1;         //La b
            nAccidentals[1] = -1;         //Re b
            nAccidentals[4] = -1;         //Sol b
            nAccidentals[0] = -1;         //Do b
            break;
        case earmDob:
        case earmLabm:
            nAccidentals[6] = -1;         //Si b
            nAccidentals[2] = -1;         //Mi b
            nAccidentals[5] = -1;         //La b
            nAccidentals[1] = -1;         //Re b
            nAccidentals[4] = -1;         //Sol b
            nAccidentals[0] = -1;         //Do b
            nAccidentals[3] = -1;         //Fa b
            break;
        default:
            wxASSERT(false);
    }

}

int GetRootNoteIndex(EKeySignatures nKeySignature)
{
    //returns the index (0..6, 0=Do, 1=Re, 3=Mi, ... , 6=Si) to the root note for
    //the Key signature. For example, if nKeySignature is La sharp minor it returns
    //index = 5 (La)

    //compute root note
    int nRootNote;
    switch(nKeySignature) {
        case earmDo:
        case earmDom:
        case earmDosm:
        case earmDos:
        case earmDob:
            nRootNote = 0;
            break;
        case earmRe:
        case earmReb:
        case earmResm:
        case earmRem:
            nRootNote = 1;
            break;
        case earmMi:
        case earmMim:
        case earmMib:
        case earmMibm:
            nRootNote = 2;
            break;
        case earmFa:
        case earmFasm:
        case earmFas:
        case earmFam:
            nRootNote = 3;
            break;
        case earmSol:
        case earmSolsm:
        case earmSolm:
        case earmSolb:
            nRootNote = 4;
            break;
        case earmLa:
        case earmLam:
        case earmLasm:
        case earmLab:
        case earmLabm:
            nRootNote = 5;
            break;
        case earmSim:
        case earmSi:
        case earmSib:
        case earmSibm:
            nRootNote = 6;
            break;
        default:
            wxASSERT(false);
    }

    return nRootNote;

}

bool IsMajor(EKeySignatures nKeySignature)
{
    return (nKeySignature < earmLam);
}

const wxString& GetKeySignatureName(EKeySignatures nKeySignature)
{
    return m_sKeySignatureName[nKeySignature - lmMIN_KEY];
}

//! Retunrs the number of fifths that corresponds to the encoded key signature
int KeySignatureToNumFifths(EKeySignatures nKeySignature)
{
    int nFifths = 0;        //num accidentals to draw (0..7)
    switch(nKeySignature) {
        case earmDo:
        case earmLam:
            nFifths = 0;
            break;

        //Sharps ---------------------------------------
        case earmSol:
        case earmMim:
            nFifths = 1;
            break;
        case earmRe:
        case earmSim:
            nFifths = 2;
            break;
        case earmLa:
        case earmFasm:
            nFifths = 3;
            break;
        case earmMi:
        case earmDosm:
            nFifths = 4;
            break;
        case earmSi:
        case earmSolsm:
            nFifths = 5;
            break;
        case earmFas:
        case earmResm:
            nFifths = 6;
            break;
        case earmDos:
        case earmLasm:
            nFifths = 7;
            break;

        //Flats -------------------------------------------
        case earmFa:
        case earmRem:
            nFifths = -1;
            break;
        case earmSib:
        case earmSolm:
            nFifths = -2;
            break;
        case earmMib:
        case earmDom:
            nFifths = -3;
            break;
        case earmLab:
        case earmFam:
            nFifths = -4;
            break;
        case earmReb:
        case earmSibm:
            nFifths = -5;
            break;
        case earmSolb:
        case earmMibm:
            nFifths = -6;
            break;
        case earmDob:
        case earmLabm:
            nFifths = -7;
            break;
        default:
            wxASSERT(false);
    }
    return nFifths;

}

EKeySignatures GetRelativeMinorKey(EKeySignatures nMajorKey)
{
    switch(nMajorKey) {
        case earmDo:
            return earmLam;
        case earmSol:
            return earmMim;
        case earmRe:
            return earmSim;
        case earmLa:
            return earmFasm;
        case earmMi:
            return earmDosm;
        case earmSi:
            return earmSolsm;
        case earmFas:
            return earmResm;
        case earmDos:
            return earmLasm;
        case earmFa:
            return earmRem;
        case earmSib:
            return earmSolm;
        case earmMib:
            return earmDom;
        case earmLab:
            return earmFam;
        case earmReb:
            return earmSibm;
        case earmSolb:
            return earmMibm;
        case earmDob:
            return earmLabm;
        default:
            wxASSERT(false);
            return earmDom;
    }

}

