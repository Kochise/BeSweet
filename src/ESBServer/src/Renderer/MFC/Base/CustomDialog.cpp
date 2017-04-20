/**
/*   Copyright (c) 2003by  Marco Welti
/*
/*   This document is  bound by the  QT Public License
/*   (http://www.trolltech.com/licenses/qpl.html).
/*   See License.txt for more information.
/*
/*
/*
/*   ALL RIGHTS RESERVED.  
/* 
/*   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
/*   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/*   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/*   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/*   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/*   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
/*   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
/*   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/*   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/* 
/***********************************************************************/

#include "stdafx.h"
#include "Resource.h"

#include "CustomDialog.h"
//#include "Dib.h"

#include <Renderer/UIProperties.h>
#include <Utilities/StringTokenizer.h>

#include <exception>

using NCButton::ButtonState::eFlat;
using NCButton::ButtonState::eActive;
using NCButton::ButtonState::eSunken;

const size_t CustomDialog::FRAME_SIZE = 3;

NCButton::NCButton(size_t f, size_t a, size_t s)
	:mState(eFlat), mIsCheckbox(true), mSize(16,15)
{	
	mImages[eFlat] = smart_ptr<CBitmap>(new CBitmap());
	mImages[eFlat]->LoadBitmap(f);

	mImages[eActive] =  smart_ptr<CBitmap>(new CBitmap());
	mImages[eActive]->LoadBitmap(a);

	mImages[eSunken] =  smart_ptr<CBitmap>(new CBitmap());
	mImages[eSunken]->LoadBitmap(s);
}

NCButton::~NCButton()
{
}

CustomDialog::CustomDialog(ULONG dialogID, CWnd* pParent)
: CDialog(dialogID, pParent)
{
	mButtons.push_back(NCButton(IDB_SORT_F, IDB_SORT_A, IDB_SORT_S));
	mButtons[0].setPos(CPoint(0,0));

	/*mButtons.push_back(NCButton(IDB_NO_FIELDS_F, IDB_NO_FIELDS_A, IDB_NO_FIELDS_S));
	mButtons[1].setPos(CPoint(16,0));

	mButtons.push_back(NCButton(IDB_NO_STATIC_F, IDB_NO_STATIC_A, IDB_NO_STATIC_S));
	mButtons[2].setPos(CPoint(32,0));

	mButtons.push_back(NCButton(IDB_PUBLIC_ONLY_F, IDB_PUBLIC_ONLY_A, IDB_PUBLIC_ONLY_S));
	mButtons[3].setPos(CPoint(48,0));*/
}

BEGIN_MESSAGE_MAP(CustomDialog, CDialog)
	//{{AFX_MSG_MAP(CTemplateDlg)
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool isPointInRect(const CPoint &p, const CRect &r)
{
	bool c1 = p.x > r.left;
	bool c2 = p.x < r.right;
	bool c3 = p.y < r.bottom;
	bool c4 = p.y > r.top;
	return c1&&c2&&c3&&c4;
//return p.x > r.left && p.x < r.right && p.y < r.bottom && p.y > r.top;
}

CRect CustomDialog::getCaptionRect()
{
	CRect wr;
	GetWindowRect(&wr);
	ScreenToClient(&wr);

	return CRect(wr.left+FRAME_SIZE+18, wr.top+FRAME_SIZE, wr.right-FRAME_SIZE, 0); 
}

NCButton& CustomDialog::getButton(CPoint mousePosition)
{
 	CRect caption = getCaptionRect();
	for(size_t i = 0; i < mButtons.size(); i++) {
		CPoint pos(mButtons[i].getPos());
		CSize size(mButtons[i].getSize());

		CPoint topLeft(pos.x-caption.left, caption.top + pos.y);

		CRect rect(topLeft, CPoint(topLeft+size)); 
		
		if(isPointInRect(mousePosition, rect)) {
			return mButtons[i];
		}
	}

  throw std::runtime_error("");
}



UINT CustomDialog::OnNcHitTest(CPoint point) 
{ 
  UINT ht = CDialog::OnNcHitTest(point); 
	if (ht == HTCAPTION)
	{
		bool hasAnythingChanged = false;
		
		try {
			ScreenToClient(&point);
			mouseAt(point);
			NCButton  *button = &getButton(point);
			if(button->getState() == eFlat) {
				button->setState(eActive);
				hasAnythingChanged = true;
			}

		} catch(...) {
			//mouse not over any button....ensure all previous active buttons are set to flat;
			for(size_t i = 0; i < mButtons.size(); i++) {
				if(mButtons[i].getState() == eActive) {
					mButtons[i].setState(eFlat); 
					hasAnythingChanged = true;
				}
			}
		}
		
		if(hasAnythingChanged) {
			SendMessage(WM_NCPAINT, NULL, NULL);
		}
	}
  
  return ht; 
} 

void CustomDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	mIsMouseButtonDown = true;
	
  try {
    ScreenToClient(&point);
    NCButton *button = &getButton(point);
		//SetCapture();
		button->setState(button->getState() == eSunken ? eFlat : eSunken);

  } catch(...) {}

	SendMessage(WM_NCPAINT, NULL, NULL);
	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CustomDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	mIsMouseButtonDown = false;

  //ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);

	/*switch(mnNumberButtonCliked)
	{
	case 0:
		{
			onButton1();
			break;
		}
	case 1:
		{
			onButton2();
			break;
		}
  }*/
}

void CustomDialog::OnNcPaint()
{
  CDialog::OnNcPaint();
  Default();

	CRect caption = getCaptionRect();
	//CalculateCaptionTextRect(this, &rc);
	
	CDC* pDC = GetWindowDC();

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, caption.Width(), caption.Height()); 
	//CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);
	
	CSize sizeButton(GetSystemMetrics(SM_CXSIZE), GetSystemMetrics(SM_CYSIZE));
  
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		NCButton *button = &mButtons[i];

		//CDC mem;
		//mem.CreateCompatibleDC(&dcMem);
		CBitmap* pOld = dcMem.SelectObject(button->getImage());

		BITMAP bm;
		button->getImage()->GetObject(sizeof(bm), &bm);

		pDC->BitBlt(button->getPos().x+caption.left, button->getPos().y+FRAME_SIZE, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pOld);	
	}
	//pDC->BitBlt(0, 0, bar.Width(), bar.Height(), &dcMem, 0,0, SRCCOPY);
	//dcMem.SelectObject(pOldBitmap);
  
	//DeleteObject(pOldBitmap);
	//ReleaseDC(&dcMem);
	//ReleaseDC(pDC);
}
//////////////////////////////////////////////////////////////////

