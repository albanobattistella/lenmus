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
/*! @file GraphicManager.cpp
    @brief Implementation file for class lmGraphicManager
    @ingroup graphic_management
*/
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "GraphicManager.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/image.h"

#include "GraphicManager.h"
#include "Formatter4.h"

//access to colors
#include "../globals/Colors.h"
extern lmColors* g_pColors;

//-----------------------------------------------------------------------------------------

lmGraphicManager::lmGraphicManager()
{
    Create((lmScore*)NULL, (lmPaper*)NULL);
}

lmGraphicManager::lmGraphicManager(lmScore* pScore, lmPaper* pPaper)
{
    Create(pScore, pPaper);
}

void lmGraphicManager::Create(lmScore* pScore, lmPaper* pPaper)
{
    m_pScore = pScore;
    m_pPaper = pPaper;

    //initializations
    m_pBoxScore = (lmBoxScore*) NULL;
    m_rScale = 0;
    m_fReLayout = true;
    m_nLastScoreID = -1;
    m_xPageSize = 0;
    m_yPageSize = 0;

}



lmGraphicManager::~lmGraphicManager()
{
    if (m_pBoxScore) {
        delete m_pBoxScore;
        m_pBoxScore = (lmBoxScore*) NULL;
    }

    DeleteBitmaps();

}

int lmGraphicManager::GetNumPages()
{
    if (m_pBoxScore)
        return m_pBoxScore->GetNumPages();
    else
        return 0;
}

void lmGraphicManager::Layout()
{
    // The goal of this method is to parse the IIR representation of a score and
    // generate the graphical representation so that it can be displayed or printed. 
    // The result is a lmBoxScore object.

    if (m_pBoxScore) {
        delete m_pBoxScore;
        m_pBoxScore = (lmBoxScore*) NULL;
    }
    lmFormatter4 oFormatter;   //the formatter object
    m_pBoxScore = oFormatter.Layout(m_pScore, m_pPaper, &m_options);

}

void lmGraphicManager::Render()
{
    if (!m_pBoxScore) return;
    m_pBoxScore->Render(m_pPaper);

}

wxBitmap* lmGraphicManager::Render(wxDC* pDC, int nPage)
{
    //Renders page 1..n.
    //If pDC==NULL returns the offscreen bitmap with the rendered page.
    //Otherwise, renders the page directly on the received DC
    if (!m_pBoxScore) return (wxBitmap*)NULL;

    if (pDC) {
        //DC received. Render page directly on the DC
        m_pPaper->SetDC(pDC);
        //pDC->SetMapMode(lmDC_MODE);
        //pDC->SetUserScale( m_rScale, m_rScale );
        m_pBoxScore->RenderPage(nPage, m_pPaper);
        return (wxBitmap*)NULL;
    }
    else {
        //No DC received. Return the offscreen bitmap for the requested page
        wxBitmap* pBitmap = GetPageBitmap(nPage);
        if (!pBitmap) {
            pBitmap = NewBitmap(nPage);
            wxMemoryDC memDC;   // Allocate a DC in memory for the offscreen bitmap
            memDC.SelectObject(*pBitmap);
            m_pPaper->SetDC(&memDC);
            memDC.Clear();
            memDC.SetMapMode(lmDC_MODE);
            memDC.SetUserScale( m_rScale, m_rScale );
            m_pBoxScore->RenderPage(nPage, m_pPaper);
            memDC.SelectObject(wxNullBitmap);
        }
        return pBitmap;
    }
}

void lmGraphicManager::Prepare(lmScore* pScore, lmLUnits paperWidth, lmLUnits paperHeight,
                               double rScale, lmPaper* pPaper)
{
    //This method forces a re-layout of the score when needed: 
    // - the first time a score is going to be rendered
    // - if the score has changed
    // - if the scale has changed (? is this needed? --> No, as bitmaps are not affected)
    // - if paper size has changed 

    m_pPaper = pPaper;
    bool fLayoutScore = m_fReLayout;
    m_fReLayout = false;
    if (!m_pScore || m_nLastScoreID != pScore->GetID() || m_rScale != rScale || fLayoutScore)
    {
        fLayoutScore = true;

        //store new values
        m_pScore = pScore;
        m_nLastScoreID = m_pScore->GetID();
        m_rScale = rScale;
        m_xPageSize = paperWidth;
        m_yPageSize = paperHeight;

        //delete the allocated bitmaps
        if (paperWidth != m_xBitmapSize || paperHeight != m_yBitmapSize) {
            DeleteBitmaps();
        }
    }

    //re-layout the score if necesary
    if (fLayoutScore) {
        Layout();
    }

}

void lmGraphicManager::DeleteBitmaps()
{
    wxBitmapListNode* pNode = m_cBitmaps.GetFirst();
    while (pNode) {
        wxBitmap* pBitmap = pNode->GetData();
        delete pBitmap;
        delete pNode;
        pNode = m_cBitmaps.GetFirst();
    }

}

wxBitmap* lmGraphicManager::GetPageBitmap(int nPage)
{
    // nPage = 1 .. n
    // Get the bitmap for requested page, or NULL if no bitmap exits for that page.

    wxASSERT(nPage > 0);
        
    if (nPage > (int)m_cBitmaps.GetCount())
        return (wxBitmap*)NULL;
    else {
        wxBitmapListNode* pNode = m_cBitmaps.Item(nPage-1);
        return (wxBitmap*)pNode->GetData();
    }

}

wxBitmap* lmGraphicManager::NewBitmap(int nPage)
{
    // nPage = 1 .. n
    // Makes room for a new bitmap, for page nPage
    // and returns it (empty bitmap)

    wxLogMessage(_T("[lmGraphicManager::NewBitmap] Page = %d"), nPage);
    //wxMessageBox(wxString::Format(_T("NewPage %d"), nPage));

    //Make room for the new bitmap

    //Allocate the new bit map
    wxBitmap* pBitmap = new wxBitmap(m_xPageSize, m_yPageSize);
    //wxLogMessage(_T("[lmGraphicManager::NewBitmap] Allocated bitmap (%d, %d) pixels, %d bits/pixel. Size= %.02f MB"),
    //    m_xPageSize, m_yPageSize, pBitmap->GetDepth(), (double)((m_xPageSize * m_yPageSize * pBitmap->GetDepth())/8000000.) );
    if (!pBitmap || !pBitmap->Ok()) {
        if (pBitmap) {
            delete pBitmap;
            pBitmap = (wxBitmap *) NULL;
        }
        wxLogMessage(_T("[lmGraphicManager::NewBitmap] Bitmap size (%d, %d) pixels."), m_xPageSize, m_yPageSize);
        wxMessageBox(_("Sorry, not enough memory to create a pBitmap to display the page."),
            _T("lmGraphicManager::NewBitmap"), wxOK);
        ::wxExit();
    }

    // add the new bitmap to the list and store its size
    m_cBitmaps.Append(pBitmap);
    m_xBitmapSize = m_xPageSize;
    m_yBitmapSize = m_yPageSize;
    return pBitmap;
}

void lmGraphicManager::BitmapsToFile()
{
    wxBitmapListNode* pNode = m_cBitmaps.GetFirst();
    int i = 1;
    while (pNode) {
        wxBitmap* pBitmap = pNode->GetData();
        wxImage oImg = pBitmap->ConvertToImage();
        wxString sName = wxString::Format(_T("LenMus_Offscreen_%d.bmp"), i);
        oImg.SaveFile(sName, wxBITMAP_TYPE_BMP);
        pNode = pNode->GetNext();
        i++;
    }

}


