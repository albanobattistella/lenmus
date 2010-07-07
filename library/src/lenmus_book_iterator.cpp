//--------------------------------------------------------------------------------------
//    LenMus Library
//    Copyright (c) 2010 LenMus project
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

using namespace std;

namespace lenmus
{


//---------------------------------------------------------------------------------------
// BookIterator
// an iterator to traverse StaffObjs in an instrument
//---------------------------------------------------------------------------------------

BookIterator::BookIterator(LenMusBook* pOwner)
    : m_pBook(pOwner)
    //, m_pPointedSO((lmStaffObj*)NULL)
{
    m_pCursor = m_pBook->GetCollection()->CreateIterator();
}

BookIterator::BookIterator(lmInstrument* pOwner, lmStaffObj* pPointedSO)
    : m_pBook(pOwner)
    //, m_pPointedSO(pPointedSO)
{
    m_pCursor = m_pBook->GetCollection()->CreateIteratorTo(pPointedSO);
}

lmStaffObj* BookIterator::GetPointedObject()
{
    return m_pCursor->GetCurrent();
}



}   //namespace lenmus
