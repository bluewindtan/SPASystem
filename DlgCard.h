#pragma once
#include "afxcmn.h"

typedef struct tagMemberCard
{
	int iItem;			// ��ǰλ��
	int iMemberID;		// �˿�ID
	CString sCardNo;	// ��Ա����
	tagMemberCard()
	{
		iItem = -1;
		iMemberID = -1;
		sCardNo = _T("");
	}
} MemberCard;

// CDlgCard �Ի���

class CDlgCard : public CDialog
{
	DECLARE_DYNAMIC(CDlgCard)

public:
	CDlgCard(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCard();

// �Ի�������
	enum { IDD = IDD_DLG_CARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listCard;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnNMDblclkListCard(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void OnCancel();
private:
	// ��ʼ��list
	void _InitList(void);
	// ���û�Ա���Ի���
	void _CallCard(int iType, int iMemberID = 0);
	// �õ���ǰѡ�еĹ˿���Ϣ
	MemberCard* _GetCurSelMember(void);
	// ɾ�����е�ָ��
	void _DeleteAllListItem(CListCtrl* pList);
};
