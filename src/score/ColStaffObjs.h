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

#ifndef __LM_COLSTAFFOBJS_H__        //to avoid nested includes
#define __LM_COLSTAFFOBJS_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ColStaffObjs.cpp"
#endif

#include <vector>
#include <list>

class lmStaffObj;
class lmSOIterator;
class lmContext;
class lmVStaff;
class lmInstrument;
class lmVStaffCursor;
class lmColStaffObjs;
class lmBarline;


// To simplify future modifications of this class (i.e changing the data structures to implement it)
// when an iterator is requested we will force to specify the intended usage so that 
// this class can optimize it. The defined codes are:
//
//	eTR_ByTime:
//		The StaffObjs must be recovered ordered by increasing time position.
//
//	eTR_AsStored:
//		Order is not important. Items are recovered as it is faster.

enum ETraversingOrder
{
    eTR_AsStored = 1,		//no order, as it is more fast
    eTR_ByTime,				//ordered by timepos
};


#define lmItMeasure		std::list<lmSegmentData*>::iterator 
#define lmItCSO			std::list<lmStaffObj*>::iterator

class lmSegment
{
public:
    lmSegment(lmVStaff* pOwner, int nSegment);
    ~lmSegment();

	void Store(lmStaffObj* pNewSO, lmVStaffCursor* pCursor);

	//context management
    inline void SetContext(int iStaff, lmContext* pContext) { m_pContext[iStaff] = pContext; }
	inline lmContext* GetContext(int iStaff) { return m_pContext[iStaff]; }

    //info
    int GetNumVoices();
    bool IsVoiceUsed(int nVoice);

	//debug
	wxString Dump();


private:
	friend class lmVStaffCursor;
	friend class lmColStaffObjs;
	friend class lmSOIterator;

    void VoiceUsed(int nVoice);
    void ShiftTimepos(lmItCSO itStart, int nVoice);

    std::list<lmStaffObj*>	m_StaffObjs;		//list of StaffObjs in this segment
    int				m_nNumSegment;				//0..n-1
    lmBarline*		m_pBarline;					//segment barline, if exists
    lmContext*		m_pContext[lmMAX_STAFF];	//ptr to current context for each staff
    int             m_bVoices;                  //voices in this segment. One bit per used voice

 //   float	m_rTime[lmMAX_VOICE];	//time from start of the measure. One counter per voice
	//									//Time is measured in 256th notes: 1-256th, ..., 256-whole, 512-double whole
	//									//Float for grater precision (triplets problem, see comment at CPONota.ExactDuration)
 //   float	m_rMaxTime[lmMAX_VOICE]; //aqui se guarda el m�ximo tiempo alcanzado en el compas en curso. Sirve para evitar que una
	//									//orden <avanzar> sobrepase este valor. Adem�s, este ser� el tiempo que corresponde a la barra
	//									//de fin de compas.

};


class lmColStaffObjs
{
public:
    lmColStaffObjs(lmVStaff* pOwner, int nNumStaves);
    ~lmColStaffObjs();

	//creation related
	inline void SetCursor(lmVStaffCursor* pCursor) { m_pVCursor = pCursor; } 
    void AddStaff();

    //add/remove StaffObjs
    void Add(lmStaffObj* pNewSO);
	void Delete(lmStaffObj* pCursorSO);
	//Compatibility. //TODO: remove
    void Store(lmStaffObj* pNewSO) { Add(pNewSO); }
	void Insert(lmStaffObj* pNewSO, lmStaffObj* pBeforeSO) { Add(pNewSO); }
    bool ShiftTime(float rTimeShift);

    //iterator related methods
    lmSOIterator* CreateIterator(ETraversingOrder nOrder, int nVoice=-1);
	lmSOIterator* CreateIteratorTo(ETraversingOrder nOrder, lmStaffObj* pSO);

	//measures related
    int GetNumMeasures();

	//context management
	lmContext* GetCurrentContext(lmStaffObj* pSO);
	lmContext* NewUpdatedContext(lmStaffObj* pSO);
	lmContext* NewUpdatedLastContext(int nStaff);
	lmContext* GetLastContext(int nStaff);
    lmContext* GetStartOfSegmentContext(int nMeasure, int nStaff);

	//debug
	wxString Dump();

	//info
	int GetNumVoicesInMeasure(int nMeasure);
    bool IsVoiceUsedInMeasure(int nVoice, int nMeasure);



private:
	friend class lmSOIterator;
	friend class lmVStaffCursor;
	friend class lmSegment;

	//general management

	//segments management
	void SplitSegment(int nSegment);
    void CreateNewSegment(int nSegment);
	void AddSegment(int nSegment);
	void InsertSegment(int nSegment);
	void UpdateContexts(lmSegment* pSegment);

	//voices management
	void AssignVoice(lmStaffObj* pSO);

	//timepos management
    void AssignTime(lmStaffObj* pSO);
    bool IsTimePosOccupied(lmSegment* pSegment, float rTime, float rDuration);


    lmVStaff*                   m_pOwner;           //owner VStaff
	std::vector<lmSegment*>		m_Segments;			//segments collection
    int                         m_nNumStaves;       //num staves in the collection
	lmVStaffCursor*          	m_pVCursor;			//cursor

	//voices management
    int                         m_nNumVoices;       //num voices in the collection
    int							m_nCurVoice[lmMAX_STAFF];	//num of current voice for each staff

};



// Cursor pointing to current position
class lmVStaffCursor
{
public:
	lmVStaffCursor();
	~lmVStaffCursor() {}

	//creation related
	void AttachToCollection(lmColStaffObjs* pColStaffObjs);

    //positioning
	void MoveRight();
	void MoveLeft();
    void MoveToTime(float rNewTime);
    void ResetCursor();
    void AdvanceToTime(float rTime);
    void AdvanceToNextSegment();

    //status
    bool IsAtEnd();
    bool IsAtBeginning();

	//access to cursor info
	inline int GetSegment() { return m_nSegment; }
	lmStaffObj* GetStaffObj();
    inline float GetTimepos() { return m_rTimepos; }
    inline lmItCSO GetCurIt() { return m_it; }


private:

	lmColStaffObjs*		m_pColStaffObjs;	//collection pointed by this cursor
	int					m_nStaff;			//staff (1..n)
	int					m_nSegment;			//current segment (0..n-1)
	lmSegment*			m_pSegment;			//current segment
	float				m_rTimepos;			//timepos
	lmItCSO				m_it;				//iterator, to speed up cursor moves

};



#endif    // __LM_COLSTAFFOBJS_H__
