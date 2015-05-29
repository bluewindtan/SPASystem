// SPASystemDlg.h : ͷ�ļ�
//

#pragma once

class CAdoConnection;
// CSPASystemDlg �Ի���
class CSPASystemDlg : public CDialog
{
// ����
public:
	CSPASystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SPASYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnMember();
	afx_msg void OnBnClickedBtnCard();
	afx_msg void OnBnClickedBtnItem();
	DECLARE_MESSAGE_MAP()
public:
	// ����ADO����
	BOOL CreateADO(void);
	// �ͷ�ADO����
	BOOL ReleaseADO(void);
	CAdoConnection* m_pConSPA;		// Ado���ݿ�������ָ��
private:
	// ����ADO����
	CAdoConnection* _CreaterAdoConnection(void);
	// ���û�Ա���Ի���
	void _CallCard(int iType, int iMemberID = 0);
	// ͨ����Ա���Ż��߻�Ա������λ�˿�ID
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
