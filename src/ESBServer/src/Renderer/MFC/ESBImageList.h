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

#ifndef OutlineIconList_H
#define OutlineIconList_H

#include <Renderer/IconBroker.h>
#include <map>


typedef size_t Decorators;

class ESBImageList : public CImageList, public IconBroker<int>
{
private:
  typedef IconBroker<int> super;

public:
  static ESBImageList& instance();

private:
  ESBImageList();

  void initialize();
	int createIcon(const IconDescriptor&);

  void renderAccessDecorator(const IconDescriptor&, const CSize&, CDC&);
  void renderMiscDecorators(const IconDescriptor&, CPoint, CDC&);
  CPoint renderDecorator(const std::string &file, const CPoint &offset, CDC &painter);
  //CPoint renderDecorator(const std::string &file, const CPoint &offset, CDC &painter, const COLORREF &transparency);

  CSize getImageDimension();
};

class Bitmap : public CBitmap
{
public:
  Bitmap() : CBitmap()
  {}

  Bitmap(const std::string &filename) : CBitmap() 
  { load(filename); }

  Bitmap(UINT resourceID) : CBitmap() 
  { LoadBitmap(resourceID); }

  //default dtor is enough... detach is done by CGDIObject

  CSize getDimension()
  {
    BITMAP bmp;
    GetBitmap(&bmp);
    return CSize(bmp.bmWidth, bmp.bmHeight);
  }

  void load(const std::string &filename)
  {
    HBITMAP hBitmap = (HBITMAP) LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
    Attach(hBitmap);
  }

  void render(CDC &painter, CPoint &pos, COLORREF transparentColor)
  {
    CSize size = getDimension();
    CImageList tmp;
    tmp.Create(size.cx, size.cy, ILC_COLOR8|ILC_MASK, 1, 1);
    tmp.SetBkColor(CLR_NONE);
    int img = tmp.Add(this, transparentColor);
    tmp.Draw(&painter, img, pos, ILD_NORMAL);
  }

  void render(CDC &painter, CPoint &pos)
  {
    painter.DrawState(pos, getDimension(), *this, DST_BITMAP);
  }
};

#endif
