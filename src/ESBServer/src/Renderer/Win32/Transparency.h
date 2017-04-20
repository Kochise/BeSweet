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

#ifndef Transparency_H
#define Transparency_H

class LayeredWindowApi
{
private:
  typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
  static lpfnSetLayeredWindowAttributes SetLayeredWindowAttributes;

  typedef BOOL (WINAPI *lpfnGetLayeredWindowAttributes)(HWND hWnd, COLORREF *crKey, BYTE *bAlpha, DWORD *dwFlags);
  static lpfnGetLayeredWindowAttributes GetLayeredWindowAttributes;

  typedef BOOL (WINAPI *lpfnUpdateLayeredWindow)(HWND hwnd, HDC hdcDst,POINT *pptDst,SIZE *psize, HDC hdcSrc, POINT *psrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
  static lpfnUpdateLayeredWindow UpdateLayeredWindow ;

public:  
  static LayeredWindowApi& instance();

  void updateLayeredWindow(
                HWND hwnd,             // handle to layered window
                HDC hdcDst,            // handle to screen DC
                POINT *pptDst,         // new screen position
                SIZE *psize,           // new size of the layered window
                HDC hdcSrc,            // handle to surface DC
                POINT *psrc,         // layer position
                COLORREF crKey,        // color key
                BLENDFUNCTION *pblend, // blend function
                DWORD dwFlags          // options
              );

  void setLayeredWindowAttributes(HWND wnd, COLORREF color, BYTE alpha, DWORD flags);
  
  void getLayeredWindowAttributes(HWND wnd, COLORREF *color, BYTE *alpha, DWORD *flags);

private:
  LayeredWindowApi();
};


class Opacity
{
public:
  size_t minimum;
  size_t maximum;

private:
  HWND mWnd;
  size_t mOpacityLevel;
  
public:
  Opacity() : maximum(100), minimum(50) {}
  Opacity(HWND wnd, size_t level);

  size_t operator=(size_t level)
  { set(level); return get(); }
  
  operator size_t() const { return get(); }

  void set(size_t level) ;
  size_t get() const { return mOpacityLevel; };

private:
  
};

class LayeredWindow
{
private:
  HWND mOther;
  HWND mSelf;

  POINT mEndDest;
  SIZE  mEndSize;

  Opacity mOpacity;
  size_t mFadeFrom;
  size_t mFadeTo;

public:
  LayeredWindow(HWND wndBelow, bool = true);
  //LayeredWindow(const RECT &area);
  //LayeredWindow(const POINT &topLeft, const SIZE &size);
  ~LayeredWindow();
  
  void takeScreenShot();

  void setHorizontalAnimation(SIZE size);
  void setHorizontalAnimation(POINT dest, SIZE size);
  void setFadeAnimation(size_t from, size_t to);

  void animate(size_t time);
  
  operator HWND() { return mSelf; }

private:
  void collaps(size_t time);
  void expand(size_t time);

  void initialize(const POINT &topLeft, const SIZE &size, bool);
  HWND newLayer(const POINT &topLeft, const SIZE &size, bool);
  bool isInRightScreenHalf();

  void reinitialize();

  SIZE sizeOf(RECT &);
  POINT topLeftOf(RECT&);

};

#endif