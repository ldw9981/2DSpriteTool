#pragma once
#include "Motion.h"

class CMotionGroup
{
public:
	CMotionGroup(void);
	virtual ~CMotionGroup(void);

protected:
	
	CList <CMotion*,CMotion*> m_listMotion;
	BOOL	m_bDeleteAll;
	int		m_nSelectedMotionIndex;

public:
	int			GetCount();
	CMotion*	AddMotion();
	void		AddMotion(CMotion* pItem);
	void		DelMotion(int index);
	CMotion*	GetMotion(int index);

	CMotion*	GetMotionSelected();

protected:
	void		DeleteAll();
};
