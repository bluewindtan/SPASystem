#pragma once


// CDlgItemInfo 对话框
class CAdoConnection;

class CDlgItemInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgItemInfo)

public:
	CDlgItemInfo(int iType = 0, int iItemID = 0, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgItemInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_ITEMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CAdoConnection* m_pConSPA;		// Ado数据库连接类指针
	// 项目ID
	int m_iItemID; 
	// 操作类型 0 新增; 1 修改;
	int m_iOperationType;

	CString m_sItemName;
	CString m_sPY;
	CString m_sCardPT;
	CString m_sCardY;
	CString m_sCardJ;
	CString m_sCardBJ;
private:
	// 总文本
	CString m_strCheck;
	// 总文本的组装
	void _CreateCheckString(CString& strCheck);
	// 初始化
	void _InitDlg(void);
	// 验证总文本是否变化，若变化，则需要保存
	BOOL _TextIsChanged(void);
	// 验证文本的有效性
	BOOL _TextIsValid(void);
	// 保存项目
	BOOL _SaveItem(void);
	// 增加项目
	BOOL _SaveItem_Add(void);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
