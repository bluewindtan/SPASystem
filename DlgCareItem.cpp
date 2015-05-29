// DlgCard.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgCareItem.h"
#include "./Ado/Ado.h"
#include "SPASystemDlg.h"
#include "DlgItemInfo.h"

// CDlgCareItem 对话框

IMPLEMENT_DYNAMIC(CDlgCareItem, CDialog)

CDlgCareItem::CDlgCareItem(CWnd* pParent /*=NULL*/)
: CDialog(CDlgCareItem::IDD, pParent)
{

}

CDlgCareItem::~CDlgCareItem()
{
}

void CDlgCareItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
}


BEGIN_MESSAGE_MAP(CDlgCareItem, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgCareItem::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CDlgCareItem::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgCareItem::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ITEM, &CDlgCareItem::OnNMDblclkListItem)
END_MESSAGE_MAP()


// CDlgCareItem 消息处理程序

BOOL CDlgCareItem::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置扩展风格
	DWORD styles =	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_listItem.m_hWnd, styles, styles );
	m_listItem.InsertColumn(0, _T("会员卡号"), LVCFMT_LEFT, 100, 0);
	m_listItem.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 80, 1);
	m_listItem.InsertColumn(2, _T("卡类型"), LVCFMT_LEFT, 80, 2);
	m_listItem.InsertColumn(3, _T("余额"), LVCFMT_LEFT, 100, 3);
	m_listItem.InsertColumn(4, _T("开卡日期"), LVCFMT_LEFT, 80, 4);
	_InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCareItem::_InitList(void)
{
	_DeleteAllListItem(&m_listItem);
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	// 加载list
	CAdoRecordSet* pSetRead = new CAdoRecordSet(pMainDlg->m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL = _T("SELECT * FROM 护理项目");
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	int iIndex = 0;
	int iCount = 0;
	CString sItemName;
	double dBalance;
	CString sBalancePT;
	CString sBalanceY;
	CString sBalanceJ;
	CString sBalanceBJ;
	int iID = 0;
	CareItem* pItem;
	while(!pSetRead->IsEOF())
	{
		pItem = new CareItem;
		pSetRead->GetCollect(_T("项目ID"), iID);
		pItem->iID = iID;
		pSetRead->GetCollect(_T("项目名称"), sItemName);
		pItem->sItemName.Format(_T("%s"), sItemName);
		iIndex = m_listItem.InsertItem(iCount, sItemName);
		pItem->iItem = iIndex;
		pSetRead->GetCollect(_T("普通卡"), dBalance);
		sBalancePT.Format(_T("%.2f"), dBalance);
		m_listItem.SetItemText(iIndex, 1, sBalancePT);
		pSetRead->GetCollect(_T("银卡"), dBalance);
		sBalanceY.Format(_T("%.2f"), dBalance);
		m_listItem.SetItemText(iIndex, 2, sBalanceY);
		pSetRead->GetCollect(_T("金卡"), dBalance);
		sBalanceJ.Format(_T("%.2f"), dBalance);
		m_listItem.SetItemText(iIndex, 3, sBalanceJ);
		pSetRead->GetCollect(_T("白金卡"), dBalance);
		sBalanceBJ.Format(_T("%.2f"), dBalance);
		m_listItem.SetItemText(iIndex, 4, sBalanceBJ);
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

void CDlgCareItem::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	_CallItem(0);
}

void CDlgCareItem::OnBnClickedBtnUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	CareItem* pCurItem = _GetCurSelItem();
	if (!pCurItem)
		return;
	_CallItem(1, pCurItem->iID);
}

void CDlgCareItem::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	CareItem* pCurItem = _GetCurSelItem();
	if (!pCurItem)
		return;
	CString sSQL;
	sSQL.Format(_T("确认是否删除该护理项目：%s?"), pCurItem->sItemName);
	if (IDCANCEL == AfxMessageBox(sSQL, MB_OKCANCEL))
		return;
	int iID = pCurItem->iID;
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	CAdoConnection* pConWrite = pMainDlg->m_pConSPA;
	pConWrite->BeginTrans();
	// 删除顾客
	sSQL.Format(_T("DELETE FROM 护理项目 WHERE 项目ID=%d"), iID);
	if (!pConWrite->Execute(sSQL))
	{
		pConWrite->RollbackTrans();
		return;
	}
	pConWrite->CommitTrans();
	m_listItem.DeleteItem(pCurItem->iItem);
	m_listItem.SetItemState(0, 0xFFFF, LVIS_SELECTED);
}

CareItem* CDlgCareItem::_GetCurSelItem(void)
{
	POSITION pos = m_listItem.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		AfxMessageBox(_T("请选择护理项目！"));
		return NULL;
	}
	// 
	int nItem = m_listItem.GetNextSelectedItem(pos);

	return (CareItem*)m_listItem.GetItemData(nItem);
}

void CDlgCareItem::_CallItem(int iType, int iID /* = 0 */)
{
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	CDlgItemInfo dlg(iType, iID);
	dlg.m_pConSPA = pMainDlg->m_pConSPA;
	dlg.DoModal();
	_InitList();
}

void CDlgCareItem::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedBtnUpdate();
	*pResult = 0;
}

void CDlgCareItem::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	_DeleteAllListItem(&m_listItem);
	CDialog::OnCancel();
}

void CDlgCareItem::_DeleteAllListItem(CListCtrl* pList)
{
	if (!pList)
		return;
	int nCount = pList->GetItemCount();
	if (nCount <= 0)
		return;
	for (int iRow=nCount-1; iRow >= 0; iRow-- )
	{
		CareItem* pItem = (CareItem*)pList->GetItemData(iRow);
		DELETE_POINTER(pItem);
		pList->DeleteItem(iRow);
	}
}
