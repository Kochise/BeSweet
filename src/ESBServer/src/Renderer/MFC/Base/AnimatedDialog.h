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

#ifndef AnimatedDialog_H
#define AnimatedDialog_H


#include <Renderer/MFC/3rdParty/ResizingDialog.h>
#include <Renderer/MFC/Base/MouseEventDispatcher.h>
#include <Renderer/Win32/Transparency.h>

#include <Utilities/smart_ptr.h>

#include <vector>

class RaisableDialog : public MouseEventDispatcher<CResizingDialog>, public MouseListener
{
public:
  typedef enum{TOPLEFT=0, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT } Corner;
  
private:
  typedef MouseEventDispatcher<CResizingDialog> super;

  HWND mCoveredBy;
  std::string mCaption;

  std::string mHoverTimeKey;
  std::string mRaiseCornerKey;
  std::string mPositionKey;

  CWnd *mFocusableWindow;
  CRect mOriginalSize;
  HICON mIcon;

  CStatic	mIconX;
  CStatic m_Spacer;

  Opacity mOpacity;

public:
  RaisableDialog(UINT id, CWnd* parent = NULL);

  void moveToForeground();
  void moveToBackground();

  void stealFocusFrom(CWnd*);
  void deactivate();

  virtual void show(bool modal = false);
  virtual void hide();
  virtual void close();
  
  virtual void setCaption(const std::string&);
  //virtual std::string getCaption();

  virtual void setPosition(short x, short y);
  virtual std::pair<short, short> getPosition();

  void setRaiseCornerKey(const std::string &key) { mRaiseCornerKey = key; }
  void setHoverTimeKey(const std::string &key) { mHoverTimeKey = key; }
  void setPositionKey(const std::string &key) { mPositionKey = key; }
  void setFocusableWindow(CWnd *wnd) { mFocusableWindow = wnd; }
  void setIcon(UINT id);

  void opacity(size_t /*0-100*/);
  const Opacity& opacity() const { return mOpacity; }
  Opacity& opacity() { return mOpacity; }

  std::string getCaption() const;

protected:
  virtual long getHoverTime();
  void savePosition();
  
  void mouseHovering(const MouseEvent&);
  void mouseExited(const MouseEvent&);

  bool isCollapsed();
  void collapse();
  void expand();
  void fade(size_t level);
    
  afx_msg void OnClose();
  afx_msg void OnShowWindow(BOOL, UINT);
  afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
  BOOL OnInitDialog();
  //afx_msg void OnActivate(UINT state, CWnd *other, BOOL isMinimized);
  //afx_msg BOOL OnNcActivate(BOOL);
  //afx_msg void OnPaint(); 
  DECLARE_MESSAGE_MAP()

private:
  CPoint getRaiseCorner();
  bool isInForeground();  
};

#endif