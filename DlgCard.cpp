// DlgCard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgCard.h"
#include "./Ado/Ado.h"
#include "SPASystemDlg.h"
#include "DlgMember.h"

// CDlgCard �Ի���

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


// CDlgCard ��Ϣ�������

BOOL CDlgCard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//������չ���
	DWORD styles =	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_listCard.m_hWnd, styles, styles );
	m_listCard.InsertColumn(0, _T("��Ա����"), LVCFMT_LEFT, 100, 0);
	m_listCard.InsertColumn(1, _T("����"), LVCFMT_LEFT, 80, 1);
	m_listCard.InsertColumn(2, _T("������"), LVCFMT_LEFT, 80, 2);
	m_listCard.InsertColumn(3, _T("���"), LVCFMT_LEFT, 100, 3);
	m_listCard.InsertColumn(4, _T("��������"), LVCFMT_LEFT, 80, 4);
	_InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgCard::_InitList(void)
{
	_DeleteAllListItem(&m_listCard);
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	// ����list
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
		pSetRead->GetCollect(_T("��Ա����"), sCardNo);
		pMemberCard->sCardNo.Format(_T("%s"), sCardNo);
		pSetRead->GetCollect(_T("����"), sMemberName);
		pSetRead->GetCollect(_T("������"), sCardName);
		pSetRead->GetCollect(_T("���"), dBalance);
		pSetRead->GetCollect(_T("�˿�ID"), iMemberID);
		pMemberCard->iMemberID = iMemberID;
		pSetRead->GetCollect(_T("��������"), sCreateCardDate);
		iIndex = m_listCard.InsertItem(iCount, sCardNo);
		pMemberCard->iItem = iIndex;
		m_listCard.SetItemText(iIndex, 1, sMemberName);
		m_listCard.SetItemText(iIndex, 2, sCardName);
		sBalance.Format(_T("%.2f"), dBalance);
		m_listCard.SetItemText(iIndex, 3, sBalance);
		m_listCard.SetItemText(iIndex, 4, sCreateCardDate);
		m_listCard.SetItemData(iIndex, (DWORD_PTR)pMemberCard);
		// ������һ��
		pSetRead->MoveNext();
		iCount++;
	}
	pSetRead->Close();
	m_listCard.SetItemState(0, 0xFFFF, LVIS_SELECTED);

	// ɾ��
	DELETE_POINTER(pSetRead);
}

void CDlgCard::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_CallCard(0);
}

void CDlgCard::OnBnClickedBtnUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("��ѡ���Ա����"));
		return NULL;
	}
	// 
	int nItem = m_listCard.GetNextSelectedItem(pos);

	return (MemberCard*)m_listCard.GetItemData(nItem);
}
void CDlgCard::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MemberCard* pMCTemp = _GetCurSelMember();
	if (!pMCTemp)
		return;
	// �ж��Ƿ���Ĭ����ͨ��
	if (DEFAULT_CARDNO == pMCTemp->sCardNo ||
		DEFUALT_MEMBERID == pMCTemp->iMemberID)
	{
		AfxMessageBox(_T("�ÿ�ΪĬ����ͨ�����޷�ɾ����"));
		return;
	}
	CString sSQL;
	sSQL.Format(_T("ȷ���Ƿ�ɾ���û�Ա��%s?"), pMCTemp->sCardNo);
	if (IDCANCEL == AfxMessageBox(sSQL, MB_OKCANCEL))
		return;
	int iMemberID = pMCTemp->iMemberID;
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	CAdoConnection* pConWrite = pMainDlg->m_pConSPA;
	pConWrite->BeginTrans();
	// ɾ���˿�
	sSQL.Format(_T("DELETE FROM �˿���Ϣ WHERE �˿�ID=%d"), iMemberID);
	if (!pConWrite->Execute(sSQL))
	{
		pConWrite->RollbackTrans();
		return;
	}
	// ɾ����Ա��
	sSQL.Format(_T("DELETE FROM ��Ա�� WHERE �˿�ID=%d"), iMemberID);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���

	_DeleteAllListItem(&m_listCard);
	CDialog::OnCancel();
}
