#pragma once


// CDlgAddMoney �Ի���

class CDlgAddMoney : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddMoney)

public:
	CDlgAddMoney(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddMoney();

// �Ի�������
	enum { IDD = IDD_DLG_ADDMONEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sMoney;
};
