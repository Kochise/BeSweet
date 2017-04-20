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
#include <stdafx.h>
#include "Transparency.h"
#include "Win32Helper.h"

#ifndef WS_EX_LAYERED 
  #define WS_EX_LAYERED           0x00080000
  #define LWA_COLORKEY            0x00000001
  #define LWA_ALPHA               0x00000002
  #define ULW_ALPHA               0x00000002
  //BOOL WINAPI SetLayeredWindowAttributes(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

#endif

LayeredWindowApi::lpfnSetLayeredWindowAttributes LayeredWindowApi::SetLayeredWindowAttributes = NULL;
LayeredWindowApi::lpfnGetLayeredWindowAttributes LayeredWindowApi::GetLayeredWindowAttributes = NULL;
LayeredWindowApi::lpfnUpdateLayeredWindow LayeredWindowApi::UpdateLayeredWindow = NULL;

LayeredWindowApi::LayeredWindowApi()
{
  HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
  SetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
  GetLayeredWindowAttributes = (lpfnGetLayeredWindowAttributes)GetProcAddress(hUser32, "GetLayeredWindowAttributes");
  UpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
  //AnimateWindow = (lpfAnimateWindow)GetProcAddress(hUser32, "AnimateWindow");
}


LayeredWindowApi& LayeredWindowApi::instance()
{
  static LayeredWindowApi theApi;
  return theApi;
}

void foo()
{
  //message_box("UpdateLayeredWindow failed:\n" + std::string(Win32Error()));
}

void LayeredWindowApi::updateLayeredWindow(HWND wnd, HDC dstDC, POINT *dst, SIZE *size, HDC srcDC, POINT *src, COLORREF color, BLENDFUNCTION *blend, DWORD flags)
{
  if(UpdateLayeredWindow) {
    if(!UpdateLayeredWindow(wnd, dstDC, dst, size, srcDC, src, color, blend, flags)) {
      log(NO_LABEL, RENDERER, eWarning) << "UpdateLayeredWindow() failed. Reason " << std::string(Win32Error()) << endlog;
    }
  } else {
    //message_box("UpdateLayeredWindow null!!");
  }
}

void LayeredWindowApi::setLayeredWindowAttributes(HWND wnd, COLORREF color, BYTE alpha, DWORD flags)
{ 
  if(SetLayeredWindowAttributes) {
    SetLayeredWindowAttributes(wnd, color, alpha, flags);
  }
}
  
void LayeredWindowApi::getLayeredWindowAttributes(HWND wnd, COLORREF *color, BYTE *alpha, DWORD *flags)
{ 
  if(GetLayeredWindowAttributes)  {
    GetLayeredWindowAttributes(wnd, color, alpha, flags);
  }
}


Opacity::Opacity(HWND wnd, size_t level)
: mWnd(wnd), mOpacityLevel(level)
{

  SetWindowLong(mWnd, GWL_EXSTYLE, GetWindowLong(mWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
  set(mOpacityLevel);
}

void Opacity::set(size_t level)
{
  mOpacityLevel = level > 100 ? 100 : level;
  LayeredWindowApi::instance().setLayeredWindowAttributes(mWnd, 0, (255 * mOpacityLevel) / 100, LWA_ALPHA);
}

const int ANIMATION_STEP_DURATION = 10;

LayeredWindow::LayeredWindow(HWND wndBelow, bool isVisible)
: mOther(wndBelow)
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::LayeredWindow()");
  log(NO_LABEL, RENDERER, eDebug) << "creating layerd window for window '" << getCaption(wndBelow) << "'" << endlog;

  RECT area;
  ::GetWindowRect(wndBelow, &area);

  POINT topLeft = {area.left, area.top};
  SIZE size = {area.right-area.left, area.bottom-area.top};
  
  initialize(topLeft, size, isVisible);
  mOpacity = Opacity(*this, 100);
}


LayeredWindow::~LayeredWindow()
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::~LayeredWindow()");
  DestroyWindow(mSelf);
}

HWND LayeredWindow::newLayer(const POINT &topLeft, const SIZE &size, bool isVisible)
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::newLayer()");
  log(NO_LABEL, RENDERER, eDebug) << "creating new layer at " << topLeft.x << "x" << topLeft.y << endlog;

  return CreateWindowEx(WS_EX_LAYERED | // Layered Windows
    WS_EX_TRANSPARENT | // Don't hittest this window
    WS_EX_TOPMOST | WS_EX_TOOLWINDOW, 
    "STATIC", "", WS_POPUP | (isVisible ? WS_VISIBLE : 0), topLeft.x,
    topLeft.y, 0, 0, (HWND)0, (HMENU)0, (HINSTANCE)0, NULL);
}

