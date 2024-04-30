#include "StdAfx.h"
#include "DIB.h"



CDIB::CDIB(void)
{
	m_pBitsBottomUp=NULL;
	m_pBitsTopDown=NULL;	
	memset(&m_BI,0,sizeof(m_BI));	
	m_hBitmap=0;
	m_bMakeDDB=FALSE;
}

CDIB::~CDIB(void)
{
	Uninit();
}

int CDIB::MemAlloc(int pitch,int height)
{
	int sizebyte;
	sizebyte=pitch * height;
	m_pBitsBottomUp = new char[sizebyte];
	m_pBitsTopDown = new char[sizebyte];
	return sizebyte;
	
}

void CDIB::Uninit()
{
	::SelectObject(m_hMemDC,m_hBitmapOld);
	::DeleteObject(m_hBitmap);
	::DeleteDC(m_hMemDC);
	m_bMakeDDB=FALSE;
	if (m_pBitsBottomUp!=NULL)
	{
		delete []m_pBitsBottomUp;
	}
	if (m_pBitsTopDown!=NULL)
	{
		delete []m_pBitsTopDown;
	}
}

BOOL CDIB::Load( LPCTSTR pszFileName )
{
	Uninit();

	CFile file;

	if (!file.Open(pszFileName,CFile::modeReadWrite))
	{
		return FALSE;
	}

	BITMAPFILEHEADER BFH;
	if ( file.Read(&BFH, sizeof(BFH))!= sizeof(BFH))
	{
		::AfxMessageBox(_T("BMPFILEHEADER를 읽을수 없습니다."));
		return FALSE;
	}
	if (BFH.bfType != 0x4d42)
	{
		::AfxMessageBox(_T("비트맵 파일이 아닙니다."));
		return FALSE;
	}

	if ( file.Read(&m_BI, sizeof(m_BI))!= sizeof(m_BI))
	{
		::AfxMessageBox(_T("BMPINFOHEADER를 읽을수 없습니다."));
		return FALSE;
	}
	m_nBPP=m_BI.bmiHeader.biBitCount;	
	m_nWidth=m_BI.bmiHeader.biWidth;
	m_nHeight=m_BI.bmiHeader.biHeight;
	m_BI.bmiHeader.biHeight = -1*m_nHeight;
	m_nPitch=ComputePitch(m_nWidth,m_nBPP);
	
	
	if (m_nBPP != 24 )
	{
		::AfxMessageBox(_T("24비트 비트맵만 처리 가능합니다."));
		return FALSE;
	}
	int sizebyte;
	
	sizebyte=MemAlloc(m_nPitch,m_nHeight);	// 메모리 할당 
	file.Seek(BFH.bfOffBits,CFile::begin);	
	if ( file.Read(m_pBitsBottomUp,sizebyte)!= sizebyte)
	{
		::AfxMessageBox(_T("비트맵 비트 데이터의 크기가 다릅니다."));
		return FALSE;
	}
	ReArrangeBits();							// 픽셀데이터 재배치 	
	SetTransparentColorAuto();
	file.Close();
	return TRUE;	
}

BOOL CDIB::Load(CDIB& SrcDIB,CRect& SrcRect)
{
	Uninit();
	memcpy(&m_BI,&SrcDIB.GetBI(),sizeof(m_BI));
	m_BI.bmiHeader.biWidth=SrcRect.Width();
	m_BI.bmiHeader.biHeight=SrcRect.Height();

	m_nBPP=m_BI.bmiHeader.biBitCount;	
	m_nWidth=m_BI.bmiHeader.biWidth;
	m_nHeight=m_BI.bmiHeader.biHeight;
	m_BI.bmiHeader.biHeight = -1*m_nHeight;

	m_nPitch=ComputePitch(m_nWidth,m_nBPP);	
	MemAlloc(m_nPitch,m_nHeight);

	for (int y=0;y<m_nHeight;y++)
	{
		PBYTE src,dst;		
		src=(PBYTE)SrcDIB.GetPixelAddress(SrcRect.left,SrcRect.top+y);
		dst=(PBYTE)GetPixelAddress(0,y);
		memcpy( dst ,src,m_nPitch);
	}
	m_TransparentColor=SrcDIB.GetTransparentColor();
	return TRUE;
}

