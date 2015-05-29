// DlgConsume.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgConsume.h"
#include "SPASystemDlg.h"
#include "./Ado/Ado.h"
#include "CustomGridCtrl.h"
#include "DlgFindItem.h"

// CDlgConsume 对话框

IMPLEMENT_DYNAMIC(CDlgConsume, CDialog)

CDlgConsume::CDlgConsume(int iMemberID /*= 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConsume::IDD, pParent)
	, m_pGridCtrl(NULL)
	, m_sConsumeDate(_T(""))
	, m_iMemberID(iMemberID)
	, m_sName(_T(""))
	, m_sCardNo(_T(""))
	, m_sBalance(_T("0.00"))
	, m_sCardType(_T(""))
	, m_sNowMoney(_T("0.00"))
	, m_iCardID(0)
{
	m_pConSPA = NULL;
}

CDlgConsume::~CDlgConsume()
{
	DELETE_POINTER(m_pGridCtrl);
}

void CDlgConsume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONSUMEDATE, m_sConsumeDate);
	DDV_MaxChars(pDX, m_sConsumeDate, 10);
}


BEGIN_MESSAGE_MAP(CDlgConsume, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgConsume::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgConsume::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgConsume 消息处理程序

BOOL CDlgConsume::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_InitGrid();
	_InitGridFromDB();
	_CalcMoney();
	_SetDefaultText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgConsume::_SetDefaultText(void)
{
	// 卡类型
	if (m_sCardType.IsEmpty())
		m_sCardType = g_sDefaultCardType;
	_SetGridText(g_iGridRow_CardType-1, g_iGridCol_CardType-1, m_sCardType, FALSE);
}

void CDlgConsume::_InitGrid(void)
{
	DELETE_POINTER(m_pGridCtrl);
	m_pGridCtrl = new CCustomGridCtrl(g_iGridRows, g_iGridCols, 0, 0);
	if (NULL == m_pGridCtrl)
		return;
	// 手工设置位置
	int iHeight = g_iRowHeight_Row0 + g_iRowHeight * (g_iGridRows - 1);
	int iWidth = (int)(g_fColWidth * (float)g_iGridCols);
	int iHeightOff = 80;
	// 窗口的大小
	this->SetWindowPos(NULL, 0, 0, iWidth, iHeight + iHeightOff, SWP_NOMOVE);
	// 按钮的位置
	CButton* pButton = (CButton*)GetDlgItem(IDOK);
	if (pButton)
	{
		CRect rect;
		GetClientRect(rect);
		CRect rectButton;
		pButton->GetClientRect(rectButton);
		pButton->SetWindowPos(NULL, (rect.right-rectButton.right)/2, 
			rect.bottom - rectButton.bottom - 10, 0, 0, SWP_NOSIZE);
	}
	// 创建网格
	CRect rectGrid(0, 0, iWidth, iHeight);
	m_pGridCtrl->Create(rectGrid, this, 1000);
	m_pGridCtrl->SetEditable(FALSE);
	m_pGridCtrl->EnableDragAndDrop(FALSE);
	// 设置所有文本居中显示
	for (int ix=0; ix<m_pGridCtrl->GetRowCount(); ix++)
	{
		for (int iy=0; iy<m_pGridCtrl->GetColumnCount(); iy++)
		{
			_SetGridCenter(ix, iy);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 输入固定数据
	//////////////////////////////////////////////////////////////////////////
	int iBeginRow = 0;
	_InitGrid_Title(iBeginRow);	// 第1行-第2行
	_InitGrid_Head(iBeginRow);	// 第3行-第4行
	_InitGrid_Body(iBeginRow);	// 第5行-第10行
	_InitGrid_Tail(iBeginRow);	// 第11行-第12行
	ASSERT(iBeginRow == g_iGridRows);
}

void CDlgConsume::_InitGrid_Title(int& iBeginRow)
{
	// ===== 共2行 =====
	_SetGridText(iBeginRow, 0, _T("雨涵女子美容SPA馆"));
	CFont fontText;
	fontText.CreateFont(
		26,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));
	LOGFONT lf;
	fontText.GetLogFont(&lf);
	m_pGridCtrl->SetItemFont(iBeginRow, 0, &lf);
	m_pGridCtrl->SetCellCombine(iBeginRow, 0, 1, 7);
	iBeginRow += 2;
}

void CDlgConsume::_InitGrid_Head(int& iBeginRow)
{
	// ===== 共2行 =====
	// ===== 第1行 =====
	// 会员卡号
	_SetGridText(iBeginRow, 0, _T("会员卡号"), FALSE);
	// 会员姓名
	_SetGridText(iBeginRow, 2, _T("会员姓名"), FALSE);
	// 会员卡别
	_SetGridText(iBeginRow, 4, _T("会员卡别"), FALSE);
	// 卡内金额
	_SetGridText(iBeginRow, 6, _T("卡内金额"), FALSE);
	iBeginRow++;
	// ===== 第2行 =====
	// 护理项目
	_SetGridText(iBeginRow, 0, _T("护     理     项     目"), FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 0, 0, 4);
	// 消费价格
	_SetGridText(iBeginRow, 5, _T("消费价格"), FALSE);
	// 会员价格
	_SetGridText(iBeginRow, 6, _T("会员价格"), FALSE);
	// 美容师签名
	_SetGridText(iBeginRow, 7, _T("美容师"), FALSE);
	iBeginRow++;
}

void CDlgConsume::_InitGrid_Body(int& iBeginRow)
{
	// ===== 共10行 =====
	// 护理项目内容
	int iRows = 0;
	CString sText;
	for ( ; iRows < g_iGridRows_Item; iRows++)
	{
		sText.Format(_T("%d"), iRows+1);
		m_pGridCtrl->SetCellCombine(iBeginRow, 0, 0, 4);
		iBeginRow++;
	}
}

void CDlgConsume::_InitGrid_Tail(int& iBeginRow)
{
	// ===== 共2行 =====
	// ===== 第1行 =====
	// 总计
	_SetGridText(iBeginRow, 0, _T("总计消费"), FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 0, 0, 5);
	iBeginRow++;
	// ===== 第2行 =====
	// 消费日期
	_SetGridText(iBeginRow, 0, _T("消费日期"), FALSE);
	CTime time = CTime::GetCurrentTime();
	m_sConsumeDate.Format(_T("%4d年%2d月%2d日"), time.GetYear(), time.GetMonth(), time.GetDay());
	_SetGridText(iBeginRow, 1, m_sConsumeDate, FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 1, 0, 1);
	m_sConsumeDate.Format(_T("%4d-%2d-%2d"), time.GetYear(), time.GetMonth(), time.GetDay());
	UpdateData(FALSE);
	// 卡内余额
	_SetGridText(iBeginRow, 3, _T("卡内余额"), FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 4, 0, 1);
	// 会员签名
	_SetGridText(iBeginRow, 6, _T("会员签名"), FALSE);
	iBeginRow++;
}

void CDlgConsume::_SetGridText(int iRow, int iCol, LPCTSTR sText, BOOL bModified /* = TRUE */)
{
	m_pGridCtrl->SetItemText(iRow, iCol, sText);
	m_pGridCtrl->SetModified(bModified, iRow, iCol);
}

