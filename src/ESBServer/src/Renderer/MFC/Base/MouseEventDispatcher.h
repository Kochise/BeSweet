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

#ifndef MouseEventDispatcher_H
#define MouseEventDispatcher_H

//#include <windows.h>
#include "stdafx.h"

#include "stl_ext.h"

#include <list>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional>

// leftbutton=1
// rightbutton=2
// wheelbutton=16
// ctrl=8
// shift=4

#define MOUSE_EVENT_BUTTON_MASK  0x13
#define MOUSE_EVENT_MODIFIERS_MASK  0xc

/*
#define WM_NCMOUSEHOVER 672
#define WM_NCMOUSELEAVE 674
#define TME_NONCLIENT 0x10
*/
class MouseEvent
{
public:
  typedef enum { eEnter, eMove, eHover, eExit, eClicked} MouseEventType;
  typedef enum { eNoButton=0, eLeftButton=1, eMiddleButton=16, eRightButton=2 } MouseButton;
  typedef enum { eShift = 4, eCtrl = 8 } MouseModifier;

private:
  std::pair<short, short> mScreenPos;
  CWnd *mComponent;
  MouseEventType mEventType;
  size_t mFlags;
  size_t mClickCount;

public:
  MouseEvent() {}
  MouseEvent(MouseEventType type, size_t flags, CWnd *component, const CPoint &pos, size_t clickCount = 0)
    : mScreenPos(std::make_pair((short)pos.x, (short)pos.y)), mFlags(flags), mComponent(component), mEventType(type), mClickCount(clickCount) {}

  size_t getClickCount() { return mClickCount; }
  std::pair<short, short> getPoint() const { return mScreenPos; }
  MouseButton getButton() const { return static_cast<MouseButton>(mFlags & MOUSE_EVENT_BUTTON_MASK); }
  size_t getMouseModifiers() const { return mFlags & MOUSE_EVENT_MODIFIERS_MASK; }
  CWnd *getSource(){ return mComponent; }
};

class MouseListener
{
public:
  virtual ~MouseListener(){}
  virtual void mouseMoved(const MouseEvent &e) {};
  virtual void mouseEntered(const MouseEvent &e) {};
  virtual void mouseExited(const MouseEvent &e) {};
  virtual void mouseHovering(const MouseEvent &e) {};
  virtual void mouseClicked(const MouseEvent &e) {};
  virtual void mousePressed(const MouseEvent &e) {};
  virtual void mouseReleased(const MouseEvent &e) {};

  bool mouseMoved_(MouseEvent e) { mouseMoved(e); return true; };
  bool mouseEntered_(MouseEvent e) { mouseEntered(e);return true; };
  bool mouseExited_(MouseEvent e) { mouseExited(e); return true; };
  bool mouseHovering_(MouseEvent e) { mouseHovering(e); return true; };
  bool mouseClicked_(MouseEvent e) { mouseClicked(e); return true; };
  bool mousePressed_(MouseEvent e) { mousePressed(e); return true; };
  bool mouseReleased_(MouseEvent e) { mouseReleased(e); return true; };
};

template<class Parent>
class MouseEventDispatcher : public Parent
{
private:
  std::list<MouseListener*> mListeners;
  long mHoverTime;
  size_t mFuzzySize;
  bool mIsMouseInside;
  bool mIsHovering;
  size_t mTimerID;
  std::pair<DWORD, CPoint> mPreviousMousePos;

public:
  typedef Parent Base;
  
  MouseEventDispatcher() 
  : mIsMouseInside(false), mIsHovering(false), mTimerID(0), mHoverTime(HOVER_DEFAULT), mFuzzySize(0)
  {};
  
  MouseEventDispatcher(UINT id, CWnd *parent = NULL) 
  : Parent(id, parent), mIsMouseInside(false), mIsHovering(false), mTimerID(0), mHoverTime(HOVER_DEFAULT), mFuzzySize(0)
  {};

  void setHoverTime(long ht)
  { mHoverTime = ht; }

  virtual long getHoverTime()
  { return mHoverTime; }

  void setFuzzySize(size_t s)
  { mFuzzySize = s; }

  size_t getFuzzySize()
  { return mFuzzySize; }

