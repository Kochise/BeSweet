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

#ifndef CustomDialog_H
#define CustomDialog_H

#include <vector>
#include "smart_ptr.h"

#include "ResizingDialog.h"
#include "MouseEventDispatcher.h"
#include "Properties.h"

class NCButton {
public:
	typedef enum { eFlat = 0, eActive, eSunken } ButtonState;

private:
	ButtonState mState;
	smart_ptr<CBitmap> mImages[3];

	CPoint mPos;
	CSize mSize;
	bool mIsCheckbox;

public:
	NCButton() : mState(eFlat) {}

	NCButton(size_t f, size_t a, size_t s);
	~NCButton();

	CBitmap* getImage() { return mImages[mState].get(); }

	bool isCheckbox() { return mIsCheckbox; }

	ButtonState getState() const { return mState; }
	void setState(ButtonState s) { mState = s; }

	const CPoint& getPos() const { return mPos; };
	void setPos(const CPoint &pos) { mPos = pos; };

	const CSize& getSize() const { return mSize; };
	void setPos(const CSize &size) { mSize = size; };
};

class CustomDialog : public CDialog
{
private:
  std::vector<NCButton> mButtons;
  bool mIsMouseButtonDown;
	const static size_t FRAME_SIZE;

public:
  CustomDialog(ULONG dialogID, CWnd* pParent = NULL);
	virtual void mouseAt(CPoint&) = 0;

protected:
	NCButton& getButton(CPoint);
	CRect getCaptionRect();

  //{{AFX_MSG(CTemplateDlg)
	afx_msg void OnNcPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown( UINT nHitTest, CPoint point );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};




#include <Renderer/MFC/base/AnimatedDialog.h>
#endif
