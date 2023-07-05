#include "StdAfx.h"
#include "Frame.h"

CFrame::CFrame(void)
{
	Init();
}

/*
CFrame::CFrame(int nBPP,CFile& File )
{
	File.Read(&m_Info,sizeof(m_Info));
	CDIB::Load();
}
*/
CFrame::CFrame( CDIB& SrcDIB,CRect& SrcRect )
{
	Init();
	CDIB::Load(SrcDIB,SrcRect);
	m_FrameHeader.m_Width=SrcRect.Width();
	m_FrameHeader.m_Height=SrcRect.Height();
	
	// Text����� ���� ����		SrcRect�� �߸��� ������� ������.
	m_FrameHeader.m_SourceLeft = SrcRect.left+1;
	m_FrameHeader.m_SourceTop = SrcRect.top+1;
	m_FrameHeader.m_SourceRight = SrcRect.right;
	m_FrameHeader.m_SourceBottom = SrcRect.bottom;
}
CFrame::~CFrame(void)
{
	Uninit();
}


void CFrame::Serialize(int nBPP,CArchive& ar )
{
	if (ar.IsStoring())
	{
		// ����
		
		//������ ���
		ar.Write(&m_FrameHeader,sizeof(m_FrameHeader));
		// �ڽ� ����
		ar.Write(&m_CollisionNum,sizeof(m_CollisionNum));	
		// �ڽ� 
		ar.Write(&m_CollisionRect,sizeof(RECT)*m_CollisionNum);		
		
		
		//�ȼ� ������
		if (m_nBPP==24)
		{			
			int x=0,y=0;
			for (y=0;y<m_nHeight;y++)
			{
				for (x=0;x<m_nWidth;x++)
				{
					DWORD Color=(DWORD)GetPixel(x,y);
					ar.Write(&Color,4);
				}
			}
		}
		else
		{
			ar.Write(m_pBitsTopDown,m_nHeight*m_nPitch);
		}	
	}
	else
	{
		//�ε�

		//������ ���
		ar.Read(&m_FrameHeader,sizeof(m_FrameHeader));
		
		// �ڽ�����
		ar.Read(&m_CollisionNum,sizeof(m_CollisionNum));	
		// �ڽ� 
		ar.Read(&m_CollisionRect,sizeof(RECT)*m_CollisionNum);	

		CDIB::Init(m_FrameHeader.m_Width,m_FrameHeader.m_Height,nBPP);
		
		//�ȼ� ������
		ar.Read(m_pBitsTopDown,m_nHeight*m_nPitch);
		CDIB::SetTransparentColorAuto();
	}
}

void CFrame::Init()
{
	memset(&m_FrameHeader,0,sizeof(m_FrameHeader));
	m_CollisionNum=0;
	memset(&m_CollisionRect,0,sizeof(RECT)*COLLISION_MAX);
}

void CFrame::Uninit()
{

}

void CFrame::SetCenter( CPoint& pt )
{
	m_FrameHeader.m_CenterX=pt.x;
	m_FrameHeader.m_CenterY=pt.y;

}

void CFrame::SerializeOut(std::ofstream& os)
{
	os << std::to_string(m_FrameHeader.m_SourceLeft)  << ',';
	os << std::to_string(m_FrameHeader.m_SourceTop) << ',';
	os << std::to_string(m_FrameHeader.m_SourceRight) << ',';
	os << std::to_string(m_FrameHeader.m_SourceBottom) << ',';
	os << std::to_string(m_FrameHeader.m_CenterX) << ',';
	os << std::to_string(m_FrameHeader.m_CenterY) << std::endl;
}
