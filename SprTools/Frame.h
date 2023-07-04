#pragma once
#include "dib.h"

typedef struct _FRAMEHEADER_
{	
	int m_Width;			// 가로
	int	m_Height;			// 세로
	int m_CenterX;			// 중점 x
	int	m_CenterY;			// 중점 y
	int	m_DelayTime;		// 지연시간 	
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
	int		m_CollisionNum;		// 충돌박스개수 (일단 한가지 충돌박스종류)
	RECT	m_CollisionRect[COLLISION_MAX];
};

