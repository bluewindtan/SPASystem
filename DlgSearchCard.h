#pragma once


// CDlgSearchCard �Ի���

class CDlgSearchCard : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchCard)

public:
	CDlgSearchCard(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSearchCard();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �����ַ���
	CString m_sInput;
	// ���ҵ����ͣ�Ĭ��ֵ0 ��Ա����; 1 ��Ա����; ��
	int m_iInputType;
private:
	// ��ʼ����������
	void _InitRadioType(int iType);
	// �õ���������
	int _GetRadioType(void);
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