BOOL CDIB::Init(int nWidth,int nHeight,int nBPP )
{
	Uninit();	
	m_BI.bmiHeader.biBitCount		=nBPP;
	m_BI.bmiHeader.biClrImportant	=0;
	m_BI.bmiHeader.biClrUsed		=0;
	m_BI.bmiHeader.biCompression	=BI_RGB;
	m_BI.bmiHeader.biHeight			=nHeight;
	m_BI.bmiHeader.biPlanes			=1;
	m_BI.bmiHeader.biSize			=sizeof(BITMAPINFOHEADER);
	m_BI.bmiHeader.biSizeImage		=nWidth*nHeight;
	m_BI.bmiHeader.biWidth			=nWidth;
	m_BI.bmiHeader.biXPelsPerMeter	=0;
	m_BI.bmiHeader.biYPelsPerMeter	=0;

	m_nBPP=m_BI.bmiHeader.biBitCount;	
	m_nWidth=m_BI.bmiHeader.biWidth;
	m_nHeight=m_BI.bmiHeader.biHeight;
	m_BI.bmiHeader.biHeight = -1*m_nHeight;

	m_nPitch=ComputePitch(m_nWidth,m_nBPP);
	MemAlloc(m_nPitch,m_nHeight);

	return TRUE;
}
BOOL CDIB::StretchDIBits( HDC hdc,int xDest,int yDest,int DestWidth,int DestHeight,UINT iUsage,DWORD rop )
{	
	return ::StretchDIBits(hdc,xDest,yDest,DestWidth,DestHeight,
		0,0,m_nWidth,m_nHeight,m_pBitsTopDown,&m_BI,iUsage,rop);
}

COLORREF CDIB::GetPixel( int x,int y )
{
	COLORREF ret=0;
	void* pBits;

	pBits=GetPixelAddress(x,y);
	if (m_nBPP==24)
	{
		memcpy(&ret,pBits,3);					
	}
	else if (m_nBPP==32)
	{
		memcpy(&ret,pBits,4);	
	}	
	return ret;
}

void CDIB::SetPixel(int x,int y,COLORREF color)
{
	void* pBits;

	pBits=GetPixelAddress(x,y);
	if (m_nBPP==24)
	{
		memcpy(pBits,&color,3);					
	}
	else if (m_nBPP==32)
	{
		memcpy(pBits,&color,4);	
	}	
}

COLORREF CDIB::GetPixel( CPoint point )
{
	COLORREF ret=0;
	void* pBits;

	pBits=GetPixelAddress(point.x,point.y);
	if (m_nBPP==24)
	{
		memcpy(&ret,pBits,3);					
	}
	else if (m_nBPP==32)
	{
		memcpy(&ret,pBits,4);	
	}	
	return ret;
}

void* CDIB::GetPixelAddress( int x,int y )
{

	_ASSERT( (x >= 0) && (x < m_nWidth) );
	_ASSERT( (y >= 0) && (y <  m_nHeight) );

	return( LPBYTE( m_pBitsTopDown )+(y*m_nPitch)+((x*m_nBPP)/8) );

}

void CDIB::ReArrangeBits()
{
	int temp;
	for (int y=0;y<m_nHeight;y++)
	{
		temp=m_nHeight-1-y;
		memcpy(GetPixelAddress(0,y),(PBYTE)m_pBitsBottomUp+m_nPitch*temp ,m_nPitch);
	}
}



void CDIB::SetTransparentColorAuto()
{
	// 4귀퉁이 검사후 투명값으로 설정	
	if(	(GetRGB(0,0)==GetRGB(m_nWidth-1,0))  ||
		(GetRGB(0,0)==GetRGB(0,m_nHeight-1)) ||		
		(GetRGB(m_nWidth-1,0)==GetRGB(m_nWidth-1,m_nHeight-1)) ||		
		(GetRGB(0,m_nHeight-1) == GetRGB(m_nWidth-1,m_nHeight-1)) ) 
	{
		SetTransparentColor(GetRGB(0,0));
	}
}

void CDIB::MakeDDB(HDC hDC)
{	
	m_hBitmap=::CreateDIBitmap(hDC,&m_BI.bmiHeader,CBM_INIT,m_pBitsTopDown,&m_BI,DIB_RGB_COLORS);
	m_hMemDC =::CreateCompatibleDC( hDC );
	m_hBitmapOld=(HBITMAP)::SelectObject(m_hMemDC,m_hBitmap);
	m_bMakeDDB=TRUE;	
}

BOOL CDIB::TransparentBlt( HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int hHeightDest )
{
	if (!m_bMakeDDB)
	{
		MakeDDB(hdcDest);
	}
	return ::TransparentBlt(hdcDest,nXOriginDest,nYOriginDest,nWidthDest,hHeightDest,
		m_hMemDC,0,0,m_nWidth,m_nHeight,m_TransparentColor);
}


