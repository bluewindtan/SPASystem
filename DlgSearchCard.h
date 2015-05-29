#pragma once


// CDlgSearchCard 对话框

class CDlgSearchCard : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchCard)

public:
	CDlgSearchCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearchCard();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 输入字符串
	CString m_sInput;
	// 查找的类型（默认值0 会员卡号; 1 会员姓名; ）
	int m_iInputType;
private:
	// 初始化查找类型
	void _InitRadioType(int iType);
	// 得到查找类型
	int _GetRadioType(void);
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
