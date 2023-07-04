#pragma once
#include "dib.h"

typedef struct _FRAMEHEADER_
{	
	int m_Width;			// ����
	int	m_Height;			// ����
	int m_CenterX;			// ���� x
	int	m_CenterY;			// ���� y
	int	m_DelayTime;		// �����ð� 	
	int m_SourceLeft;
	int m_SourceTop;
	int m_SourceRight;
	int m_SourceBottom;
} FRAMEHEADER;


class CFrame :
	public CDIB
{
public:
	CFrame(void);	
	CFrame(CDIB& SrcDIB,CRect& SrcRect);

	virtual ~CFrame(void);

	enum { COLLISION_MAX=10 };

	void	Serialize(int nBPP,CArchive& ar);
	
	
	int		GetWidth()				{ return m_FrameHeader.m_Width;		}
	int		GetHeight()				{ return m_FrameHeader.m_Height;	}

	int		GetCenterX()			{ return m_FrameHeader.m_CenterX;		}
	void	SetCenterX(int val)		{ m_FrameHeader.m_CenterX=val;	}
	
	int		GetCenterY()			{ return m_FrameHeader.m_CenterY;		}
	void	SetCenterY(int val)		{ m_FrameHeader.m_CenterY=val;	}
	void	SetCenter(CPoint& pt);

	int		GetDelayTime()			{ return m_FrameHeader.m_DelayTime;		}
	void	SetDelayTime(int val)	{ m_FrameHeader.m_DelayTime=val;	}

	void	SerializeOut(std::ofstream& os);
protected:
	void	Init();
	void	Uninit();

protected:
	FRAMEHEADER m_FrameHeader;	
	int		m_CollisionNum;		// �浹�ڽ����� (�ϴ� �Ѱ��� �浹�ڽ�����)
	RECT	m_CollisionRect[COLLISION_MAX];
};

