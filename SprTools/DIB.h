#pragma once



class CDIB
{
public:
	CDIB(void);	
	virtual ~CDIB(void);

	enum { TOPDOWN,BOTTOMUP};
protected:
			
	BITMAPINFO	m_BI;					// DIB출력,DDB를 만들기 위한 멤버
	COLORREF	m_TransparentColor;	
	
	int			m_nWidth;				// 폭
	int			m_nHeight;				// 높이
	int			m_nPitch;				// 라인당 바이트수(4바이트정렬수)
	int			m_nBPP;					// 픽셀당 비트수

	BOOL		m_bMakeDDB;
	void*		m_pBitsBottomUp;		// BMP파일 로드시에만 사용 
	void*		m_pBitsTopDown;			// 화면 출력과 저장을 위해 사용

	HDC			m_hMemDC;				// DDB를 만들기 위한 멤버
	HBITMAP		m_hBitmap;				// DDB를 만들기 위한 멤버
	HBITMAP		m_hBitmapOld;

public:

	int					GetBPP()		{	return m_nBPP;		}
	int					GetWidth()		{	return m_nWidth;	}
	int					GetHeight()		{	return m_nHeight;	}
	int					GetPitch()		{	return m_nPitch;	}
	BITMAPINFO&			GetBI()			{	return m_BI;		}

	BOOL				IsNull()		{	return (m_pBitsTopDown==NULL ? TRUE : FALSE );  }

	
	BOOL		Load(LPCTSTR pszFileName);
	BOOL		Load(CDIB& SrcDIB,CRect& SrcRect);
	BOOL		Init(int nWidth,int nHeight,int nBPP );
	
	
	void		SetPixel(int x,int y,COLORREF color);
	COLORREF	GetRGB(int x,int y);
	COLORREF	GetRGB( CPoint point);
	COLORREF	GetPixel(int x,int y);
	COLORREF	GetPixel(CPoint point);
	void*		GetPixelAddress(int x,int y);
	void*		GetBitsBottomUp()		{ return m_pBitsBottomUp; }
	void*		GetBitsTopDown()	{ return m_pBitsTopDown; }	
	
	static int	ComputePitch( int nWidth, int nBPP )
	{
		return( (((nWidth*nBPP)+31)/32)*4 );
	}	
	COLORREF	GetTransparentColor() const { return m_TransparentColor; }
	void		SetTransparentColor(COLORREF val) { m_TransparentColor = val; }
	void		SetTransparentColorAuto();

	CRect		GetMaximizeRect( CPoint& SelectPoint );
	CRect		GetMinimizeRect( CRect& SelectRect );

	BOOL		StretchDIBits(HDC hdc,int xDest,int yDest,int DestWidth,int DestHeight,UINT iUsage,DWORD rop);

	BOOL		TransparentBlt(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int hHeightDest );
	BOOL		BitBlt(HDC hdcDest,int nXDest,int nYDest,int nWidth,int nHeight);
	
	
protected:
	int			MemAlloc(int pitch,int height);
	void		Uninit();
	void		ReArrangeBits();
	void		MakeDDB(HDC hDC);
};