void LayeredWindow::initialize(const POINT &topLeft, const SIZE &size, bool isVisible)
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::initialize()");
  mSelf = newLayer(topLeft, size, isVisible);
  takeScreenShot();
}

void LayeredWindow::reinitialize()
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::reinitialize()");
  
  RECT rect;
  GetWindowRect(*this, &rect);
  
  SIZE size = { rect.right-rect.left, rect.bottom-rect.top };
  POINT dest = { rect.left, rect.top};
  HWND hack = mSelf;
  initialize(dest, size, true);
  DestroyWindow(hack);
}

void LayeredWindow::takeScreenShot()
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::takeScreenShot()");

  RECT rect;
  ::GetWindowRect(mOther ? mOther : mSelf, &rect);

  SIZE size = { rect.right-rect.left, rect.bottom-rect.top};
  POINT src = {0,0};
  
  //RECT self;
  //GetWindowRect(*this, &self);
  //POINT dst = { self.left, self.top};
  
  HDC dstDC = ::GetDC(mOther ? mOther : ::GetDesktopWindow());
    
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, 0};
  LayeredWindowApi::instance().updateLayeredWindow(*this, NULL, NULL, &size, dstDC, &src, NULL, &blend, ULW_ALPHA);

  //SetWindowPos(*this, NULL, 0, 600, 0,0, SWP_NOSIZE | SWP_NOZORDER);
}

void LayeredWindow::setHorizontalAnimation(POINT dest, SIZE size)
{
  setHorizontalAnimation(size);
  mEndDest = dest;
}

void LayeredWindow::setHorizontalAnimation(SIZE size)
{
  mEndDest.x = -1;
  mEndDest.y = -1;
  mEndSize = size;
}

void LayeredWindow::setFadeAnimation(size_t from, size_t to)
{
  mFadeFrom = from;
  mFadeTo = to;
}

bool LayeredWindow::isInRightScreenHalf()
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::isInRightScreenHalf()");

  int width = GetSystemMetrics(SM_CXFULLSCREEN);
  int height = GetSystemMetrics(SM_CYFULLSCREEN);
  
  RECT pos;
  ::GetWindowRect(*this, &pos);
  return pos.left > GetSystemMetrics(SM_CXFULLSCREEN)/2;
  //To get the work area (i.e. Taskbar etc. excluded) use 
  //RECT r;
  //SystemParameterInfo(SPI_GETWORKAREA, 0, &r, 0);
}


void LayeredWindow::collaps(size_t time)
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::collaps()");
  log(NO_LABEL, RENDERER, eDebug) << "playing collaps animation for window " << mOther << ": " << getCaption(mOther) << endlog;
  
  RECT rect;
  ::GetWindowRect(*this, &rect);
  
  //i have no clue why this is neccessary !!!
  reinitialize();
  bool leftToRightCollaps = isInRightScreenHalf();  
  
  SIZE size = { rect.right-rect.left, rect.bottom-rect.top };
  POINT dest = { rect.left, rect.top};

  int steps = time/ANIMATION_STEP_DURATION; 
  int deltaW = mEndSize.cx > 0 ? (size.cx-mEndSize.cx)/steps : 0;
  int deltaH = mEndSize.cy > 0 ? (size.cy-mEndSize.cy)/steps : 0;
  int deltaAlpha = steps ? ((mFadeFrom-mFadeTo)*255)/(100*steps) : 0;
  
  Opacity opacity(mOther, 0);
  
  POINT src = {0, 0};
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, (mFadeFrom*255)/100, 0};
  
  for(int i = 0; i < steps; ++i) {
    
    if(leftToRightCollaps) {
      dest.x += deltaW;

    } else {
      src.x += deltaW;
    }

    src.y += deltaH; 
    
    size.cx -= deltaW;
    size.cy -= deltaH;
    blend.SourceConstantAlpha -= deltaAlpha;

    LayeredWindowApi::instance().updateLayeredWindow(*this, NULL, &dest, &size, ::GetDC(mOther), &src, NULL, &blend, ULW_ALPHA);
        
    Sleep(ANIMATION_STEP_DURATION);
  }
  
  
  POINT end = { rect.right - mEndSize.cx, rect.top};
  if(!leftToRightCollaps) {
    end.x = rect.left;
  }

  if(mEndSize.cx <= 0) {
    mEndSize.cx = sizeOf(rect).cx;
    end.x = rect.left;
  }

  if(mEndSize.cy <= 0) {
    mEndSize.cy = sizeOf(rect).cy;
  }

  if(end.x < 0) {
    end.x = 0;
  } else if(end.x > GetSystemMetrics(SM_CXFULLSCREEN)-5) {
    end.x = GetSystemMetrics(SM_CXFULLSCREEN) -5;
  }
  if(end.y < 0) {
    end.y = 0;
  } else if(end.y > GetSystemMetrics(SM_CYFULLSCREEN)-5) {
    end.y = GetSystemMetrics(SM_CYFULLSCREEN) -5;
  }

  log(NO_LABEL, RENDERER, eDebug) << "moving window to final position " << end.x << "x" << end.y << endlog;
  ::SetWindowPos(mOther, NULL, end.x, end.y, mEndSize.cx, mEndSize.cy, SWP_NOZORDER | SWP_NOACTIVATE);
  opacity = mFadeTo;
  DestroyWindow(mSelf);
}