BOOL CDIB::BitBlt( HDC hdcDest,int nXDest,int nYDest,int nWidth,int nHeight )
{
	if (!m_bMakeDDB)
	{
		MakeDDB(hdcDest);
	}
	return ::BitBlt(hdcDest,nXDest,nYDest,nWidth,nHeight,m_hMemDC,0,0,SRCCOPY);
}

CRect CDIB::GetMaximizeRect( CPoint& SelectPoint )
{	
	CRect rtRet(0,0,0,0);
	CPoint ptTemp;
	COLORREF color;
	BOOL bLExpand=FALSE,bRExpand=FALSE,bUExpend=FALSE,bDExpand=FALSE;

	//1. 해당 픽셀 검사
	color=GetRGB(SelectPoint.x,SelectPoint.y);
	if (color==m_TransparentColor)
	{
		return rtRet;
	}

	//2. POINT기준 1차 박스 확장
	// Left
	ptTemp=SelectPoint;	
	while(ptTemp.x > 0)	
	{
		ptTemp.x--;
		if (GetRGB(ptTemp)==m_TransparentColor)
		{
			rtRet.left=ptTemp.x;
			break;
		}
	} 
	// Right
	ptTemp=SelectPoint;	
	while(ptTemp.x < m_nWidth-1)
	{
		ptTemp.x++;
		if (GetRGB(ptTemp)==m_TransparentColor)
		{
			rtRet.right=ptTemp.x;
			break;
		}
	} 

	// Up
	ptTemp=SelectPoint;	
	while(ptTemp.y > 0)
	{
		ptTemp.y--;
		if (GetRGB(ptTemp)==m_TransparentColor)
		{
			rtRet.top=ptTemp.y;
			break;
		}
	} 

	// Down
	ptTemp=SelectPoint;	
	while(ptTemp.y < m_nHeight-1)
	{
		ptTemp.y++;
		if (GetRGB(ptTemp)==m_TransparentColor)
		{
			rtRet.bottom=ptTemp.y;
			break;
		}
	} 



	// 3. RECT기준 2차 박스 확장
	int x=0,y=0;
	int countRetry=0;
	do 
	{

		bLExpand=FALSE;
		bRExpand=FALSE;
		bUExpend=FALSE;
		bDExpand=FALSE;
		//LEFT 확장
		for (y=rtRet.top;y<=rtRet.bottom;y++)
		{	
			ptTemp=CPoint(rtRet.left,y);		
			do 
			{				
				color=GetRGB(ptTemp);
				if (color!=m_TransparentColor)
				{
					bLExpand=TRUE;
					ptTemp.x--;					
				}
				else
				{
					rtRet.left=min(ptTemp.x,rtRet.left);
					break;
				}				
			} while(ptTemp.x > 0);				
		}

		//RIGHT 확장
		for (y=rtRet.top;y<=rtRet.bottom;y++)
		{
			ptTemp=CPoint(rtRet.right,y);		
			do 
			{				
				if (GetRGB(ptTemp)!=m_TransparentColor)
				{
					bRExpand=TRUE;
					ptTemp.x++;
				}
				else 
				{					
					rtRet.right=max(ptTemp.x,rtRet.right);
					break;
				}
			} while(ptTemp.x < m_nWidth-1);
		}


		//UP 확장
		for (x=rtRet.left;x<=rtRet.right;x++)
		{	
			ptTemp=CPoint(x,rtRet.top);		
			do 
			{				
				if (GetRGB(ptTemp)!=m_TransparentColor)
				{
					bUExpend=TRUE;
					ptTemp.y--;
				}
				else 
				{					
					rtRet.top=min(ptTemp.y,rtRet.top);
					break;
				}
			} while(ptTemp.y > 0);
		}

		//DOWN 확장
		for (x=rtRet.left;x<=rtRet.right;x++)
		{		
			ptTemp=CPoint(x,rtRet.bottom);		
			do 
			{				
				if (GetRGB(ptTemp)!=m_TransparentColor)
				{				
					bDExpand=TRUE;
					ptTemp.y++;
				}
				else 
				{
					rtRet.bottom=max(ptTemp.y,rtRet.bottom);
					break;	
				}
			} while(ptTemp.y < m_nHeight-1);
		}

		// 무한루프 방지를 위한 재시도 횟수 설정 
		countRetry++;
		if (countRetry>=30)
		{
			break;
		}

	} while( (bLExpand)||(bRExpand)||(bUExpend)||(bDExpand) );

//	rtRet.left;
//	rtRet.top;
	rtRet.right--;
	rtRet.bottom--;


	TRACE(_T("L%d,T%d,R%d,B%d\n"),rtRet.left,rtRet.top,rtRet.right,rtRet.bottom);
	return rtRet;

}

