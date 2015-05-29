#pragma once
#include "afxcmn.h"

typedef struct tagCareItem
{
	int iItem;			// ��ǰλ��
	int iID;			// ID
	CString sItemName;	// ��Ŀ����
	tagCareItem()
	{
		iItem = -1;
		iID = -1;
		sItemName = _T("");
	}
} CareItem;

// CDlgCareItem �Ի���

class CDlgCareItem : public CDialog
{
	DECLARE_DYNAMIC(CDlgCareItem)

public:
	CDlgCareItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCareItem();

// �Ի�������
	enum { IDD = IDD_DLG_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listItem;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void OnCancel();
private:
	// ��ʼ��list
	void _InitList(void);
	// �õ���ǰѡ�е���Ϣ
	CareItem* _GetCurSelItem(void);
	// ���û�Ա���Ի���
	void _CallItem(int iType, int iID = 0);
	// ɾ�����е�ָ��
	void _DeleteAllListItem(CListCtrl* pList);
};
