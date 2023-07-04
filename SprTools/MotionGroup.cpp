#include "StdAfx.h"
#include "MotionGroup.h"

CMotionGroup::CMotionGroup(void)
{
	m_bDeleteAll=FALSE;
	m_nSelectedMotionIndex=0;
}

CMotionGroup::~CMotionGroup(void)
{
	if (!m_bDeleteAll)
	{
		DeleteAll();
	}
}

CMotion* CMotionGroup::AddMotion()
{
	CMotion* pItem;
	pItem=new CMotion;
	m_listMotion.AddTail(pItem);
	m_bDeleteAll=FALSE;
	m_nSelectedMotionIndex=(int)m_listMotion.GetCount()-1;
	return pItem;
}

void CMotionGroup::AddMotion( CMotion* pItem )
{
	m_listMotion.AddTail(pItem);
	m_nSelectedMotionIndex=(int)m_listMotion.GetCount()-1;
	m_bDeleteAll=FALSE;
}

void CMotionGroup::DelMotion( int index )
{
	if (m_listMotion.GetSize()==0)
		return;

	int cntIndex=0;
	POSITION pos;
	CMotion* pItem;

	pos= m_listMotion.FindIndex(index);
	if (pos==NULL)
		return;

	pItem=m_listMotion.GetAt(pos);
	if (pItem==NULL)
		return;

	delete pItem;
	m_listMotion.RemoveAt(pos);

	if (m_listMotion.GetCount()==0)
	{
		return;
	}
	if (index==m_listMotion.GetCount())
	{
		m_nSelectedMotionIndex=(int)m_listMotion.GetCount()-1;
	}
	
}

CMotion* CMotionGroup::GetMotion( int index )
{
	POSITION pos;
	CMotion* pItem=NULL;

	if (index<0)
	{
		return pItem;
	}

	pos=m_listMotion.FindIndex(index);
	if (pos!=NULL)
	{
		pItem=m_listMotion.GetAt(pos);
	}
	return pItem;
}

void CMotionGroup::DeleteAll()
{
	POSITION pos;
	CMotion* pItem;

	if ((pos=m_listMotion.GetHeadPosition())==NULL)
		return;

	while (pos!=NULL)
	{
		pItem=m_listMotion.GetNext(pos);
		delete pItem;
	}
	m_listMotion.RemoveAll();
	m_bDeleteAll=TRUE;

	
}

int CMotionGroup::GetCount()
{
	return (int)m_listMotion.GetCount();
}

CMotion* CMotionGroup::GetMotionSelected()
{
	return GetMotion(m_nSelectedMotionIndex);
}