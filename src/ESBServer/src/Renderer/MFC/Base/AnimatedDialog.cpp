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
#include "AnimatedDialog.h"

#include <Renderer/UIProperties.h>
#include <Renderer/Win32/Win32Helper.h>

#include <Utilities/Properties.h>
#include <Utilities/StringTokenizer.h>

#include <exception>


BEGIN_MESSAGE_MAP(RaisableDialog, RaisableDialog::super)
  ON_WM_CLOSE()
  ON_WM_SHOWWINDOW()
  ON_WM_GETMINMAXINFO()
  //ON_WM_ACTIVATE()
  //ON_WM_NCACTIVATE()
	//ON_WM_PAINT()
END_MESSAGE_MAP()

#define IDC_ICONX 12345
#define IDC_SPACERX 12346
#define ICON_SIZE 16

#ifndef WS_EX_LAYERED 
  #define WS_EX_LAYERED           0x00080000
  #define LWA_COLORKEY            0x00000001
  #define LWA_ALPHA               0x00000002
  #define ULW_ALPHA               0x00000002
#endif


using namespace std;

RaisableDialog::RaisableDialog(UINT id, CWnd* parent)
: super(id, parent), mCoveredBy(NULL), mFocusableWindow(NULL), mIcon(0), 
  mHoverTimeKey(Browsers::HOVERTIME), mRaiseCornerKey(Browsers::RAISECORNER)
{ 
  registerMouseListener(this); 

}

void RaisableDialog::setIcon(UINT id)
{
  mIcon = AfxGetApp()->LoadIcon(id);
}

void RaisableDialog::moveToForeground()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::moveToForeground()");

  if(!isInForeground()) {
    log(NO_LABEL, RENDERER, eDebug) << "moving window '" << getCaption() << "' to foreground" << endlog;
    CWnd *coveredBy = WindowFromPoint(getRaiseCorner());
    SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOACTIVATE);

    stealFocusFrom(coveredBy);
    
    fade(0/*out*/);
  }
}

void RaisableDialog::moveToBackground()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::moveToBackground()");

  if(mCoveredBy) {
    log(NO_LABEL, RENDERER, eDebug) << "moving window '" << getCaption() << "' to background" << endlog;
    fade(1/*in*/);
    deactivate();
  }
}

void RaisableDialog::expand()
{  
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::expand()");

  if(!isCollapsed()) {
    return;
  }

  log(NO_LABEL, RENDERER, eDebug) << "expanding window '" << getCaption() << "'" << endlog;
  
  vector<string> roll = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::ROLL_ANIMATION, "30:300"), ":").tokens();
  vector<string> fade = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::FADE_ANIMATION, "30:100:300"), ":").tokens();
  opacity().maximum = lexical_cast<size_t>(fade[1]);
  
  if(PropertiesManager::instance().getProperty(Browsers::FADE_UNFADE, "0") != "1") {
    opacity().minimum = 100;
    opacity().maximum = 100;
    opacity(100);
  }
  
  size_t time = lexical_cast<size_t>(roll[2]);
  
  
  CWnd *coveredBy = GetForegroundWindow();
  
  LayeredWindow layer(m_hWnd, false);
  layer.setHorizontalAnimation(mOriginalSize.TopLeft(), mOriginalSize.Size());
  layer.setFadeAnimation(opacity(), opacity().maximum);
  
  ModifyStyle(0, WS_SYSMENU | WS_CAPTION, SWP_NOACTIVATE);
  layer.animate(time);
  
  opacity(opacity().maximum);
  stealFocusFrom(coveredBy);
}

void RaisableDialog::collapse()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::collapse()");

  if(isCollapsed()) {
    return;
  }
  
  log(NO_LABEL, RENDERER, eDebug) << "collapsing window '" << getCaption() << "'" << endlog;

  vector<string> roll = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::ROLL_ANIMATION, "30:300"), ":").tokens();
  vector<string> fade = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::FADE_ANIMATION, "30:100:300"), ":").tokens();
  opacity().minimum = lexical_cast<size_t>(fade[0]);
  
  size_t endWidth = lexical_cast<size_t>(roll[0]);
  size_t endHeight = lexical_cast<size_t>(roll[1]);
  size_t time = lexical_cast<size_t>(roll[2]);
  
  if(PropertiesManager::instance().getProperty(Browsers::FADE_UNFADE, "0") != "1") {
    opacity().minimum = 100;
    opacity().maximum = 100;
    opacity(100);
  }
  
  GetWindowRect(mOriginalSize);
  
  LayeredWindow layer(m_hWnd);
  layer.setHorizontalAnimation(CSize(endWidth, endHeight));
  layer.setFadeAnimation(opacity(), opacity().minimum);
  
  if(endWidth) {
    ModifyStyle(WS_SYSMENU | WS_CAPTION, 0, SWP_NOACTIVATE);
  }
  layer.animate(time);
  
  opacity(opacity().minimum);
  deactivate();
}

