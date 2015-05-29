// DlgFindItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgFindItem.h"
#include "SPASystemDlg.h"
#include "./Ado/Ado.h"

// CDlgFindItem �Ի���

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


// CDlgFindItem ��Ϣ�������

void CDlgFindItem::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//������չ���
	DWORD styles =	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_listItem.m_hWnd, styles, styles );
	m_listItem.InsertColumn(0, _T("������Ŀ"), LVCFMT_LEFT, 100, 0);
	m_listItem.InsertColumn(1, _T("���Ѽ۸�"), LVCFMT_LEFT, 80, 1);
	m_listItem.InsertColumn(2, _T("��Ա�۸�"), LVCFMT_LEFT, 80, 2);
	_InitList();
	m_pSelItem = new FindItem;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgFindItem::_InitList(void)
{
	_DeleteAllListItem(&m_listItem);
	if (m_strCardType.IsEmpty())
		return;
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	// ����list
	CAdoRecordSet* pSetRead = new CAdoRecordSet(pMainDlg->m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL;
	sSQL.Format(_T("SELECT ��ĿID,��Ŀ����,��ͨ�� AS Cost,%s AS Price FROM ������Ŀ"), m_strCardType);
	if (!m_sFindItem.IsEmpty())
	{
		sSQL.AppendFormat(_T(" WHERE ���ƴ�� LIKE '%s%%'"), m_sFindItem);
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
		pSetRead->GetCollect(_T("��ĿID"), iID);
		pItem->iID = iID;
		pSetRead->GetCollect(_T("��Ŀ����"), sItemName);
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
		// ������һ��
		pSetRead->MoveNext();
		iCount++;
	}
	pSetRead->Close();
	m_listItem.SetItemState(0, 0xFFFF, LVIS_SELECTED);

	// ɾ��
	DELETE_POINTER(pSetRead);
}

FindItem* CDlgFindItem::_GetCurSelItem(void)
{
	POSITION pos = m_listItem.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		AfxMessageBox(_T("��ѡ������Ŀ��"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedOk();
	*pResult = 0;
}

void CDlgFindItem::OnEnChangeEditFind()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData())
	{
		_InitList();
	}
}
