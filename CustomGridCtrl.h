#pragma once
#include "./GridControl/GridCtrl.h"

//////////////////////////////////////////////////////////////////////////
// 定义常量
const int g_iGridRows = 12;			// 总共行数
const int g_iRowHeight_Row0 = 40;	// 行高(第一行)
const int g_iRowHeight = 25;		// 行高
const float g_fColWidth = 85.2f;    // 列宽
const int g_iGridCols = 8;			// 总共列数
const int g_iBeginRow_Item = 5;	// 护理项目从第几行开始
const int g_iGridRows_Item = 6;	// 护理项目总行数

const int g_iGridCol_Cost = 6; // 原来价格所在的列
const int g_iGridCol_Money = 7; // 优惠后价格所在的列

const int g_iGridRow_CardType = 3; // 会员卡号、卡类型等等所在的行

const int g_iGridCol_CardNo = 2; // 卡类型所在的列
const int g_iGridCol_CardType = 6; // 卡类型所在的列

const CString g_sDefaultCardType = _T("普通卡"); // 默认卡类型

const int g_iFootDistance = 30; // 页脚离表格的距离
const int g_iHalfPageGap = 15; // 半页之间的距离

enum GridMsg
{
	GRIDMSG_DBCLICK = 8888, // 双击网格消息

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
    
	// 画出表格的过程
	void DrawGrid(CDC *pDC, CPrintInfo *pInfo, int iBeginX = 0, int iBeginY = 0);
	// 是否是项目部分
	BOOL IsInItemRow(int iRow);
};
