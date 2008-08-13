//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2008 Cecilio Salmeron
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

//THINK: -----------------------------------------------------------------------------------
//Pregunta: �No deber�a existir un constructor en lmNoteRest capaz de crear una nota a partir del fuente?
//Resp: Ning�n PentObj deben crearse fuera de un pentagrama y luego agregarse a �l.
//Pregunta: �Quiz�, entonces, una funcion en CStaff que admita un fuente? �Hay algo as� para
//   otros tipos de PentObjs?
//Resp: Si; ahora mismo existen algunas. Todos los restantes PentObjs
//   se crean a partir de par�metros, no de un fuente.
//Pregunta: �Deben mantenerse o eliminarse?
//Resp: No veo razones por las que una funcion de creacion en CStaff no pueda admitir ambos
//   tipos de argumentos: un fuente o sus par�metros ya procesados. El �nico argumento es que
//   si admite un fuente, el an�lisis del fuente no debe duplicarse, sino que debe estar
//   centralizado en algun sitio, quiza en MParseFile. Ello implica que la esta funci�n de
//   analisis tiene que devolver par�metros, con lo que terminamos en que la creaci�n se
//   hace, al final, a partir de sus par�metros no de su fuente. Sin embargo, puede dar
//   flexibilidad.
//Conclusi�n: Pueden existir funciones de creaci�n a partir de los fuentes, pero siempre
//   son auxiliares ya que tienen que tener la
//   restricci�n de que, internamente, llamen a la funci�n de an�lisis y, tras ella, a la
//   de creaci�n por par�metros.
//Problema: La funci�n de an�lisis de MParse trabaja con una estructura de CNodos no con el
//   fuente plano. Realizar otra para an�lisis del fuente plano duplicar�a el trabajo s�lo
//   con el objeto de no llamar internamente con par�mtros sino con fuente plano
//Conclusi�n final: Se rechaza la creaci�n directa a partir de fuente. Puede hacerse s�lo para
//   encapsular la llamada a la funci�n de an�lisis
//---------------------------------------------------------------------------------------------

#ifndef __LM_NOTEREST_H__        //to avoid nested includes
#define __LM_NOTEREST_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "NoteRest.cpp"
#endif

class lmVStaff;

#include "NotesRelationship.h"

#define lmDEFINE_REST        true
#define lmDEFINE_NOTE        false

// struct BeamInfo represents raw info about this note beaming. It is the same information
// present on a <beam> MusicXML tag.
struct lmTBeamInfo
{
    lmEBeamType    Type;
    bool        Repeat;
};

class lmLyric;

class lmNoteRest:  public lmStaffObj
{
public:
    //ctors and dtor
    virtual ~lmNoteRest();

    inline bool IsRest() const { return m_fIsRest; }
    inline bool IsNote() const { return !m_fIsRest; }

    //implementation of virtual methods of base class lmStaffObj
    virtual bool IsComposite() { return true; }

	//methods related to voice
	inline void SetVoice(int nVoice) { m_nVoice = nVoice; }

    // methods related to note/rest positioning information
    lmLUnits GetStaffOffset() const;

    //methods related to associated AuxObjs management
    lmFermata* AddFermata(const lmEPlacement nPlacement);
    void AddLyric(lmLyric* pLyric);

    //methods related to duration
    inline float GetDuration() const { return m_rDuration; }
    inline float GetTimePosIncrement() { return m_rDuration; }
    wxString GetLDPNoteType();

    //methods related to dots
    inline int GetNumDots() { return m_nNumDots; }
    void ChangeDots(int nDots);

