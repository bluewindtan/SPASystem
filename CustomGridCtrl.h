#pragma once
#include "./GridControl/GridCtrl.h"

//////////////////////////////////////////////////////////////////////////
// ���峣��
const int g_iGridRows = 12;			// �ܹ�����
const int g_iRowHeight_Row0 = 40;	// �и�(��һ��)
const int g_iRowHeight = 25;		// �и�
const float g_fColWidth = 85.2f;    // �п�
const int g_iGridCols = 8;			// �ܹ�����
const int g_iBeginRow_Item = 5;	// ������Ŀ�ӵڼ��п�ʼ
const int g_iGridRows_Item = 6;	// ������Ŀ������

const int g_iGridCol_Cost = 6; // ԭ���۸����ڵ���
const int g_iGridCol_Money = 7; // �Żݺ�۸����ڵ���

const int g_iGridRow_CardType = 3; // ��Ա���š������͵ȵ����ڵ���

const int g_iGridCol_CardNo = 2; // ���������ڵ���
const int g_iGridCol_CardType = 6; // ���������ڵ���

const CString g_sDefaultCardType = _T("��ͨ��"); // Ĭ�Ͽ�����

const int g_iFootDistance = 30; // ҳ������ľ���
const int g_iHalfPageGap = 15; // ��ҳ֮��ľ���

enum GridMsg
{
	GRIDMSG_DBCLICK = 8888, // ˫��������Ϣ

};

class CCustomGridCtrl :
	public CGridCtrl
{
public:
	CCustomGridCtrl(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CCustomGridCtrl(void);

	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    
	// �������Ĺ���
	void DrawGrid(CDC *pDC, CPrintInfo *pInfo, int iBeginX = 0, int iBeginY = 0);
	// �Ƿ�����Ŀ����
	BOOL IsInItemRow(int iRow);
};
