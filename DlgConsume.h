#pragma once

// CDlgConsume 对话框

class CCustomGridCtrl;
class CAdoConnection;
class CDlgConsume : public CDialog
{
	DECLARE_DYNAMIC(CDlgConsume)

public:
	CDlgConsume(int iMemberID = DEFUALT_MEMBERID, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConsume();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONSUME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 网格控件
	CCustomGridCtrl* m_pGridCtrl;
	// 消费日期
	CString m_sConsumeDate;
	// 顾客ID
	int m_iMemberID;
	// 会员姓名
	CString m_sName;
	// 会员卡号
	CString m_sCardNo;
	// 会员ID
	int m_iCardID;
	// 余额
	CString m_sBalance;
	// 本次消费后余额
	CString m_sNowMoney;
	// 卡类型
	CString m_sCardType;
	// 初始化网格
	void _InitGrid(void);
	// 初始化网格数据
	void _InitGridFromDB(void);
	// 设置网格文本
	void _SetGridText(int iRow, int iCol, LPCTSTR sText, BOOL bModified = TRUE);
	// 设置网格文本居中
	void _SetGridCenter(int iRow, int iCol);
	// 保存消费单
	BOOL _SaveConsume(void);
	// 保存消费单明细
	BOOL _SaveConsume_Detail(int iID);
	// 得到下一个消费单ID
	BOOL _GetNextConsumeID(int& iMaxID);
	// 根据护理项目名称得到ID
	BOOL _GetItemIDByName(int& iItemID, const CString& sItemName);
	// 网格数据-Title
	void _InitGrid_Title(int& iBeginRow);
	// 网格数据-Head
	void _InitGrid_Head(int& iBeginRow);
	// 网格数据-Body
	void _InitGrid_Body(int& iBeginRow);
	// 网格数据-Tail
	void _InitGrid_Tail(int& iBeginRow);
	// 计算金额
	void _CalcMoney(void);
	// 响应Grid消息
	void _ProcMsgGrid(int iRow, int iCol);
	// 设置一些必须的文本
	void _SetDefaultText(void);
	// 删除一行
	void _DeleteGrid(int iRow);
	// 打印网格数据
	void _PrintGrid(void);
public:
	CAdoConnection* m_pConSPA;		// Ado数据库类用于读
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