    // methods related to beams
    void CreateBeam(bool fBeamed, lmTBeamInfo BeamInfo[]);
	inline void OnIncludedInBeam(lmBeam* pBeam) { m_pBeam = pBeam; }
	inline void OnRemovedFromBeam() { m_pBeam = (lmBeam*)NULL; }
    inline bool IsBeamed() const { return m_pBeam != (lmBeam*)NULL; }
    inline lmEBeamType GetBeamType(int level) { return m_BeamInfo[level].Type; }
    inline void SetBeamType(int level, lmEBeamType type) { m_BeamInfo[level].Type = type; }
	inline lmBeam* GetBeam() { return m_pBeam; }
	inline lmTBeamInfo* GetBeamInfo() { return &m_BeamInfo[0]; }
    lmBeam* IncludeOnBeam(lmEBeamType nBeamType, lmBeam* pBeam=(lmBeam*)NULL);


    //methods related to tuplets
    inline bool IsInTuplet() { return m_pTuplet != (lmTupletBracket*)NULL; }
    inline lmTupletBracket* GetTuplet() { return m_pTuplet; }
	inline void OnIncludedInTuplet(lmTupletBracket* pTuplet) { m_pTuplet = pTuplet; }
	inline void OnRemovedFromTuplet() { m_pTuplet = (lmTupletBracket*)NULL; }

    //methods related to sound
    void AddMidiEvents(lmSoundManager* pSM, float rMeasureStartTime, int nChannel,
                       int nMeasure);

    //accessors
    inline lmENoteType GetNoteType() const { return m_nNoteType; }
	inline int GetVoice() { return m_nVoice; }

	//source & debug
    virtual wxString Dump();
    virtual wxString SourceLDP(int nIndent);
    virtual wxString SourceXML(int nIndent);

    //undo/redo
    virtual void Freeze(lmUndoData* pUndoData);
    virtual void UnFreeze(lmUndoData* pUndoData);

	//relationships
	template <class T> T* FreezeRelationship(T* pRel, lmUndoData* pUndoData);
	template <class T> T* UnFreezeRelationship(lmUndoData* pUndoData);
	template <class T> T* GetRelationship();
	void OnIncludedInRelationship(void* pRel, lmERelationshipClass nRelClass);
	void OnRemovedFromRelationship(void* pRel, lmERelationshipClass nRelClass);



protected:
    lmNoteRest(lmVStaff* pVStaff, bool IsRest, lmENoteType nNoteType, float rDuration,
               int nNumDots, int nStaff, int nVoice, bool fVisible);

    lmLUnits DrawDot(bool fMeasuring, lmPaper* pPaper, lmLUnits xPos, lmLUnits yPos, 
                     wxColour colorC, bool fUseFont);
    lmLUnits AddDotShape(lmCompositeShape* pCS, lmPaper* pPaper, lmLUnits xPos, lmLUnits yPos, 
                         wxColour colorC);

        
        //
        // member variables
        //

    bool		m_fIsRest;          //This lmNoteRest is a rest
    lmENoteType	m_nNoteType;        //type of note / rest
	int			m_nVoice;			//voice: 1..lmMAX_VOICE
    
    //duration and time modifiers
    float       m_rDuration;            //duration as defined in MusicXML: duration/divisions
    int         m_nNumDots;             //number of dots: 0..n (3 max?)

    // beaming information: only valid if m_pBeam != NULL
    lmBeam*     m_pBeam;                //if not NULL the note/rest is in this beam
    lmTBeamInfo m_BeamInfo[6];          //beam mode for each level

    //tuplet related variables
    lmTupletBracket*    m_pTuplet;    //ptr to lmTupletBracket if this note/rest is part of a tuplet

    //AuxObjs associated to this note
    AuxObjsList*    m_pNotations;     //list of Notations
    AuxObjsList*    m_pLyrics;        //list of Lyrics

};


// global functions related to noterests
extern int LDPNoteTypeToEnumNoteType(const wxString& sNoteType);
extern float LDPNoteTypeToDuration(const wxString& sNoteType);
extern float NoteTypeToDuration(lmENoteType nNoteType, bool fDotted, bool fDoubleDotted);
extern float NoteTypeToDuration(lmENoteType nNoteType, int nDots);



#endif    // __LM_NOTEREST_H__
