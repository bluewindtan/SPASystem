#pragma once
#include "afxcmn.h"

typedef struct tagMemberCard
{
	int iItem;			// 当前位置
	int iMemberID;		// 顾客ID
	CString sCardNo;	// 会员卡号
	tagMemberCard()
	{
		iItem = -1;
		iMemberID = -1;
		sCardNo = _T("");
	}
} MemberCard;

// CDlgCard 对话框

class CDlgCard : public CDialog
{
	DECLARE_DYNAMIC(CDlgCard)

public:
	CDlgCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCard();

// 对话框数据
	enum { IDD = IDD_DLG_CARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 初始化list
	void _InitList(void);
	// 调用会员卡对话框
	void _CallCard(int iType, int iMemberID = 0);
	// 得到当前选中的顾客信息
	MemberCard* _GetCurSelMember(void);
	// 删除所有的指针
	void _DeleteAllListItem(CListCtrl* pList);
};
