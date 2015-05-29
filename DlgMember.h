#pragma once
#include "afxwin.h"


// CDlgMember �Ի���
class CAdoConnection;
class CAdoRecordSet;

class CDlgMember : public CDialog
{
	DECLARE_DYNAMIC(CDlgMember)

public:
	CDlgMember(int iType = 4, int iMemberID = DEFUALT_MEMBERID, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMember();

	// �Ի�������
	enum { IDD = IDD_DLG_MEMBER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnAddmoney();
	virtual BOOL OnInitDialog();
public:
	// ��Ա����
	CString m_sName;
	// ���ƴ��
	CString m_sPinYin;
	// ����
	CString m_sBirth;
	// ��ַ
	CString m_sAddress;
	// �ʱ�
	CString m_sCode;
	// EMail
	CString m_sEMail;
	// �ֻ�
	CString m_sMobile;
	// �绰
	CString m_sPhone;
	// ��Ա����
	CString m_sCardNo;
	// ���
	CString m_sBalance;
	// ������
	CComboBox m_comboType;
	// �������� 0 ����; 1 �޸�;
	int m_iOperationType;
	// ��ԱID
	int m_iMemberID;
	// ��������
	CString m_sCardDate;
private:
	// ���ı�
	CString m_strCheck;
	// ���ı�����װ
	void _CreateCheckString(CString& strCheck);
	// ��֤���ı��Ƿ�仯�����仯������Ҫ����
	BOOL _TextIsChanged(void);
	// ��֤�ı�����Ч��
	BOOL _TextIsValid(void);
private:
	// ��ʼ��
	void _InitDlg(void);
	// ������
	void _AddCard(CAdoRecordSet* pSetRead);
	// �޸Ŀ�
	void _UpdateCard(CAdoRecordSet* pSetRead);
	// �������
	void _AddMoney(CString& sAddMoney);
	// ������Ϣ
	BOOL _SaveMember(void);
	// ������Ա
	BOOL _SaveMember_Add(void);
	// �ӹ˿������õ�ID
	BOOL _GetMemberIDFromName(const CString& strName, int& iMemberID);
	// �õ���һ���˿�ID
	BOOL _GetNextMemberID(int& iMemberID);
public:
	CAdoConnection* m_pConSPA;		// Ado���ݿ�������ָ��
	BOOL m_bSaved;	// �����
};
