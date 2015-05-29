#pragma once


// CDlgItemInfo �Ի���
class CAdoConnection;

class CDlgItemInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgItemInfo)

public:
	CDlgItemInfo(int iType = 0, int iItemID = 0, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgItemInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_ITEMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CAdoConnection* m_pConSPA;		// Ado���ݿ�������ָ��
	// ��ĿID
	int m_iItemID; 
	// �������� 0 ����; 1 �޸�;
	int m_iOperationType;

	CString m_sItemName;
	CString m_sPY;
	CString m_sCardPT;
	CString m_sCardY;
	CString m_sCardJ;
	CString m_sCardBJ;
private:
	// ���ı�
	CString m_strCheck;
	// ���ı�����װ
	void _CreateCheckString(CString& strCheck);
	// ��ʼ��
	void _InitDlg(void);
	// ��֤���ı��Ƿ�仯�����仯������Ҫ����
	BOOL _TextIsChanged(void);
	// ��֤�ı�����Ч��
	BOOL _TextIsValid(void);
	// ������Ŀ
	BOOL _SaveItem(void);
	// ������Ŀ
	BOOL _SaveItem_Add(void);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