void CDlgConsume::_SetGridCenter(int iRow, int iCol)
{
	m_pGridCtrl->SetItemFormat(iRow, iCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CDlgConsume::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	int iResult = AfxMessageBox(_T("该消费单是否结账？"), MB_YESNOCANCEL);
	if (IDYES == iResult)
	{
		if (!_SaveConsume())
		{
			//AfxMessageBox(_T("保存失败！"));
			return;
		}
	}
	else if (IDNO == iResult)
	{
	}
	else if (IDCANCEL == iResult)
	{
		return; // 不关闭对话框
	}
	OnCancel();
}

void CDlgConsume::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!_SaveConsume())
	{
		//AfxMessageBox(_T("保存失败！"));
		return;
	}
	OnOK();
}

void CDlgConsume::_PrintGrid(void)
{
	m_pGridCtrl->Print();
}

BOOL CDlgConsume::_SaveConsume(void)
{
	if (!UpdateData())
		return FALSE;
	// 判断是否消费过
	if (0 == m_sNowMoney.CompareNoCase(m_sBalance))
	{
		if (IDOK == AfxMessageBox(_T("本次没有消费，是否打印？"), MB_OKCANCEL))
		{
			_PrintGrid();
		}
		return TRUE;
	}
	// 保存到数据库
	int iMaxID = 0;
	if (!_GetNextConsumeID(iMaxID))
	{
		ASSERT(0);
		SeriousError();
		return FALSE;
	}
	CString sSQL;
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("INSERT INTO 消费单(ID, 会员卡ID,消费日期,总计金额) VALUES (%d,%d,'%s',%s-%s)"),
		iMaxID, m_iCardID, m_sConsumeDate, m_sBalance, m_sNowMoney);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	sSQL.Format(_T("UPDATE 会员卡 SET 余额=%s WHERE ID=%d"), m_sNowMoney, m_iCardID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	// 接下来保存明细
	m_pConSPA->BeginTrans();
	if (!_SaveConsume_Detail(iMaxID))
	{
		ASSERT(0); // 目前明细不是重要的，所以无论保存是否成功，都打印！
		m_pConSPA->RollbackTrans();
	}
	m_pConSPA->CommitTrans();
	// 打印
	_PrintGrid();
	return TRUE;
}

