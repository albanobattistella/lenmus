//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
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
/*! @file LDPParser.h
    @brief Header file for class lmLDPParser
    @ingroup ldp_parser
*/
#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __LMPARSER_H        //to avoid nested includes
#define __LMPARSER_H

#include "wx/txtstrm.h"
#include "wx/wfstream.h"

#include "LDPNode.h"
#include "LDPToken.h"
#include "LDPTags.h"

#include "../score/score.h"

// The stack of states will be implemented by an array of integers. Let's declare this array
//WX_DEFINE_ARRAY(int, wxArrayInt);

class lmLDPParser
{
public:
    lmLDPParser();
    ~lmLDPParser();

    // "Parse" methods: work on source text
    lmScore*    ParseFile(const wxString& filename);
    lmLDPNode*  ParseText(const wxString& sSource);

    // "Analyze" methods: work on a tree of LMNodes generated by ParseText().
    // User is responsible for calling the appropiate Analyze method, depending on
    // the type of text parsed
    bool        AnalyzeBarline(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeChord(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeClef(lmVStaff* pVStaff, lmLDPNode* pNode);
    void        AnalyzeFont(lmLDPNode* pNode, wxString* pFontName, int* pFontSize,
                            lmETextStyle* pStyle);
    void        AnalyzeInstrument(lmLDPNode* pNode, lmScore* pScore, int nInstr);
    void        AnalyzeInstrument105(lmLDPNode* pNode, lmScore* pScore, int nInstr);
    bool        AnalyzeKeySignature(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeLocation(lmLDPNode* pNode, int* pValue, lmEUnits* pUnit);
    void        AnalyzeMeasure(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeNewSystem(lmLDPNode* pNode, lmVStaff* pVStaff);
    lmNote*     AnalyzeNote(lmLDPNode* pNode, lmVStaff* pVStaff, bool fChord=false);
    lmRest*     AnalyzeRest(lmLDPNode* pNode, lmVStaff* pVStaff);
    lmScore*    AnalyzeScore(lmLDPNode* pNode);
    void        AnalyzeSplit(lmLDPNode* pNode, lmVStaff* pVStaff);
    EStemType   AnalyzeStem(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeTitle(lmLDPNode* pNode, lmScore* pScore);
    bool        AnalyzeTimeSignature(lmVStaff* pVStaff, lmLDPNode* pNode);
    void        AnalyzeVoice(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeVStaff(lmLDPNode* pNode, lmVStaff* pVStaff);


    // for lmLDPToken
    const wxString& GetNewBuffer();

    // error and logging
    void AnalysisError(const wxChar* szFormat, ...);
    void ParseMsje(wxString sMsg);


    // auxiliary methods
    float   GetDefaultDuration(ENoteType nNoteType, bool fDotted, bool fDoubleDotted,
                      int nTupletNumber);
    int     GetBeamingLevel(ENoteType nNoteType);
    bool    ParenthesisMatch(const wxString& sSource);


private:
    enum EParsingStates
    {
        A0_WaitingForStartOfElement = 0,
        A1_WaitingForName,
        A2_WaitingForParameter,
        A3_ProcessingParameter,
        A4_Exit,
        A5_ExitError
    };

    lmLDPNode*  LexicalAnalysis();
    lmScore*    AnalyzeScoreV102(lmLDPNode* pNode);
    lmScore*    AnalyzeScoreV105(lmLDPNode* pNode);
    void        AnalyzeVStaff_V103(lmLDPNode* pNode, lmVStaff* pVStaff);

    void Clear();
    void FileParsingError(const wxString sMsg);
    void PushNode(EParsingStates nPopState);
    bool PopNode();
    void Do_WaitingForStartOfElement();
    void Do_WaitingForName();
    void Do_WaitingForParameter();
    void Do_ProcessingParameter();

    void ParseError(EParsingStates nState, lmLDPToken* pTk);

    //LDP tags table
    lmLdpTagsTable*     m_pTags;

    // variables to store temporary values as the file is being analized.
    wxInt32     m_nCurStaff;        //default staff num. for the lmNoteRest being processed
    wxInt32     m_nCurVoice;        //default voice num. for the lmNoteRest being processed
    long        m_nNumStaves;       //number of staffs for the lmVStaff being processed
    lmTupletBracket* m_pTupletBracket;      //tuplet being analyzed



    lmLDPTokenBuilder*  m_pTokenizer;       //ptr to token builder object
    lmLDPToken*         m_pTk;              //current token
    EParsingStates      m_nState;           //estado actual del aut�mata
    int                 m_nLevel;           //numero de nodos
    wxArrayInt          m_stackStates;      //Nodo de retorno tras un PopNodo
    ArrayNodePtrs       m_stackNodes;       //Pila de nodos
    lmLDPNode*          m_pCurNode;         //nodo en proceso
    wxString            m_sVersion;         //versi�n del lenguaje en que est� la partitura en proceso
    int                 m_nVersion;         //versi�n convertida a numerico 100*num+rev. Ej. 1.2 = 102

    bool                m_fDebugMode;

    // parsing control and error variables
    bool            m_fFromString;        // true: parsing a string. false: parsing a file
    bool            m_fStartingTextAnalysis;    //to signal the start of a new analysis
    wxString        m_sLastBuffer;        // to keep line under analysis
    long            m_nErrors;            // numebr of serious errors during parsing
    long            m_nWarnings;          // number of warnings during parsing
    wxFileInputStream*    m_pFile;        // file being parsed
    wxTextInputStream*    m_pTextFile;    // file being parsed

    //
    //variables to keep settings that propagate
    //

    // font and aligment for <title> elements
    lmEAlignment    m_nTitleAlignment;
    wxString        m_sTitleFontName;
    int             m_nTitleFontSize; 
    lmETextStyle    m_nTitleStyle;


};
    
#endif    // __LMPARSER_H
