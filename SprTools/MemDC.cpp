#include "StdAfx.h"
#include "MemDC.h"

CMemDC::CMemDC(void)
{
	m_pBitmapOld=NULL;
	m_bDelete=TRUE;
	m_nHeight=0;
	m_nWidth=0;
}

CMemDC::~CMemDC(void)
{
	if (!m_bDelete)
	{
		Uninit();
	}	
}

BOOL CMemDC::Init( CDC* pDC,int width,int height )
{
	if(!m_Bitmap.CreateCompatibleBitmap(pDC,width,height))
	{
		return FALSE;
	}

	if (!CreateCompatibleDC(pDC))
	{
		return FALSE;
	}		
	
	m_pBitmapOld=SelectObject(&m_Bitmap);
	m_bDelete=FALSE;
	m_nWidth=width;
	m_nHeight=height;
	return TRUE;
}

void CMemDC::Uninit()
{
	if (!m_bDelete)
	{	
		m_bDelete=TRUE;
		SelectObject(m_pBitmapOld);
		DeleteDC();	
		m_Bitmap.DeleteObject();
	}	
}

void CMemDC::ClearBlack()
{
	PatBlt(0,0,m_nWidth,m_nHeight,BLACKNESS);
}

void CMemDC::ClearWhite()
{
	PatBlt(0,0,m_nWidth,m_nHeight,WHITENESS);
}