CRect CDIB::GetMinimizeRect( CRect& SelectRect )
{
	CRect		rtRet;	
	CPoint		ptTemp;
	COLORREF	color;
	BOOL		bFind=FALSE;
	int x,y;

	// 드래그 방향에 따른 Rect보정
	rtRet.left=min(SelectRect.left,SelectRect.right);
	rtRet.right=max(SelectRect.left,SelectRect.right);
	rtRet.top=min(SelectRect.top,SelectRect.bottom);
	rtRet.bottom=max(SelectRect.top,SelectRect.bottom);
	
	// SetCapture에 의해 음수or이미지 크기 벗어나는 RECT보정
	rtRet.left=max(0,rtRet.left);
	rtRet.right=min(m_nWidth-1,rtRet.right);
	rtRet.top=max(0,rtRet.top);
	rtRet.bottom=min(m_nHeight-1,rtRet.bottom);
	



	//LEFT Edge 축소
	bFind=FALSE;
	for (x=rtRet.left;x<=rtRet.right;x++)
	{	
		ptTemp=CPoint(x,rtRet.top);		
		do 
		{				
			color=GetRGB(ptTemp);
			if (color==m_TransparentColor)
			{				
				ptTemp.y++;						
			}
			else
			{				
				bFind=TRUE;
				break;
			}				
		} while(ptTemp.y <= rtRet.bottom);
		if (bFind)
		{
			break;
		}
	}
//	TRACE("LEFT %d\n",x);
	rtRet.left=x;

	//RIGHT Edge축소 
	bFind=FALSE;
	for (x=rtRet.right;x>=rtRet.left;x--)
	{
		ptTemp=CPoint(x,rtRet.top);		
		do 
		{				
			color=GetRGB(ptTemp);
			if (color==m_TransparentColor)
			{				
				ptTemp.y++;					

			}
			else
			{	
				bFind=TRUE;
				break;
			}				
		} while(ptTemp.y <= rtRet.bottom);
		if (bFind)
		{
			break;
		}
	}
//	TRACE("RIGHT %d\n",x);
	rtRet.right=x;

	//UP Edge 축소 
	bFind=FALSE;
	for (y=rtRet.top;y<=rtRet.bottom;y++)
	{	
		ptTemp=CPoint(rtRet.left,y);		
		do 
		{				
			color=GetRGB(ptTemp);
			if (color==m_TransparentColor)
			{				
				ptTemp.x++;
			}
			else 
			{					
				bFind=TRUE;
				break;
			}
		} while(ptTemp.x <= rtRet.right);
		if (bFind)
		{
			break;
		}
	}
//	TRACE("TOP %d\n",y);
	rtRet.top=y;

	//DOWN Edge 축소
	bFind=FALSE;
	for (y=rtRet.bottom;y>=rtRet.top;y--)
	{		
		ptTemp=CPoint(rtRet.left,y);			
		do 
		{	
			color=GetRGB(ptTemp);
			if (color==m_TransparentColor)
			{				
				ptTemp.x++;
			}
			else 
			{					
				bFind=TRUE;
				break;
			}
		} while(ptTemp.x <= rtRet.right);
		if (bFind)
		{
			break;
		}
	}
//	TRACE("BOTTOM %d\n",y);
	rtRet.bottom=y;

	if (bFind==FALSE)
	{
		rtRet.SetRectEmpty();
	}

	TRACE(_T("L%d,T%d,R%d,B%d\n"),rtRet.left,rtRet.top,rtRet.right,rtRet.bottom);

	return rtRet;
}

COLORREF CDIB::GetRGB( int x,int y )
{
	COLORREF ret=0;
	DWORD	temp;
	void* pBits;

	pBits=GetPixelAddress(x,y);
	if (m_nBPP==24)
	{
		memcpy(&temp,pBits,3);			
	}
	else if (m_nBPP==32)
	{
		memcpy(&temp,pBits,4);	
		
	}	
	ret	 =(temp&0xff0000)>>16;
	ret	|=(temp&0x00ff00);
	ret	|=(temp&0x0000ff)<<16;
	return ret;
}

COLORREF CDIB::GetRGB( CPoint point)
{
	COLORREF ret=0;
	DWORD	temp;
	void* pBits;

	pBits=GetPixelAddress(point.x,point.y);
	if (m_nBPP==24)
	{
		memcpy(&temp,pBits,3);		
		ret	 =(temp&0xff0000)>>16;
		ret	|=(temp&0x00ff00);
		ret	|=(temp&0x0000ff)<<16;
	}
	else if (m_nBPP==32)
	{
		memcpy(&temp,pBits,4);	
	}	
	return ret;
}