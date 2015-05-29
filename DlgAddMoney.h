#pragma once


// CDlgAddMoney 对话框

class CDlgAddMoney : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddMoney)

public:
	CDlgAddMoney(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddMoney();

// 对话框数据
	enum { IDD = IDD_DLG_ADDMONEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sMoney;
};