void LayeredWindow::expand(size_t time)
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::expand()");
  log(NO_LABEL, RENDERER, eDebug) << "playing expand animation for window " << mOther << ": " << getCaption(mOther) << endlog;

  RECT rect;
  GetWindowRect(mOther, &rect);
  
  
  bool left2RightExpand =  !isInRightScreenHalf();
  
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, 255, 0};
  
  int steps = time/ANIMATION_STEP_DURATION; 
  int deltaW = (mEndSize.cx-sizeOf(rect).cx)/steps;
  int deltaH = (mEndSize.cy-sizeOf(rect).cy)/steps;
  int deltaAlpha = steps ? (((int)mFadeFrom-(int)mFadeTo)*255)/(100*steps) : 0;

  
  RECT endArea = { mEndDest.x, mEndDest.y, mEndDest.x + mEndSize.cx, mEndDest.y + mEndSize.cy};
  HWND desktop = newLayer(topLeftOf(endArea), sizeOf(endArea), true);

  LayeredWindowApi::instance().updateLayeredWindow(desktop, NULL, &topLeftOf(endArea), &mEndSize, ::GetDC(GetDesktopWindow()), &topLeftOf(endArea), NULL, &blend, ULW_ALPHA);
  //SetWindowPos(desktop, NULL, 0, 600, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
  
  
  Opacity targetOpacity(mOther, 0);
  SetWindowPos(mOther, NULL, endArea.left, endArea.top, sizeOf(endArea).cx, sizeOf(endArea).cy, SWP_NOZORDER | SWP_NOACTIVATE);
  UpdateWindow(mOther);

  //targetOpacity = 0;
  
  POINT dest = topLeftOf(rect);
  SIZE size = sizeOf(rect);
  POINT src = {0, sizeOf(endArea).cy-size.cy };
  
  if(left2RightExpand) {
    src.x = sizeOf(endArea).cx-size.cx;
  }

  blend.SourceConstantAlpha = (mFadeFrom*255)/100;
  for(int i = 0; i < steps; ++i) {
    
    if(left2RightExpand) {
      src.x -= deltaW;

    } else { 
      dest.x -= deltaW;
    }

    src.y -= deltaH;
    size.cx += deltaW;
    size.cy += deltaH;   
    blend.SourceConstantAlpha -= deltaAlpha;
    
    LayeredWindowApi::instance().updateLayeredWindow(desktop, NULL, &dest, &size, ::GetDC(mOther), &src, NULL, &blend, ULW_ALPHA);
  
    Sleep(ANIMATION_STEP_DURATION);
  }
  
  targetOpacity = mFadeTo;

  log(NO_LABEL, RENDERER, eDebug) << "moving window to final position " << endArea.left << "x" << endArea.top << endlog;
  SetWindowPos(mOther, NULL, endArea.left, endArea.top, sizeOf(endArea).cx, sizeOf(endArea).cy, SWP_NOZORDER | SWP_NOACTIVATE);
  DestroyWindow(desktop);
}

void LayeredWindow::animate(size_t time)
{
  Tracer trc(NO_LABEL, RENDERER, "LayeredWindow::animate()");

  RECT rect;
  GetWindowRect(mOther, &rect);

  SIZE mysize = sizeOf(rect);
  if(mysize.cx > mEndSize.cx || mysize.cy > mEndSize.cy) {
    collaps(time);
    
  } else if(mysize.cx < mEndSize.cx || mysize.cy < mEndSize.cy) {
    expand(time);
  }
  
}

SIZE LayeredWindow::sizeOf(RECT &area)
{
  SIZE size = { area.right-area.left, area.bottom-area.top };
  return size;
}

POINT LayeredWindow::topLeftOf(RECT &area)
{
  POINT topLeft = { area.left, area.top };
  return topLeft;
}

