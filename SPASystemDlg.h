// SPASystemDlg.h : 头文件
//

#pragma once

class CAdoConnection;
// CSPASystemDlg 对话框
class CSPASystemDlg : public CDialog
{
// 构造
public:
	CSPASystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SPASYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnMember();
	afx_msg void OnBnClickedBtnCard();
	afx_msg void OnBnClickedBtnItem();
	DECLARE_MESSAGE_MAP()
public:
	// 创建ADO连接
	BOOL CreateADO(void);
	// 释放ADO连接
	BOOL ReleaseADO(void);
	CAdoConnection* m_pConSPA;		// Ado数据库连接类指针
private:
	// 创建ADO连接
	CAdoConnection* _CreaterAdoConnection(void);
	// 调用会员卡对话框
	void _CallCard(int iType, int iMemberID = 0);
	// 通过会员卡号或者会员姓名定位顾客ID
	int _GetMemberID(int iType, const CString& strInput);
public:
	afx_msg void OnCardAll();
	afx_msg void OnCardConsume();
	afx_msg void OnSystemCardtype();
	afx_msg void OnSystemItem();
	virtual void OnCancel();
	afx_msg void OnCardInfo();
	afx_msg void OnCardNew();
protected:
	virtual void OnOK();
};
