#pragma once
#include "afxwin.h"


// CDlgMember 对话框
class CAdoConnection;
class CAdoRecordSet;

class CDlgMember : public CDialog
{
	DECLARE_DYNAMIC(CDlgMember)

public:
	CDlgMember(int iType = 4, int iMemberID = DEFUALT_MEMBERID, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMember();

	// 对话框数据
	enum { IDD = IDD_DLG_MEMBER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnAddmoney();
	virtual BOOL OnInitDialog();
public:
	// 会员姓名
	CString m_sName;
	// 快捷拼音
	CString m_sPinYin;
	// 生日
	CString m_sBirth;
	// 地址
	CString m_sAddress;
	// 邮编
	CString m_sCode;
	// EMail
	CString m_sEMail;
	// 手机
	CString m_sMobile;
	// 电话
	CString m_sPhone;
	// 会员卡号
	CString m_sCardNo;
	// 余额
	CString m_sBalance;
	// 卡类型
	CComboBox m_comboType;
	// 操作类型 0 新增; 1 修改;
	int m_iOperationType;
	// 会员ID
	int m_iMemberID;
	// 开卡日期
	CString m_sCardDate;
private:
	// 总文本
	CString m_strCheck;
	// 总文本的组装
	void _CreateCheckString(CString& strCheck);
	// 验证总文本是否变化，若变化，则需要保存
	BOOL _TextIsChanged(void);
	// 验证文本的有效性
	BOOL _TextIsValid(void);
private:
	// 初始化
	void _InitDlg(void);
	// 新增卡
	void _AddCard(CAdoRecordSet* pSetRead);
	// 修改卡
	void _UpdateCard(CAdoRecordSet* pSetRead);
	// 增加余额
	void _AddMoney(CString& sAddMoney);
	// 保存信息
	BOOL _SaveMember(void);
	// 新增会员
	BOOL _SaveMember_Add(void);
	// 从顾客姓名得到ID
	BOOL _GetMemberIDFromName(const CString& strName, int& iMemberID);
	// 得到下一个顾客ID
	BOOL _GetNextMemberID(int& iMemberID);
public:
	CAdoConnection* m_pConSPA;		// Ado数据库连接类指针
	BOOL m_bSaved;	// 保存过
};