void RaisableDialog::fade(size_t fadeout)
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::collapse()");

  if(PropertiesManager::instance().getProperty(Browsers::FADE_UNFADE, "0") != "1") {
    return;
  }
  
  log(NO_LABEL, RENDERER, eDebug) << "fading window '" << getCaption() << "'" << endlog;

  vector<string> fade = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::FADE_ANIMATION, "30:100:300"), ":").tokens();
  
  int steps = lexical_cast<int>(fade[2])/7;
  int op = opacity();
  int opacityDelta;
  
  if(fadeout) {
    opacity().minimum =lexical_cast<size_t>(fade[0]);
    opacityDelta = (static_cast<int>(opacity().minimum)-op)/steps;
  } else {
    opacity().maximum =lexical_cast<size_t>(fade[1]);
    opacityDelta =(static_cast<int>(opacity().maximum)-op)/steps;
  }
  
  for(int i = 0; i < steps; ++i, op+=opacityDelta) {
    opacity(op);
    Sleep(7);
  }
  
  opacity(fadeout ? opacity().minimum : opacity().maximum);
}

void RaisableDialog::mouseHovering(const MouseEvent&)
{   
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::mouseHovering()");
  log(NO_LABEL, RENDERER, eDebug) << " mouse hoovering over window '" << getCaption() << "'" << endlog;
  
  if(PropertiesManager::instance().getProperty(Browsers::RAISE_ON_MOUSE_HOOVER, "1") == "1") {
    moveToForeground();
  }
  
  if(PropertiesManager::instance().getProperty(Browsers::ROLL_UNROLL, "0") == "1") {
    expand();
  }
  
  if(PropertiesManager::instance().getProperty(Browsers::STAY_ALLWAYS_ONTOP, "0") == "1") {
    if(!isInForeground()) {
      SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
    }
  }
}

void RaisableDialog::mouseExited(const MouseEvent&)
{ 
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::mouseExited()");
  log(NO_LABEL, RENDERER, eDebug) << " mouse exited window '" << getCaption() << "'" << endlog;
  
  if(PropertiesManager::instance().getProperty(Browsers::ROLL_UNROLL, "0") == "1") {
    collapse();
  }
  
  if(PropertiesManager::instance().getProperty(Browsers::HIDE_ON_MOUSE_EXIT, "1") == "1") {
    moveToBackground();
  }
  
  if(PropertiesManager::instance().getProperty(Browsers::STAY_ALLWAYS_ONTOP, "0") == "1") {
    SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
  }
}

bool RaisableDialog::isInForeground() 
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::isInForeground()");

  CWnd *wnd = WindowFromPoint(getRaiseCorner());
  return wnd ? wnd->m_hWnd == m_hWnd : true;
}

bool RaisableDialog::isCollapsed()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::isCollapsed()");

  CRect rect;
  GetWindowRect(rect);
  return rect.Width() < 50 || rect.Height() < 50;
}

long RaisableDialog::getHoverTime()
{ 
  //Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::getHoverTime()");

  std::string hovertime = PropertiesManager::instance().getProperty(mHoverTimeKey, "");
  return hovertime.empty() ? HOVER_DEFAULT : lexical_cast<size_t>(hovertime);
}

CPoint RaisableDialog::getRaiseCorner()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::getRaiseCorner()");

  string raiseCorner = PropertiesManager::instance().getProperty(mRaiseCornerKey);
  Corner corner = raiseCorner.empty() ? TOPRIGHT : (Corner)lexical_cast<int>(raiseCorner);
  CRect windowRect;
	GetWindowRect(windowRect);
  CPoint ptCorner(0, 0);
	
  switch (corner)
	{
		case TOPLEFT:
			ptCorner = windowRect.TopLeft();
      ptCorner.Offset(10,10);
			break;

		case TOPRIGHT:
			ptCorner = CPoint(windowRect.right, windowRect.top);
      ptCorner.Offset(-10,10);
			break;

		case BOTTOMRIGHT:
			ptCorner = windowRect.BottomRight();
      ptCorner.Offset(-10,-10);
			break;

		case BOTTOMLEFT:
			ptCorner = CPoint(windowRect.left, windowRect.bottom);
      ptCorner.Offset(10,-10);
			break;

		default:
			ASSERT (0);
	}

	return ptCorner;
}

std::string RaisableDialog::getCaption() const
{
  return mCaption;
}


void RaisableDialog::show(bool modal)
{
}

