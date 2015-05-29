#pragma once
#include "afxcmn.h"


// CDlgFindItem 对话框
typedef struct tagFindItem
{
	int iItem;			// 当前位置
	int iID;			// ID
	CString sItemName;	// 项目名称
	CString sCost;		// 原来价格
	CString sMoney;		// 优惠后价格
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
	CDlgFindItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFindItem();

// 对话框数据
	enum { IDD = IDD_DIALOG_FINDITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sFindItem;
	CListCtrl m_listItem;
	CString m_strCardType;
	FindItem* m_pSelItem;
private:
	// 初始化list
	void _InitList(void);
	// 得到当前选中的信息
	FindItem* _GetCurSelItem(void);
	// 删除所有的指针
	void _DeleteAllListItem(CListCtrl* pList);
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnEnChangeEditFind();
};
