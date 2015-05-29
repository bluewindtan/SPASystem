#pragma once
#include "afxcmn.h"

typedef struct tagCareItem
{
	int iItem;			// 当前位置
	int iID;			// ID
	CString sItemName;	// 项目名称
	tagCareItem()
	{
		iItem = -1;
		iID = -1;
		sItemName = _T("");
	}
} CareItem;

// CDlgCareItem 对话框

class CDlgCareItem : public CDialog
{
	DECLARE_DYNAMIC(CDlgCareItem)

public:
	CDlgCareItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCareItem();

// 对话框数据
	enum { IDD = IDD_DLG_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 初始化list
	void _InitList(void);
	// 得到当前选中的信息
	CareItem* _GetCurSelItem(void);
	// 调用会员卡对话框
	void _CallItem(int iType, int iID = 0);
	// 删除所有的指针
	void _DeleteAllListItem(CListCtrl* pList);
};