void RaisableDialog::hide()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::hide()");
  ShowWindow(SW_HIDE);
}

void RaisableDialog::close() 
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::close()");
}

void RaisableDialog::setCaption(const std::string &caption)
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::setCaption()");
  log(NO_LABEL, RENDERER, eDebug) << "renaming window "<< getCaption() << "' to '" << caption << "'" << endlog;

  if(m_hWnd) {
    mCaption = caption;
    SetWindowText(caption.c_str());

  } else {
    mCaption = caption;
  }
}

void RaisableDialog::setPosition(short x, short y)
{
  SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
}

std::pair<short, short> RaisableDialog::getPosition()
{
  CRect rect;
  GetWindowRect(&rect);
  return make_pair((short)rect.TopLeft().x, (short)rect.TopLeft().y);
}



void RaisableDialog::opacity(size_t opacity/*0-100*/)
{ 
  mOpacity = opacity;
}


BOOL RaisableDialog::OnInitDialog()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::OnInitDialog()");

  super::OnInitDialog();

  if(mIcon != 0) {
    SetIcon(mIcon, FALSE);  // Set big icon
	  SetIcon(mIcon, TRUE);		// Set small icon
  }

  CString text;
  GetWindowText(text);
  if(getCaption().empty()) {
    mCaption = text.GetBuffer(0);
  }
    
  if(getCaption() != text.GetBuffer(0)) {
    setCaption(mCaption);
  }

  HideSizeIcon();

  vector<string> fade = StringVectorTokenizer(PropertiesManager::instance().getProperty(Browsers::FADE_ANIMATION, "30:100:300"), ":").tokens();

  mOpacity = Opacity(m_hWnd, lexical_cast<size_t>(fade[1]));
  mOpacity.minimum = lexical_cast<size_t>(fade[0]);
  mOpacity.maximum = lexical_cast<size_t>(fade[1]);

  return FALSE;//TRUE;
}

void RaisableDialog::OnClose()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::OnClose()");

  if(!mPositionKey.empty()) {
    savePosition();
  }

  close();
  super::OnClose();
}

void RaisableDialog::OnShowWindow(BOOL show, UINT)
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::OnShowWindow()");
  log(NO_LABEL, RENDERER, eDebug) << (show ? "showing" : "hiding") << " window '" << getCaption() << "'" << endlog;
  
  setFuzzySize(lexical_cast<size_t>(PropertiesManager::instance().getProperty(Browsers::FUZZY_SIZE, "0")));

  if(show) {
    if(!mPositionKey.empty()) {
      vector<string> pos = StringVectorTokenizer(PropertiesManager::instance().getProperty(mPositionKey), ":").tokens();
      setPosition(lexical_cast<size_t>(pos[0]), lexical_cast<size_t>(pos[1]));
    }
        
    setCaption(mCaption);

    if(PropertiesManager::instance().getProperty(Browsers::STAY_ALLWAYS_ONTOP, "0") == "1") {
      SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
    }
    
  } else {
    savePosition();
  }
}

void RaisableDialog::savePosition()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::savePosition()");

  stringstream position;
  position << getPosition().first << ":" << getPosition().second;
  PropertiesManager::instance().setProperty(mPositionKey, position.str());
  PropertiesManager::instance().dump();
}

void RaisableDialog::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI )
{
  lpMMI->ptMinTrackSize = CPoint(ICON_SIZE, ICON_SIZE);
}



void RaisableDialog::stealFocusFrom(CWnd *wnd)
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::savePosition()");
  
  if(wnd) {
    CWnd *focused = wnd;
    if(focused->GetTopLevelParent()) {
      focused = focused->GetTopLevelParent();
    }

    log(NO_LABEL, RENDERER, eDebug) << "stealing focus from" << ::getCaption(focused->m_hWnd) << endlog;

    if(GetTopLevelParent() == focused) {
      focused = 0;
    }

    if(focused) {
      mCoveredBy = focused->m_hWnd;
    }
  }
  
  SetForegroundWindow();

  if(mFocusableWindow) {
    mFocusableWindow->SetActiveWindow();
    mFocusableWindow->SetFocus();
  }
}

void RaisableDialog::deactivate()
{
  Tracer trc(NO_LABEL, RENDERER, "RaisableDialog::deactivate()");

  if(mCoveredBy) {
    log(NO_LABEL, RENDERER, eDebug) << "restoring focus to" << ::getCaption(mCoveredBy) << endlog;

    ::SetWindowPos(m_hWnd, mCoveredBy, 0, 0, 0 ,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
    ::SetActiveWindow(mCoveredBy);
    ::SetForegroundWindow(mCoveredBy);
    
   
    mCoveredBy = NULL;
  }
}
