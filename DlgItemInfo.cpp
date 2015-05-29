// DlgItemInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgItemInfo.h"
#include "./Ado/Ado.h"

// CDlgItemInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgItemInfo, CDialog)

CDlgItemInfo::CDlgItemInfo(int iType /*= 0*/, int iItemID /*= 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemInfo::IDD, pParent)
	, m_iItemID(iItemID)
	, m_iOperationType(iType)
	, m_sItemName(_T(""))
	, m_sPY(_T(""))
	, m_sCardPT(_T(""))
	, m_sCardY(_T(""))
	, m_sCardJ(_T(""))
	, m_sCardBJ(_T(""))
	, m_strCheck(_T(""))
{
	m_pConSPA = NULL;
}

CDlgItemInfo::~CDlgItemInfo()
{
}

void CDlgItemInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ITEMNAME, m_sItemName);
	DDX_Text(pDX, IDC_EDIT_PY, m_sPY);
	DDX_Text(pDX, IDC_EDIT_CARDPT, m_sCardPT);
	DDX_Text(pDX, IDC_EDIT_CARDY, m_sCardY);
	DDX_Text(pDX, IDC_EDIT_CARDJ, m_sCardJ);
	DDX_Text(pDX, IDC_EDIT_CARDBJ, m_sCardBJ);
}


BEGIN_MESSAGE_MAP(CDlgItemInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgItemInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgItemInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgItemInfo ��Ϣ�������

void CDlgItemInfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!_SaveItem())
	{
		//AfxMessageBox(_T("����ʧ�ܣ�"));
		return;
	}
	OnOK();
}

void CDlgItemInfo::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (_TextIsChanged())
	{
		int iResult = AfxMessageBox(_T("��Ϣ�Ѿ����ģ��˳�ǰ�Ƿ񱣴棿"), MB_YESNOCANCEL);
		if (IDYES == iResult)
		{
			if (!_SaveItem())
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

BOOL CDlgItemInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	_InitDlg();
	// ������޸ģ��������޷�����
	if (1 == m_iOperationType)
	{
		GetDlgItem(IDC_EDIT_ITEMNAME)->EnableWindow(FALSE);
	}
	_CreateCheckString(m_strCheck);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CDlgItemInfo::_CreateCheckString(CString& strCheck)
{
	// ��Ŀ����
	strCheck.Append(m_sItemName);
	// ���ƴ��
	strCheck.Append(m_sPY);
	// ��ͨ��
	strCheck.Append(m_sCardPT);
	// ����
	strCheck.Append(m_sCardY);
	// ��
	strCheck.Append(m_sCardJ);
	// �׽�
	strCheck.Append(m_sCardBJ);
}

void CDlgItemInfo::_InitDlg(void)
{
	// ��������=0����ʾ����
	if (0 == m_iOperationType)
		return;
	// �������ݿ�
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL;
	sSQL.Format(_T("SELECT * FROM ������Ŀ WHERE ��ĿID=%d"), m_iItemID);
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	if (pSetRead->IsEOF())
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	pSetRead->GetCollect(_T("��Ŀ����"), m_sItemName);
	pSetRead->GetCollect(_T("���ƴ��"), m_sPY);
	double dBalance;
	pSetRead->GetCollect(_T("��ͨ��"), dBalance);
	m_sCardPT.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("����"), dBalance);
	m_sCardY.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("��"), dBalance);
	m_sCardJ.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("�׽�"), dBalance);
	m_sCardBJ.Format(_T("%.2f"), dBalance);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	UpdateData(FALSE);
}

BOOL CDlgItemInfo::_TextIsValid(void)
{
	if (!UpdateData())
		return FALSE;
	// �����ж�����
	if (m_sItemName.IsEmpty())
	{
		AfxMessageBox(_T("������Ŀ���Ʋ���Ϊ�գ�"), MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_EDIT_ITEMNAME)->SetFocus();
		return FALSE;
	}
	// �ж���ͨ��
	if (!IsNum_Dot(m_sCardPT) || !IsNum_Dot(m_sCardY) || 
		!IsNum_Dot(m_sCardJ) || !IsNum_Dot(m_sCardBJ))
	{
		AfxMessageBox(_T("���󣺽���ʽӦ��Ϊ'100.01'��"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgItemInfo::_TextIsChanged(void)
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

BOOL CDlgItemInfo::_SaveItem(void)
{
	// ������Ч��������������ݿⱣ�棬ֱ�ӷ��ر���ʧ��
	if (!_TextIsValid())
		return FALSE;
	// û���޸Ĺ������߱������������������ݿⱣ�棬ֱ�ӷ��ر���ɹ�
	if (!_TextIsChanged())
		return TRUE;
	_CreateCheckString(m_strCheck);
	// ��������������Ȳ���˿���Ϣ�ͻ�Ա��
	if (0 == m_iOperationType)
	{
		if (!_SaveItem_Add())
			return FALSE;
	}
	// Ȼ�����
	m_pConSPA->BeginTrans();
	CString sSQL;
	sSQL.Format(_T("UPDATE ������Ŀ SET ��Ŀ����='%s',���ƴ��='%s',��ͨ��=%s,\
				   ����=%s,��=%s,�׽�=%s WHERE ��ĿID=%d"),
				   m_sItemName, m_sPY, m_sCardPT, m_sCardY, m_sCardJ, m_sCardBJ, m_iItemID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	return TRUE;
}

BOOL CDlgItemInfo::_SaveItem_Add(void)
{
	// ����ȷ����ĿID
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return FALSE;
	CString sSQL = _T("SELECT MAX(��ĿID)+1 AS MaxID FROM ������Ŀ");
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return FALSE;
	}
	if (pSetRead->IsEOF())
	{
		DELETE_POINTER(pSetRead);
		return FALSE;
	}
	pSetRead->GetCollect(_T("MaxID"), m_iItemID);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	// ��ʼ����
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("INSERT INTO ������Ŀ(��ĿID) VALUES (%d)"), m_iItemID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	return TRUE;
}