BOOL CDlgConsume::_GetNextConsumeID(int& iMaxID)
{
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return FALSE;
	CString sSQL = _T("SELECT MAX(ID)+1 AS MaxID FROM 消费单");
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return FALSE;
	}
	pSetRead->GetCollect(_T("MaxID"), iMaxID);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	return TRUE;
}


BOOL CDlgConsume::_GetItemIDByName(int& iItemID, const CString& sItemName)
{
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return FALSE;
	CString sSQL;
	sSQL.Format(_T("SELECT 项目ID FROM 护理项目 WHERE 项目名称='%s'"), sItemName);
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return FALSE;
	}
	pSetRead->GetCollect(_T("项目ID"), iItemID);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	return TRUE;
}

BOOL CDlgConsume::_SaveConsume_Detail(int iID)
{
	// 首先确定护理项目开始行和列数
	double dTemp = 0.00;
	LPTSTR strTemp;
	CString strMoney;
	CString sItemName;
	CString sSQL;
	int iItemID = 0;
	int iRowItem = g_iBeginRow_Item - 1;
	for (int ix=0; ix<g_iGridRows_Item; ix++)
	{
		sItemName = m_pGridCtrl->GetItemText(iRowItem+ix, 0);
		strMoney = m_pGridCtrl->GetItemText(iRowItem+ix, g_iGridCol_Money-1);
		dTemp = _tcstod(strMoney, &strTemp);
		if (0.00 >= dTemp)
			continue;
		// 保存明细
		if (!_GetItemIDByName(iItemID, sItemName))
		{	
			return FALSE;
		}
		sSQL.Format(_T("INSERT INTO 消费详细记录(消费单ID,护理项目ID,消费金额) VALUES (%d,%d,%s)"),
			iID, iItemID, strMoney);
		if (!m_pConSPA->Execute(sSQL))
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CDlgConsume::_InitGridFromDB(void)
{
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	m_pConSPA = pMainDlg->m_pConSPA;
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL;
	sSQL.Format(_T("SELECT * FROM QueryCard WHERE 顾客ID=%d"), m_iMemberID);
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	int iMemberID = 0;
	if (!pSetRead->IsEOF())
	{
		pSetRead->GetCollect(_T("会员卡ID"), m_iCardID);
		pSetRead->GetCollect(_T("会员卡号"), m_sCardNo);
		_SetGridText(2, 1, m_sCardNo, FALSE);
		pSetRead->GetCollect(_T("姓名"), m_sName);
		_SetGridText(2, 3, m_sName, FALSE);
		pSetRead->GetCollect(_T("卡名称"), m_sCardType);
		double dBalance;
		pSetRead->GetCollect(_T("余额"), dBalance);
		m_sBalance.Format(_T("%.2f"), dBalance);
		_SetGridText(2, 7, m_sBalance, FALSE);
	}
	pSetRead->Close();

	// 删除
	DELETE_POINTER(pSetRead);
}

void CDlgConsume::_CalcMoney(void)
{
	// 消费前卡内金额
	LPTSTR strTemp;
	double dMoney = _tcstod(m_sBalance, &strTemp);
	m_sNowMoney = m_sBalance;
	// 计算总计消费金额
	double dConsume = 0.00;
	double dTemp = 0.00;
	// 首先确定护理项目开始行和列数
	CString strMoney;
	int iRowItem = g_iBeginRow_Item - 1;
	for (int ix=0; ix<g_iGridRows_Item; ix++)
	{
		strMoney = m_pGridCtrl->GetItemText(iRowItem+ix, g_iGridCol_Money-1);
		dTemp = _tcstod(strMoney, &strTemp);
		dConsume += dTemp;
	}
	strMoney.Format(_T("%.2f"), dConsume);
	_SetGridText(g_iGridRows-2, g_iGridCol_Money-1, strMoney, FALSE);
	m_pGridCtrl->RedrawCell(g_iGridRows-2, g_iGridCol_Money-1);
	// 卡内余额
	strMoney.Format(_T("%.2f"), dMoney - dConsume);
	_SetGridText(g_iGridRows-1, 4, strMoney, FALSE);
	m_sNowMoney = strMoney;
	m_pGridCtrl->RedrawCell(g_iGridRows-1, 4);
	if (dMoney - dConsume < 0)
	{
		AfxMessageBox(_T("卡内余额不足，需要充值！"));
	}
}

void CDlgConsume::_ProcMsgGrid(int iRow, int iCol)
{
	// 寻找项目
	CDlgFindItem dlgFind;
	dlgFind.m_strCardType = m_pGridCtrl->GetItemText(g_iGridRow_CardType-1, g_iGridCol_CardType-1);
	if (IDOK == dlgFind.DoModal())
	{
		FindItem* pItem = dlgFind.m_pSelItem;
		if (pItem)
		{
			_SetGridText(iRow, 0, pItem->sItemName);
			_SetGridText(iRow, g_iGridCol_Cost-1, pItem->sCost);
			_SetGridText(iRow, g_iGridCol_Money-1, pItem->sMoney);
			m_pGridCtrl->RedrawRow(iRow);
			_CalcMoney();
		}
	}
}

void CDlgConsume::_DeleteGrid(int iRow)
{
	_SetGridText(iRow, 0, _T(""));
	_SetGridText(iRow, g_iGridCol_Cost-1, _T(""));
	_SetGridText(iRow, g_iGridCol_Money-1, _T(""));
	m_pGridCtrl->RedrawRow(iRow);
	_CalcMoney();
}

BOOL CDlgConsume::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (wParam == m_pGridCtrl->GetDlgCtrlID())
	{
		NM_GRIDVIEW* pNmgv = (NM_GRIDVIEW*)lParam;
		if (pNmgv)
		{		
			//if (GVN_SELCHANGED == pNmgv->hdr.code)
			if (GRIDMSG_DBCLICK == pNmgv->hdr.code)
			{
				_ProcMsgGrid(pNmgv->iRow, pNmgv->iColumn);
			}
			else if (GVN_ENDLABELEDIT == pNmgv->hdr.code)
			{
				_DeleteGrid(pNmgv->iRow);
			}
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}
