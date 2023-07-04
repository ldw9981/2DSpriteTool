#pragma once
#include "afxwin.h"

class CMemDC :
	public CDC
{
public:
	CMemDC(void);
	~CMemDC(void);

	BOOL Init(CDC* pDC,int width,int height);
	void Uninit();
	void ClearBlack();
	void ClearWhite();

	int GetWidth() const { return m_nWidth; }
	void SetWidth(int val) { m_nWidth = val; }
	int GetHeight() const { return m_nHeight; }
	void SetHeight(int val) { m_nHeight = val; }
protected:
	CBitmap	 m_Bitmap;
	CBitmap* m_pBitmapOld;
	BOOL	 m_bDelete;
	int		 m_nWidth;
	int		 m_nHeight;
	
};
