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
#include "WorkspaceBrowserRenderer.h"

#include <Renderer/MFC/3rdParty/Dib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WorkspaceBrowserRenderer::WorkspaceBrowserRenderer()
: super(0)
{
	mWorkspaceBrowserRendererImages.Create(16, 16, ILC_COLOR8, 2, 1);
  CDib bmp;
	bmp.Load("res/headerfile256.bmp");
	mWorkspaceBrowserRendererImages.Add(&bmp,RGB(0,255,255));

	bmp.Load("res/sourcefile256.bmp");
	mWorkspaceBrowserRendererImages.Add(&bmp,RGB(0,255,255));

	bmp.Load("res/workspace256.bmp");
	mWorkspaceBrowserRendererImages.Add(&bmp,RGB(0,255,255));

  setImages(mWorkspaceBrowserRendererImages);
  setCaption("Select File");
  setIcon(IDI_WORKSPACE);
  setFilterDescription("Choose a file ( ? = any character, * = any string)");
}
