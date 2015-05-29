#pragma once

// CDlgConsume �Ի���

class CCustomGridCtrl;
class CAdoConnection;
class CDlgConsume : public CDialog
{
	DECLARE_DYNAMIC(CDlgConsume)

public:
	CDlgConsume(int iMemberID = DEFUALT_MEMBERID, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConsume();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONSUME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// ����ؼ�
	CCustomGridCtrl* m_pGridCtrl;
	// ��������
	CString m_sConsumeDate;
	// �˿�ID
	int m_iMemberID;
	// ��Ա����
	CString m_sName;
	// ��Ա����
	CString m_sCardNo;
	// ��ԱID
	int m_iCardID;
	// ���
	CString m_sBalance;
	// �������Ѻ����
	CString m_sNowMoney;
	// ������
	CString m_sCardType;
	// ��ʼ������
	void _InitGrid(void);
	// ��ʼ����������
	void _InitGridFromDB(void);
	// ���������ı�
	void _SetGridText(int iRow, int iCol, LPCTSTR sText, BOOL bModified = TRUE);
	// ���������ı�����
	void _SetGridCenter(int iRow, int iCol);
	// �������ѵ�
	BOOL _SaveConsume(void);
	// �������ѵ���ϸ
	BOOL _SaveConsume_Detail(int iID);
	// �õ���һ�����ѵ�ID
	BOOL _GetNextConsumeID(int& iMaxID);
	// ���ݻ�����Ŀ���Ƶõ�ID
	BOOL _GetItemIDByName(int& iItemID, const CString& sItemName);
	// ��������-Title
	void _InitGrid_Title(int& iBeginRow);
	// ��������-Head
	void _InitGrid_Head(int& iBeginRow);
	// ��������-Body
	void _InitGrid_Body(int& iBeginRow);
	// ��������-Tail
	void _InitGrid_Tail(int& iBeginRow);
	// ������
	void _CalcMoney(void);
	// ��ӦGrid��Ϣ
	void _ProcMsgGrid(int iRow, int iCol);
	// ����һЩ������ı�
	void _SetDefaultText(void);
	// ɾ��һ��
	void _DeleteGrid(int iRow);
	// ��ӡ��������
	void _PrintGrid(void);
public:
	CAdoConnection* m_pConSPA;		// Ado���ݿ������ڶ�
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
