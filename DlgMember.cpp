// DlgMember.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgMember.h"
#include "./Ado/Ado.h"
#include "DlgAddMoney.h"

// CDlgMember �Ի���
const LPCTSTR g_sDefaultCode = _T("000000");

IMPLEMENT_DYNAMIC(CDlgMember, CDialog)

CDlgMember::CDlgMember(int iType /*= 0*/, int iMemberID /*= 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMember::IDD, pParent)
	, m_sName(_T(""))
	, m_sPinYin(_T(""))
	, m_sBirth(_T(""))
	, m_sAddress(_T(""))
	, m_sCode(g_sDefaultCode)
	, m_sEMail(_T(""))
	, m_sMobile(_T(""))
	, m_sPhone(_T(""))
	, m_sCardNo(_T(""))
	, m_sBalance(_T("0.00"))
	, m_iOperationType(iType)
	, m_iMemberID(iMemberID)
	, m_sCardDate(_T(""))
	, m_bSaved(FALSE)
	, m_strCheck(_T(""))
{
	m_pConSPA = NULL;
}

CDlgMember::~CDlgMember()
{
}

void CDlgMember::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_PY, m_sPinYin);
	DDX_Text(pDX, IDC_EDIT_BIRTH, m_sBirth);
	DDX_Text(pDX, IDC_EDIT_ADRESS, m_sAddress);
	DDX_Text(pDX, IDC_EDIT_CODE, m_sCode);
	DDX_Text(pDX, IDC_EDIT_EMail, m_sEMail);
	DDX_Text(pDX, IDC_EDIT_MOBILE, m_sMobile);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_sPhone);
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_sCardNo);
	DDX_Text(pDX, IDC_EDIT_MONEY, m_sBalance);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Text(pDX, IDC_EDIT_CARDDATE, m_sCardDate);
	DDV_MaxChars(pDX, m_sCode, 6);
	DDV_MaxChars(pDX, m_sAddress, 100);
	DDV_MaxChars(pDX, m_sName, 8);
	DDV_MaxChars(pDX, m_sPinYin, 30);
	DDV_MaxChars(pDX, m_sEMail, 40);
	DDV_MaxChars(pDX, m_sMobile, 20);
	DDV_MaxChars(pDX, m_sPhone, 20);
}


BEGIN_MESSAGE_MAP(CDlgMember, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgMember::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgMember::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_ADDMONEY, &CDlgMember::OnBnClickedBtnAddmoney)
END_MESSAGE_MAP()


// CDlgMember ��Ϣ�������

void CDlgMember::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!_SaveMember())
	{
		//AfxMessageBox(_T("����ʧ�ܣ�"));
		return;
	}
	OnOK();
}

void CDlgMember::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (_TextIsChanged())
	{
		int iResult = AfxMessageBox(_T("��Ϣ�Ѿ����ģ��˳�ǰ�Ƿ񱣴棿"), MB_YESNOCANCEL);
		if (IDYES == iResult)
		{
			if (!_SaveMember())
			{
				//AfxMessageBox(_T("����ʧ�ܣ�"));
				return;
			}
		}
		else if (IDNO == iResult)
		{
		}
		else if (IDCANCEL == iResult)
		{
			return; // ���رնԻ���
		}
	}
	
	OnCancel();
}

void CDlgMember::OnBnClickedBtnAddmoney()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgAddMoney dlg;
	if (IDOK == dlg.DoModal())
	{
		_AddMoney(dlg.m_sMoney);
	}
}

BOOL CDlgMember::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	_InitDlg();
	// ������޸ģ��������޷�����
	if (1 == m_iOperationType)
	{
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	}
	m_bSaved = FALSE;
	_CreateCheckString(m_strCheck);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgMember::_CreateCheckString(CString& strCheck)
{
	strCheck = _T("");
	// ��Ա����
	strCheck.Append(m_sName);
	// ���ƴ��
	strCheck.Append(m_sPinYin);
	// ����
	strCheck.Append(m_sBirth);
	// ��ַ
	strCheck.Append(m_sAddress);
	// �ʱ�
	strCheck.Append(m_sCode);
	// EMail
	strCheck.Append(m_sEMail);
	// �ֻ�
	strCheck.Append(m_sMobile);
	// �绰
	strCheck.Append(m_sPhone);
	// ��Ա����
	strCheck.Append(m_sCardNo);
	// ���
	strCheck.Append(m_sBalance);
	// ������
	CString sComboType;
	m_comboType.GetWindowText(sComboType);
	strCheck.Append(sComboType);
	// ��������
	strCheck.Append(m_sCardDate);
}

void CDlgMember::_InitDlg(void)
{
	// �������ݿ�
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL = _T("SELECT * FROM ������ ORDER BY ����ID");
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	int iIndex = 0;
	CString sCardName;
	int iCurrentID;
	while(!pSetRead->IsEOF())
	{
		pSetRead->GetCollect(_T("������"), sCardName);
		pSetRead->GetCollect(_T("����ID"), iCurrentID);
		m_comboType.InsertString(iIndex, sCardName);
		m_comboType.SetItemData(iIndex, (DWORD_PTR)iCurrentID);
		// ������һ��
		pSetRead->MoveNext();
		iIndex++;
	}
	m_comboType.SetCurSel(iIndex-1);
	pSetRead->Close();
	// ��������=0����ʾ����
	if (0 == m_iOperationType)
	{
		_AddCard(pSetRead);
	}
	else if (1 == m_iOperationType) // �޸�
	{
		_UpdateCard(pSetRead);
	}
	DELETE_POINTER(pSetRead);
}

void CDlgMember::_AddCard(CAdoRecordSet* pSetRead)
{
	if (!pSetRead)
		return;
	CTime time = CTime::GetCurrentTime();
	m_sCardDate.Format(_T("%4d-%2d-%2d"), time.GetYear(), time.GetMonth(), time.GetDay());
	// �Զ����ɿ���
	CString sSQL;
	sSQL.Format(_T("SELECT ��Ա���� FROM ��Ա�� WHERE ��Ա���� LIKE '%d%%' ORDER BY ��Ա���� DESC"), 
				time.GetYear());
	if(!pSetRead->Open(sSQL))
	{
		return;
	}
	CString sMaxID;
	if (!pSetRead->IsEOF())
	{
		pSetRead->GetCollect(_T("��Ա����"), sMaxID);
		int iCardNO = _ttoi(sMaxID.Mid(4));
		m_sCardNo.Format(_T("%4d%04d"), time.GetYear(), iCardNO+1);
	}
	else
	{
		m_sCardNo.Format(_T("%4d0001"), time.GetYear());
	}
	pSetRead->Close();
	UpdateData(FALSE);
}

void CDlgMember::_UpdateCard(CAdoRecordSet* pSetRead)
{
	if (!pSetRead)
		return;
	CString sSQL;
	// ���ع˿���Ϣ
	sSQL.Format(_T("SELECT * FROM �˿���Ϣ WHERE �˿�ID=%d"), m_iMemberID);
	if(!pSetRead->Open(sSQL))
	{
		return;
	}
	if (pSetRead->IsEOF())
	{
		CString sMsg;
		sMsg.Format(_T("�˿�ID����'%d'�Ĺ˿Ͳ�����!"), m_iMemberID);
		TRACE(sMsg);
		return;
	}
	pSetRead->GetCollect(_T("����"), m_sName);
	pSetRead->GetCollect(_T("��ַ"), m_sAddress);
	int iCode = 0;
	pSetRead->GetCollect(_T("��������"), iCode);
	m_sCode.Format(_T("%06d"), iCode);
	pSetRead->GetCollect(_T("�����ʼ�"), m_sEMail);
	pSetRead->GetCollect(_T("��ϵ�绰"), m_sPhone);
	pSetRead->GetCollect(_T("����"), m_sBirth);
	pSetRead->GetCollect(_T("�ֻ�"), m_sMobile);
	pSetRead->GetCollect(_T("���ƴ��"), m_sPinYin);
	pSetRead->Close();
	// ���ػ�Ա����Ϣ
	sSQL.Format(_T("SELECT * FROM QueryCard WHERE �˿�ID=%d"), m_iMemberID);
	if(!pSetRead->Open(sSQL))
	{
		return;
	}
	if (pSetRead->IsEOF())
	{
		CString sMsg;
		sMsg.Format(_T("�˿�ID����'%d'�Ĺ˿Ͳ�����!"), m_iMemberID);
		TRACE(sMsg);
		return;
	}
	pSetRead->GetCollect(_T("��Ա����"), m_sCardNo);
	CString sCardName;
	pSetRead->GetCollect(_T("������"), sCardName);
	int iIndex = m_comboType.FindString(0, sCardName);
	m_comboType.SetCurSel(iIndex);
	double dBalance;
	pSetRead->GetCollect(_T("���"), dBalance);
	m_sBalance.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("��������"), m_sCardDate);
	pSetRead->Close();

	UpdateData(FALSE);
}

void CDlgMember::_AddMoney(CString& sAddMoney)
{
	LPCTSTR strAddMoney = sAddMoney.GetBuffer(sAddMoney.GetLength());
	LPCTSTR strBalance = m_sBalance.GetBuffer(m_sBalance.GetLength());
	LPTSTR strTemp;
	double dAdd = _tcstod(strAddMoney, &strTemp); // ���ӵ�ֵ
	double dBalance = _tcstod(strBalance, &strTemp); // ���ӵ�ֵ
	sAddMoney.ReleaseBuffer();
	m_sBalance.ReleaseBuffer();
	m_sBalance.Format(_T("%.2f"), dAdd+dBalance);
	GetDlgItem(IDC_EDIT_MONEY)->SetWindowText(m_sBalance);
}

BOOL CDlgMember::_SaveMember_Add(void)
{
	CString sSQL;
	if (!_GetMemberIDFromName(m_sName, m_iMemberID) || 0 != m_iMemberID)
	{
		sSQL.Format(_T("�˿�'%s'�Ѿ���Ϊ��Ա�����ʵ��"), m_sName);
		AfxMessageBox(sSQL);
		return FALSE;
	}
	if (!_GetNextMemberID(m_iMemberID))
	{
		sSQL.Format(_T("�˿�'%s'�Ѿ���Ϊ��Ա�����ʵ��"), m_sName);
		AfxMessageBox(sSQL);
		return FALSE;
	}
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("INSERT INTO �˿���Ϣ (�˿�ID,����) VALUES (%d,'%s')"), m_iMemberID, m_sName);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	sSQL.Format(_T("INSERT INTO ��Ա�� (��Ա����,�˿�ID) VALUES ('%s',%d)"),	m_sCardNo, m_iMemberID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	m_bSaved = TRUE;
	return TRUE;
}

BOOL CDlgMember::_SaveMember(void)
{
	// ������Ч��������������ݿⱣ�棬ֱ�ӷ��ر���ʧ��
	if (!_TextIsValid())
		return FALSE;
	// û���޸Ĺ������߱������������������ݿⱣ�棬ֱ�ӷ��ر���ɹ�
	if (!_TextIsChanged())
		return TRUE;
	_CreateCheckString(m_strCheck);
	// ������Ϣ
	CString sSQL;
	// ��������������Ȳ���˿���Ϣ�ͻ�Ա��
	if (0 == m_iOperationType)
	{
		if (!_SaveMember_Add())
			return FALSE;
	}
	// Ȼ�����
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("UPDATE �˿���Ϣ SET ��ַ='%s',��������=%s,�����ʼ�='%s',\
				   ��ϵ�绰='%s',����='%s',�ֻ�='%s',���ƴ��='%s' WHERE �˿�ID=%d"),
				   m_sAddress, m_sCode.IsEmpty()?g_sDefaultCode:m_sCode, m_sEMail, 
				   m_sPhone, m_sBirth, m_sMobile, m_sPinYin, m_iMemberID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	CString strComboType;
	int iIndex = m_comboType.GetCurSel();
	int iCardID = (int)m_comboType.GetItemData(iIndex);
	sSQL.Format(_T("UPDATE ��Ա�� SET �˿�ID=%d,������ID=%d,���=%s,\
				   ��������='%s' WHERE ��Ա����='%s'"),
				   m_iMemberID, iCardID, m_sBalance, 
				   m_sCardDate, m_sCardNo);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	m_bSaved = TRUE;
	return m_bSaved;
}

BOOL CDlgMember::_GetMemberIDFromName(const CString& strName, int& iMemberID)
{
	CAdoRecordSet* pRecordset = new CAdoRecordSet(m_pConSPA);
	if (!pRecordset)
		return FALSE;
	CString sSQL;
	sSQL.Format(_T("SELECT * FROM �˿���Ϣ WHERE ����='%s'"), strName);
	if(!pRecordset->Open(sSQL))
	{
		DELETE_POINTER(pRecordset);
		return FALSE;
	}
	if (!pRecordset->IsEOF())
	{
		pRecordset->GetCollect(_T("�˿�ID"), iMemberID);
	}
	pRecordset->Close();

	DELETE_POINTER(pRecordset);
	return TRUE;
}

BOOL CDlgMember::_GetNextMemberID(int& iMemberID)
{
	CAdoRecordSet* pRecordset = new CAdoRecordSet(m_pConSPA);
	if (!pRecordset)
		return FALSE;
	CString sSQL = _T("SELECT MAX(�˿�ID) AS MAXID FROM �˿���Ϣ");
	if(!pRecordset->Open(sSQL))
	{
		DELETE_POINTER(pRecordset);
		return FALSE;
	}
	if (!pRecordset->IsEOF())
	{
		pRecordset->GetCollect(_T("MAXID"), iMemberID);
		ASSERT(iMemberID>0);
		iMemberID++;
	}
	else
	{
		iMemberID = 1;
	}
	pRecordset->Close();

	DELETE_POINTER(pRecordset);
	return TRUE;
}

BOOL CDlgMember::_TextIsChanged(void)
{
	if (!UpdateData())
		return FALSE;
	BOOL bChanged = FALSE;
	CString sTemp;
	_CreateCheckString(sTemp);
	if (0 != sTemp.Compare(m_strCheck))
	{
		bChanged = TRUE;
	}

	return bChanged;
}

BOOL CDlgMember::_TextIsValid(void)
{
	if (!UpdateData())
		return FALSE;
	// �����ж�����
	if (m_sName.IsEmpty())
	{
		AfxMessageBox(_T("������������Ϊ�գ�"), MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_EDIT_NAME)->SetFocus();
		return FALSE;
	}
	// �ж�����
	if (!IsDateFormat(m_sBirth))
	{
		AfxMessageBox(_T("�������ո�ʽӦ��Ϊ'2008-08-08'��"), MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_EDIT_BIRTH)->SetFocus();
		return FALSE;
	}

	return TRUE;
}