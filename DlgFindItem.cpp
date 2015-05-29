// DlgFindItem.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgFindItem.h"
#include "SPASystemDlg.h"
#include "./Ado/Ado.h"

// CDlgFindItem 对话框

IMPLEMENT_DYNAMIC(CDlgFindItem, CDialog)

CDlgFindItem::CDlgFindItem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindItem::IDD, pParent)
	, m_sFindItem(_T(""))
	, m_strCardType(_T(""))
	, m_pSelItem(NULL)
{

}

CDlgFindItem::~CDlgFindItem()
{
	DELETE_POINTER(m_pSelItem);
}

void CDlgFindItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIND, m_sFindItem);
	DDV_MaxChars(pDX, m_sFindItem, 10);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
}


BEGIN_MESSAGE_MAP(CDlgFindItem, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgFindItem::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ITEM, &CDlgFindItem::OnNMDblclkListItem)
	ON_EN_CHANGE(IDC_EDIT_FIND, &CDlgFindItem::OnEnChangeEditFind)
END_MESSAGE_MAP()


// CDlgFindItem 消息处理程序

void CDlgFindItem::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	FindItem* pItem = _GetCurSelItem();
	if (pItem && m_pSelItem)
	{
		*m_pSelItem = *pItem;
	}
	_DeleteAllListItem(&m_listItem);
	OnOK();
}

BOOL CDlgFindItem::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置扩展风格
	DWORD styles =	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_listItem.m_hWnd, styles, styles );
	m_listItem.InsertColumn(0, _T("护理项目"), LVCFMT_LEFT, 100, 0);
	m_listItem.InsertColumn(1, _T("消费价格"), LVCFMT_LEFT, 80, 1);
	m_listItem.InsertColumn(2, _T("会员价格"), LVCFMT_LEFT, 80, 2);
	_InitList();
	m_pSelItem = new FindItem;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgFindItem::_InitList(void)
{
	_DeleteAllListItem(&m_listItem);
	if (m_strCardType.IsEmpty())
		return;
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	// 加载list
	CAdoRecordSet* pSetRead = new CAdoRecordSet(pMainDlg->m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL;
	sSQL.Format(_T("SELECT 项目ID,项目名称,普通卡 AS Cost,%s AS Price FROM 护理项目"), m_strCardType);
	if (!m_sFindItem.IsEmpty())
	{
		sSQL.AppendFormat(_T(" WHERE 快捷拼音 LIKE '%s%%'"), m_sFindItem);
	}
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	int iIndex = 0;
	int iCount = 0;
	CString sItemName;
	double dBalance;
	CString sCost;
	CString sMoney;
	int iID = 0;
	FindItem* pItem;
	while(!pSetRead->IsEOF())
	{
		pItem = new FindItem;
		pSetRead->GetCollect(_T("项目ID"), iID);
		pItem->iID = iID;
		pSetRead->GetCollect(_T("项目名称"), sItemName);
		pItem->sItemName.Format(_T("%s"), sItemName);
		iIndex = m_listItem.InsertItem(iCount, sItemName);
		pItem->iItem = iIndex;
		pSetRead->GetCollect(_T("Cost"), dBalance);
		pItem->sCost.Format(_T("%.2f"), dBalance);
		m_listItem.SetItemText(iIndex, 1, pItem->sCost);
		pSetRead->GetCollect(_T("Price"), dBalance);
		pItem->sMoney.Format(_T("%.2f"), dBalance);
		m_listItem.SetItemText(iIndex, 2, pItem->sMoney);
		m_listItem.SetItemData(iIndex, (DWORD_PTR)pItem);
		// 继续下一个
		pSetRead->MoveNext();
		iCount++;
	}
	pSetRead->Close();
	m_listItem.SetItemState(0, 0xFFFF, LVIS_SELECTED);

	// 删除
	DELETE_POINTER(pSetRead);
}

FindItem* CDlgFindItem::_GetCurSelItem(void)
{
	POSITION pos = m_listItem.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		AfxMessageBox(_T("请选择护理项目！"));
		return NULL;
	}
	// 
	int nItem = m_listItem.GetNextSelectedItem(pos);

	return (FindItem*)m_listItem.GetItemData(nItem);
}

void CDlgFindItem::_DeleteAllListItem(CListCtrl* pList)
{
	if (!pList)
		return;
	int nCount = pList->GetItemCount();
	if (nCount <= 0)
		return;
	for (int iRow=nCount-1; iRow >= 0; iRow-- )
	{
		FindItem* pItem = (FindItem*)pList->GetItemData(iRow);
		DELETE_POINTER(pItem);
		pList->DeleteItem(iRow);
	}
}
void CDlgFindItem::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedOk();
	*pResult = 0;
}

void CDlgFindItem::OnEnChangeEditFind()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (UpdateData())
	{
		_InitList();
	}
}
