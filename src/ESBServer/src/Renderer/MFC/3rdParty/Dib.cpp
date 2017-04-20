#include "StdAfx.h"
#include "Dib.h"
 

IMPLEMENT_DYNAMIC(CDib, CBitmap)

CDib::CDib()
{
	m_pbmih = NULL;
	m_hdd = NULL;
}

CDib::~CDib()
{
	DeleteObject();
}

void CDib::DeleteObject()

{
	if (m_hdd)
	{
		DrawDibClose(m_hdd);
		m_hdd = NULL;
	}

	if (m_pbmih)
	{
		delete [] (char*)m_pbmih;
		m_pbmih = NULL;
	}
}

UINT CDib::GetNumPaletteColors()
{
	// Calculate # entries in color table:
	// if biClrUsed is not specified, then use:
	// (2,16,256) for (1,4,8)-bit bitmaps;
	// 0 for 24, 32-bit bitmaps (no color table)

	UINT nColors=m_pbmih->biClrUsed;
	if (nColors==0 && m_pbmih->biBitCount<=8)
		nColors = 1<<m_pbmih->biBitCount;

	return nColors;
}

LPBYTE CDib::GetBits()
{
	return (LPBYTE)m_pbmih + m_pbmih->biSize + GetNumPaletteColors()*sizeof(RGBQUAD);
}

const BITMAP_TYPE = (WORD)('M' << 8) | 'B';

BOOL CDib::Load(LPCTSTR szPathName)
{
	if( m_hObject )
		Detach();
	
	if( Attach(::LoadImage(NULL, szPathName,IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE)) )
	{
		CFile file;
		BOOL bRet = FALSE;

		file.Open( szPathName, CFile::modeRead );
		if( Load( file )==TRUE )
			bRet = CreatePalette();
		file.Close();
		return bRet;
	}

	return FALSE;
}

BOOL CDib::Load(CFile &file)
{
	BITMAPFILEHEADER hdr;
	DWORD len = file.Read(&hdr, sizeof(hdr));

	if ((len!=sizeof(hdr)) || (hdr.bfType!=BITMAP_TYPE))
	{
		TRACE0("***CDib: bad BITMAPFILEHEADER\n");
		return FALSE;
	}

	len = file.GetLength() - len;
	m_pbmih = (BITMAPINFOHEADER*)new char[len];
	file.Read(m_pbmih, len);

	return TRUE;
}

BOOL CDib::Draw(CDC& dc, const CRect* rcDst, const CRect* rcSrc)
{
	if (!m_pbmih)
		return FALSE;

	if (!m_hdd)
		VERIFY(m_hdd = DrawDibOpen());

	CRect rc;

	if (!rcSrc)
	{
		// if no source rect, use whole bitmap
		rc.SetRect(0, 0, m_pbmih->biWidth, m_pbmih->biHeight);
		rcSrc=&rc;
	}

	if (!rcDst)
	{
		// if no destination rect, use source
		rcDst=rcSrc;
	}

	// This is as easy as it gets in Windows.
	return DrawDibDraw(m_hdd, dc, rcDst->left, rcDst->top, rcDst->Width(), rcDst->Height(), m_pbmih, GetBits(), rcSrc->left, rcSrc->top, rcSrc->Width(), rcSrc->Height(), 0);
}

BOOL CDib::Draw(CDC *pDC)
{
	if( m_hObject )
	{
		CPalette* pOldPal = pDC->SelectPalette(CPalette::FromHandle(m_pal),FALSE);

		pDC->RealizePalette();

		BOOL bRet = DrawBitmap(*pDC, this); // as before

		pDC->SelectPalette(pOldPal, TRUE);

		return bRet;
	}

	return FALSE;
}

BOOL CDib::CreatePalette()
{
	int	i = 0;
	int nColors = GetNumPaletteColors();
	CDC *pDC = CDC::FromHandle( GetDC(AfxGetApp()->m_pMainWnd->GetSafeHwnd()) );
	CDC mdc; // memory DC
	RGBQUAD *pRgbQuad = NULL;
	LPLOGPALETTE lpPal;
	HANDLE              hLogPal;
	CBitmap* pOld = NULL;

	pRgbQuad = new RGBQUAD[nColors];
	mdc.CreateCompatibleDC(pDC);
	pOld = mdc.SelectObject(this);
	GetDIBColorTable( mdc.GetSafeHdc(), 0, nColors, pRgbQuad );

	if( nColors )
	{
		hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
							 sizeof (PALETTEENTRY) * nColors);
		lpPal = (LPLOGPALETTE) GlobalLock (hLogPal);
		lpPal->palVersion    = 0x300;
		lpPal->palNumEntries = nColors;

		for (i = 0;  i < nColors;  i++)
		{
			lpPal->palPalEntry[i].peRed   = pRgbQuad[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = pRgbQuad[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue  = pRgbQuad[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		m_pal = ::CreatePalette( lpPal );

		GlobalUnlock(hLogPal);
		GlobalFree(hLogPal);
	}

	delete []pRgbQuad;

	ReleaseDC( AfxGetApp()->m_pMainWnd->GetSafeHwnd(), pDC->GetSafeHdc() );

	return TRUE;
}

BOOL DrawBitmap(CDC& dc, CBitmap* pbm)
{
	CDC mdc; // memory DC
	BITMAP bm;

	mdc.CreateCompatibleDC(&dc);

	CBitmap* pOld = mdc.SelectObject(pbm);
	pbm->GetObject(sizeof(bm), &bm);

	BOOL bRet = dc.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(pOld);

	return bRet;
}
