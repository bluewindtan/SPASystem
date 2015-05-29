// DlgCard.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgCard.h"
#include "./Ado/Ado.h"
#include "SPASystemDlg.h"
#include "DlgMember.h"

// CDlgCard 对话框

IMPLEMENT_DYNAMIC(CDlgCard, CDialog)

CDlgCard::CDlgCard(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCard::IDD, pParent)
{

}

CDlgCard::~CDlgCard()
{
}

void CDlgCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CARD, m_listCard);
}


BEGIN_MESSAGE_MAP(CDlgCard, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgCard::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CDlgCard::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgCard::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CARD, &CDlgCard::OnNMDblclkListCard)
END_MESSAGE_MAP()


// CDlgCard 消息处理程序

BOOL CDlgCard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置扩展风格
	DWORD styles =	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_listCard.m_hWnd, styles, styles );
	m_listCard.InsertColumn(0, _T("会员卡号"), LVCFMT_LEFT, 100, 0);
	m_listCard.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 80, 1);
	m_listCard.InsertColumn(2, _T("卡类型"), LVCFMT_LEFT, 80, 2);
	m_listCard.InsertColumn(3, _T("余额"), LVCFMT_LEFT, 100, 3);
	m_listCard.InsertColumn(4, _T("开卡日期"), LVCFMT_LEFT, 80, 4);
	_InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCard::_InitList(void)
{
	_DeleteAllListItem(&m_listCard);
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	// 加载list
	CAdoRecordSet* pSetRead = new CAdoRecordSet(pMainDlg->m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL = _T("SELECT * FROM QueryCard");
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	int iIndex = 0;
	int iCount = 0;
	CString sCardNo;
	CString sMemberName;
	CString sCardName;
	double dBalance;
	CString sBalance;
	CString sCreateCardDate;
	int iMemberID = 0;
	MemberCard* pMemberCard;
	while(!pSetRead->IsEOF())
	{
		pMemberCard = new MemberCard;
		pSetRead->GetCollect(_T("会员卡号"), sCardNo);
		pMemberCard->sCardNo.Format(_T("%s"), sCardNo);
		pSetRead->GetCollect(_T("姓名"), sMemberName);
		pSetRead->GetCollect(_T("卡名称"), sCardName);
		pSetRead->GetCollect(_T("余额"), dBalance);
		pSetRead->GetCollect(_T("顾客ID"), iMemberID);
		pMemberCard->iMemberID = iMemberID;
		pSetRead->GetCollect(_T("开卡日期"), sCreateCardDate);
		iIndex = m_listCard.InsertItem(iCount, sCardNo);
		pMemberCard->iItem = iIndex;
		m_listCard.SetItemText(iIndex, 1, sMemberName);
		m_listCard.SetItemText(iIndex, 2, sCardName);
		sBalance.Format(_T("%.2f"), dBalance);
		m_listCard.SetItemText(iIndex, 3, sBalance);
		m_listCard.SetItemText(iIndex, 4, sCreateCardDate);
		m_listCard.SetItemData(iIndex, (DWORD_PTR)pMemberCard);
		// 继续下一个
		pSetRead->MoveNext();
		iCount++;
	}
	pSetRead->Close();
	m_listCard.SetItemState(0, 0xFFFF, LVIS_SELECTED);

	// 删除
	DELETE_POINTER(pSetRead);
}

void CDlgCard::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	_CallCard(0);
}

void CDlgCard::OnBnClickedBtnUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	MemberCard* pMCTemp = _GetCurSelMember();
	if (!pMCTemp)
		return;
	_CallCard(1, pMCTemp->iMemberID);
}

void CDlgCard::_CallCard(int iType, int iMemberID /* = 0 */)
{
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	CDlgMember dlg(iType, iMemberID);
	dlg.m_pConSPA = pMainDlg->m_pConSPA;
	dlg.DoModal();
	_InitList();
}

MemberCard* CDlgCard::_GetCurSelMember(void)
{
	POSITION pos = m_listCard.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		AfxMessageBox(_T("请选择会员卡！"));
		return NULL;
	}
	// 
	int nItem = m_listCard.GetNextSelectedItem(pos);

	return (MemberCard*)m_listCard.GetItemData(nItem);
}
void CDlgCard::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	MemberCard* pMCTemp = _GetCurSelMember();
	if (!pMCTemp)
		return;
	// 判断是否是默认普通卡
	if (DEFAULT_CARDNO == pMCTemp->sCardNo ||
		DEFUALT_MEMBERID == pMCTemp->iMemberID)
	{
		AfxMessageBox(_T("该卡为默认普通卡，无法删除！"));
		return;
	}
	CString sSQL;
	sSQL.Format(_T("确认是否删除该会员卡%s?"), pMCTemp->sCardNo);
	if (IDCANCEL == AfxMessageBox(sSQL, MB_OKCANCEL))
		return;
	int iMemberID = pMCTemp->iMemberID;
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	CAdoConnection* pConWrite = pMainDlg->m_pConSPA;
	pConWrite->BeginTrans();
	// 删除顾客
	sSQL.Format(_T("DELETE FROM 顾客信息 WHERE 顾客ID=%d"), iMemberID);
	if (!pConWrite->Execute(sSQL))
	{
		pConWrite->RollbackTrans();
		return;
	}
	// 删除会员卡
	sSQL.Format(_T("DELETE FROM 会员卡 WHERE 顾客ID=%d"), iMemberID);
	if (!pConWrite->Execute(sSQL))
	{
		pConWrite->RollbackTrans();
		return;
	}
	pConWrite->CommitTrans();
	m_listCard.DeleteItem(pMCTemp->iItem);
	m_listCard.SetItemState(0, 0xFFFF, LVIS_SELECTED);
}

void CDlgCard::OnNMDblclkListCard(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedBtnUpdate();
	*pResult = 0;
}

void CDlgCard::_DeleteAllListItem(CListCtrl* pList)
{
	if (!pList)
		return;
	int nCount = pList->GetItemCount();
	if (nCount <= 0)
		return;
	for (int iRow=nCount-1; iRow >= 0; iRow-- )
	{
		MemberCard* pItem = (MemberCard*)pList->GetItemData(iRow);
		DELETE_POINTER(pItem);
		pList->DeleteItem(iRow);
	}
}

void CDlgCard::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	_DeleteAllListItem(&m_listCard);
	CDialog::OnCancel();
}
