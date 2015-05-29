#include "StdAfx.h"
#include "CustomGridCtrl.h"

#define HEADER_HEIGHT       2    // For printing
#define FOOTER_HEIGHT       2
#define LEFT_MARGIN         4
#define RIGHT_MARGIN        4
#define TOP_MARGIN          1
#define BOTTOM_MARGIN       1 
#define GAP                 1

#define SELECTED_CELL_FONT_WEIGHT 600    // weight of text for selected items

CCustomGridCtrl::CCustomGridCtrl(int nRows /* = 0 */, int nCols /* = 0 */, int nFixedRows /* = 0 */, int nFixedCols /* = 0 */)
: CGridCtrl(nRows, nCols, nFixedRows, nFixedCols)
{
}

CCustomGridCtrl::~CCustomGridCtrl(void)
{
}


void CCustomGridCtrl::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
	CGridCtrl::OnBeginPrinting(pDC, pInfo);
}

void CCustomGridCtrl::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
	//CGridCtrl::OnPrint(pDC, pInfo);
	if (!pDC || !pInfo) return;

	//CRect rcPage(pInfo->m_rectDraw);
	CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

	// Set the page map mode to use GridCtrl units, and setup margin
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_LogicalPageSize);
	pDC->SetViewportExt(m_PaperSize);

	// 画出表格
	DrawGrid(pDC, pInfo, 0, g_iFootDistance);
	// 翻半页，继续画
	//DrawGrid(pDC, pInfo, 0, (m_LogicalPageSize.cy + 1) / 2 + g_iHalfPageGap);

	// SetWindowOrg back for next page
	pDC->SetWindowOrg(0,0);

	pDC->SelectObject(pOldFont);	
}

void CCustomGridCtrl::OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)
{
	CGridCtrl::OnEndPrinting(pDC, pInfo);
}

void CCustomGridCtrl::DrawGrid(CDC *pDC, CPrintInfo *pInfo, 
							   int iBeginX /* = 0 */, int iBeginY /* = 0 */)
{
	pDC->SetWindowOrg(-LEFT_MARGIN*m_CharSize.cx, -iBeginY);
	// Header
	pInfo->m_rectDraw.top    = 0;
	pInfo->m_rectDraw.left   = 0;
	pInfo->m_rectDraw.right  = m_LogicalPageSize.cx - (LEFT_MARGIN+RIGHT_MARGIN)*m_CharSize.cx;
	pInfo->m_rectDraw.bottom = HEADER_HEIGHT*m_CharSize.cy;
	//PrintHeader(pDC, pInfo);
	pDC->OffsetWindowOrg(0, -HEADER_HEIGHT*m_CharSize.cy);

	// Gap between header and column headings
	pDC->OffsetWindowOrg(0, -GAP*m_CharSize.cy);

	// Print the column headings
	pInfo->m_rectDraw.bottom = GetFixedRowHeight(); 
	PrintColumnHeadings(pDC, pInfo);
	pDC->OffsetWindowOrg(0, -GetFixedRowHeight()); 

	// We need to find out which row to start printing for this page.
	int nTotalRowHeight = 0;
	UINT nNumPages = 1;
	int nCurrPrintRow = GetFixedRowCount();

	while (nCurrPrintRow < GetRowCount() && nNumPages < pInfo->m_nCurPage)
	{
		nTotalRowHeight += GetRowHeight(nCurrPrintRow);
		if (nTotalRowHeight > m_nPageHeight) {
			nNumPages++;
			if (nNumPages == pInfo->m_nCurPage) break;
			nTotalRowHeight = GetRowHeight(nCurrPrintRow);
		}
		nCurrPrintRow++;
	}
	if (nCurrPrintRow >= GetRowCount()) return;

	// Draw as many rows as will fit on the printed page.
	// Clip the printed page so that there is no partially shown
	// row at the bottom of the page (the same row which will be fully
	// shown at the top of the next page).

	BOOL bFirstPrintedRow = TRUE;
	CRect rect;
	rect.bottom = -1;
	while (nCurrPrintRow < GetRowCount())
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(nCurrPrintRow) - 1;

		if (rect.bottom > m_nPageHeight) break;            // Gone past end of page

		rect.right = -1;
		for (int col = 0; col < GetColumnCount(); col++)
		{
			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) - 1;

			DrawCell(pDC, nCurrPrintRow, col, rect);

			if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
			{
				int Overlap = (col == 0)? 0:1;
				pDC->MoveTo(rect.left-Overlap, rect.bottom);
				pDC->LineTo(rect.right, rect.bottom);
				if (nCurrPrintRow == 0) {
					pDC->MoveTo(rect.left-Overlap, rect.top);
					pDC->LineTo(rect.right, rect.top);
				}
			}
			if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
			{
				int Overlap = (bFirstPrintedRow)? 0:1;
				pDC->MoveTo(rect.right, rect.top-Overlap);
				pDC->LineTo(rect.right, rect.bottom);    
				if (col == 0) {
					pDC->MoveTo(rect.left, rect.top-Overlap);
					pDC->LineTo(rect.left, rect.bottom);    
				}
			}

		}
		nCurrPrintRow++;
		bFirstPrintedRow = FALSE;
	}

	// Footer
	CPoint pPoint = pDC->GetWindowOrg();
	CRect rectGrid;
	GetClientRect(rectGrid);
	pInfo->m_rectDraw.top = 0;
	pInfo->m_rectDraw.bottom = FOOTER_HEIGHT*m_CharSize.cy;
	//pDC->OffsetWindowOrg(-LEFT_MARGIN*m_CharSize.cx-pPoint.x, pPoint.y+iBeginY-rectGrid.Height()-g_iFootDistance);
	//PrintFooter(pDC, pInfo);
}

void CCustomGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{	
	CCellID cell = GetCellFromPt(point);
	//if (cell.row == g_iGridRow_CardType-1 && cell.col == g_iGridCol_CardNo - 1)
	//{
	//	CGridCtrl::OnLButtonDblClk(nFlags, point);
	//}
	if (IsInItemRow(cell.row))
	{
		CGridCtrl::SendMessageToParent(cell.row, cell.col, GRIDMSG_DBCLICK);
	}
	
	CGridCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CCustomGridCtrl::IsInItemRow(int iRow)
{
	return (iRow>= g_iBeginRow_Item - 1 && iRow <= g_iBeginRow_Item + g_iGridRows_Item - 2);
}

void CCustomGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_DELETE: 
		if (IsInItemRow(m_idCurrentCell.row))
		{
			SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_BEGINLABELEDIT);
			SetItemText(m_idCurrentCell.row, m_idCurrentCell.col, _T(""));
			SetModified(TRUE, m_idCurrentCell.row, m_idCurrentCell.col);
			SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_ENDLABELEDIT);
			RedrawCell(m_idCurrentCell);
		}
		break;
	default:
		break;
	}

	return CGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
