#ifndef _DIB_H_
#define _DIB_H_

#include "vfw.h"

class CDib : public CBitmap
{
	DECLARE_DYNCREATE(CDib)

	public:
		CDib();
		~CDib();
		BOOL Load(LPCTSTR szPathName);
		BOOL Load(CFile &file);
		BOOL Draw(CDC& dc, const CRect* rcDst, const CRect* rcSrc);
		BOOL Draw(CDC* pDC);

	protected:
		HPALETTE m_pal;
		BITMAPINFOHEADER *m_pbmih;
		HDRAWDIB m_hdd;

		void DeleteObject();
		UINT GetNumPaletteColors();
		LPBYTE GetBits();
		BOOL CreatePalette();
};

BOOL DrawBitmap(CDC& dc, CBitmap* pbm);

#endif



