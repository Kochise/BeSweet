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

#include "ESBImageList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
const COLORREF TRANSPARENCY_COLOR = RGB(255,0,255); //white



ESBImageList::ESBImageList()
{
  Create(16, 16, ILC_COLOR8 | ILC_MASK, 5, 1);
  SetBkColor(CLR_NONE);

  initialize();
}

void ESBImageList::initialize()
{
	struct XX
  { string file; MetaObjectType type; XX(const string& f, MetaObjectType t) : file(f), type(t) {}};
  
  const size_t size = 10;
	XX images[size]= {
		XX("res/namespace256.bmp", eNamespace), 
		XX("res/class256.bmp", eClass), XX("res/struct256.bmp", eStruct), XX("res/union256.bmp", eUnion),
		XX("res/enum256.bmp", eEnum), XX("res/typedef256.bmp", eTypedef),
		XX("res/function256.bmp", eFunction), XX("res/method256.bmp", eMethod),  
    XX("res/field256.bmp", eField), XX("res/variable256.bmp", eVariable)
											};

  IconMap icons;
	for(int i = 0; i < size; i++) {
		Bitmap image(images[i].file);
    icons[images[i].type][0] = Add(&image, TRANSPARENCY_COLOR); 
	}

  super::initialize(icons);
}

ESBImageList& ESBImageList::instance()
{
  static ESBImageList theInstance;  
  return theInstance;
}


int ESBImageList::createIcon(const IconDescriptor &icon)
{
  CDC *winDC = AfxGetApp()->GetMainWnd()->GetDC();
  
  CDC virtualPainter;
  virtualPainter.CreateCompatibleDC(winDC); 
  
  CSize dim = getImageDimension();
  Bitmap result;
  result.CreateBitmap(dim.cx, dim.cy, virtualPainter.GetDeviceCaps(PLANES), virtualPainter.GetDeviceCaps(BITSPIXEL),0);
  CBitmap* oldBmp = virtualPainter.SelectObject(&result);
  
  //draw the main icon/bitmap
  Draw(&virtualPainter, getIcon(icon.type), CPoint(0,0), ILD_NORMAL);
  
  renderAccessDecorator(icon, result.getDimension(), virtualPainter);
  renderMiscDecorators(icon, CPoint(result.getDimension().cx, 0), virtualPainter);
  
  virtualPainter.SelectObject(oldBmp);
  
  winDC->ReleaseOutputDC();
  return Add(&result, RGB(0,0,0));
}

void ESBImageList::renderAccessDecorator(const IconDescriptor &icon, const CSize& size, CDC &painter)
{  
  if(!icon.decorators.hasAccessDecorator) {
    return;
  }

  string prefix;
  switch(icon.decorators.accessDecorator) {
    case ePublic : prefix = "public"; break;
    case eProtected: prefix = "protected"; break;
    case ePrivate: prefix = "private"; break;
  }
  
  string postfix;
  switch(icon.type) {
    case eField : postfix = "_field"; break;
    case eMethod : postfix = "_method";break;
  }
  
  CPoint topRight(size.cx, 0);
  if(icon.type == eMethod || icon.type == eFunction || icon.type == eField) {
    topRight += CPoint(-8, 5);

  } else {
    topRight.y += 9;
  }
    
  renderDecorator("res\\" + prefix + postfix + "256.bmp", topRight, painter);
}

void ESBImageList::renderMiscDecorators(const IconDescriptor &icon, CPoint offset, CDC &painter)
{  
  switch(icon.decorators.implementationDecorator) {
    case eVirtual : offset = renderDecorator("res\\virtual256.bmp", offset, painter); break;
    case eAbstract : offset = renderDecorator("res\\abstract256.bmp", offset, painter); break;
  }
  
  if(icon.decorators.isSelected) {
    CPoint p(5, 4);
    renderDecorator("res\\selection_arrow256.bmp", p, painter);
  }
  
  if(icon.decorators.hasConstDecorator) {
    offset = renderDecorator("res\\const256.bmp", offset, painter);
  }
  
  if(icon.decorators.hasStaticDecorator) {
    offset = renderDecorator("res\\static256.bmp", offset, painter);
  }

  if(icon.decorators.hasQuestion) {
    offset = renderDecorator("res\\questionmark256.bmp", offset, painter);
  }
  
  if(icon.decorators.hasProblem) {
    offset = renderDecorator("res\\problem256.bmp", offset, painter);
  }

  if(icon.decorators.isPrototype) {
    offset = renderDecorator("res\\prototype256.bmp", offset, painter);
  }
}

//CPoint ESBImageList::renderDecorator(const std::string &file, const CPoint &offset, CDC &painter)
//{
//  return renderDecorator(file, offset, painter, TRANSPARENCY_COLOR);
//}

CPoint ESBImageList::renderDecorator(const std::string &file, const CPoint &offset, CDC &painter/*, const COLORREF &transparency*/)
{
  COLORREF transparency = TRANSPARENCY_COLOR;
  
  Bitmap decorator(file);

  CPoint size = decorator.getDimension();
  CPoint topleft = CPoint(offset.x - size.x, offset.y);
  
  CRect dest(topleft, topleft+size);
  
  decorator.render(painter, topleft, transparency);
  
  const size_t space = 2;
  return CPoint(offset.x - size.x - space, offset.y);
}

CSize ESBImageList::getImageDimension()
{
  IMAGEINFO info;
  GetImageInfo(0, &info);
  return CRect(info.rcImage).Size();
}