  void registerMouseListener(MouseListener *l) { mListeners.push_back(l); }
  
private:
  bool isInFrame(bool fuzzy)
  {
    CRect window;
    GetWindowRect(&window);
    
    CPoint cursor;
    GetCursorPos(&cursor);
    if(fuzzy) {
      size_t s = getFuzzySize();
      if(cursor.x >= window.right) {
        cursor.x -= s;
      } else if(cursor.x <= window.left) {
        cursor.x += s;
      }

      if(cursor.y <= window.top) {
        cursor.y += s;
      } else if(cursor.y >= window.bottom) {
        cursor.y -= s;
      }
    }

    if(window.PtInRect(cursor)) {
      CWnd *wnd = WindowFromPoint(cursor);
      if(!wnd) {
        log(NO_LABEL, RENDERER, eDebug) << typeid(*this).name() << "::isInFrame(): no window below cursor!!" << endlog;
        return false;
      }

      if((wnd->m_hWnd == m_hWnd || IsChild(wnd) || wnd->GetTopLevelParent()->m_hWnd == m_hWnd)) {
        return true;
      }
      log(NO_LABEL, RENDERER, eDebug) <<  typeid(*this).name() << "::isInFrame(): not a child" << endlog;
    }
    return false;
  }

  BOOL PreTranslateMessage(MSG* pMsg)
  {
    try {
      if(pMsg->message == WM_NCMOUSEMOVE || pMsg->message == WM_MOUSEMOVE) {
        CPoint mousePos(pMsg->pt);
        if(mPreviousMousePos.second != mousePos) {
          mouseMoved(MouseEvent(MouseEvent::eMove, pMsg->wParam/*pMsg->lParam,*/ , this, mousePos));
          mPreviousMousePos = std::make_pair(pMsg->time, mousePos);
        }

      } else if(pMsg->message == WM_TIMER && pMsg->wParam == mTimerID) {
        if(!isInFrame(true/*fuzzy*/)) {
          mouseExited(MouseEvent(MouseEvent::eExit, pMsg->wParam/*pMsg->lParam,*/ , this, CPoint(pMsg->pt)));

        } else if(isInFrame(false) && static_cast<long>(pMsg->time - mPreviousMousePos.first) > getHoverTime()) {
          if(!mIsHovering) {
            mouseHovering(MouseEvent(MouseEvent::eHover, pMsg->wParam/*pMsg->lParam,*/ , this, CPoint(pMsg->pt))); 
          }
        }

      } else if(pMsg->message == WM_LBUTTONUP || pMsg->message == WM_NCLBUTTONUP) {
        mouseButtonClicked(MouseEvent(MouseEvent::eClicked, MouseEvent::eLeftButton | pMsg->wParam, this, CPoint(pMsg->pt)));
    
      } else if(pMsg->message == WM_MBUTTONUP || pMsg->message == WM_NCMBUTTONUP) {
        mouseButtonClicked(MouseEvent(MouseEvent::eClicked, MouseEvent::eMiddleButton | pMsg->wParam, this, CPoint(pMsg->pt)));
    
      } else if(pMsg->message == WM_RBUTTONUP || pMsg->message == WM_NCRBUTTONUP) {
        mouseButtonClicked(MouseEvent(MouseEvent::eClicked, MouseEvent::eRightButton | pMsg->wParam, this, CPoint(pMsg->pt)));
      }
    } catch(std::exception &e) {
      log(NO_LABEL, RENDERER, eError) << typeid(*this).name() << "::PreTranslateMessage()" << std::endl << e << endlog;
    
    } catch(...) {
      log(NO_LABEL, RENDERER, eError) << typeid(*this).name() << "::PreTranslateMessage(): " << "unknown exception occured" << endlog;

    }
    
    return Base::PreTranslateMessage(pMsg);
  }

  void mouseEntered(const MouseEvent &event)
  {
    mIsMouseInside = true;
    mIsHovering = false;
    mTimerID = SetTimer(1, 100, NULL);
    dispatchEvent(event, std::mem_fun(MouseListener::mouseEntered_));
  }

  void mouseMoved(const MouseEvent &event)
  {     
    mIsHovering = false;
    if(!mIsMouseInside) {
      mouseEntered(event);

    } else {
      dispatchEvent(event, std::mem_fun(MouseListener::mouseMoved_));
    }
  }
  
  void mouseExited(const MouseEvent &event)
  { 
    mIsMouseInside = false;
    mIsHovering = false;
    KillTimer(mTimerID);
    dispatchEvent(event, std::mem_fun(MouseListener::mouseExited_));
  }

  void mouseHovering(const MouseEvent &event)
  { 
    mIsHovering = true;
    dispatchEvent(event, std::mem_fun(MouseListener::mouseHovering_));
  }

  void mouseButtonClicked(const MouseEvent &event)
  {
    dispatchEvent(event, std::mem_fun(MouseListener::mouseClicked_));
  }

  template<class FunctionAdaptor>
  void  dispatchEvent(const MouseEvent &event, const FunctionAdaptor &function) {
    size_t dbgCnt = mListeners.size();
    std::for_each(mListeners.begin(), mListeners.end(), std::bind2nd(function, event));
  }
};


#endif
