#pragma once
#include "afxcmn.h"


// CDlgFindItem �Ի���
typedef struct tagFindItem
{
	int iItem;			// ��ǰλ��
	int iID;			// ID
	CString sItemName;	// ��Ŀ����
	CString sCost;		// ԭ���۸�
	CString sMoney;		// �Żݺ�۸�
	tagFindItem()
	{
		iItem = -1;
		iID = -1;
		sItemName = _T("");
		sCost = _T("0.00");
		sMoney = _T("0.00");
	}
} FindItem;

class CDlgFindItem : public CDialog
{
	DECLARE_DYNAMIC(CDlgFindItem)

public:
	CDlgFindItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFindItem();

// �Ի�������
	enum { IDD = IDD_DIALOG_FINDITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sFindItem;
	CListCtrl m_listItem;
	CString m_strCardType;
	FindItem* m_pSelItem;
private:
	// ��ʼ��list
	void _InitList(void);
	// �õ���ǰѡ�е���Ϣ
	FindItem* _GetCurSelItem(void);
	// ɾ�����е�ָ��
	void _DeleteAllListItem(CListCtrl* pList);
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnEnChangeEditFind();
};
