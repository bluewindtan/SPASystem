// DlgMember.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgMember.h"
#include "./Ado/Ado.h"
#include "DlgAddMoney.h"

// CDlgMember 对话框
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


// CDlgMember 消息处理程序

void CDlgMember::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!_SaveMember())
	{
		//AfxMessageBox(_T("保存失败！"));
		return;
	}
	OnOK();
}

void CDlgMember::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (_TextIsChanged())
	{
		int iResult = AfxMessageBox(_T("信息已经更改，退出前是否保存？"), MB_YESNOCANCEL);
		if (IDYES == iResult)
		{
			if (!_SaveMember())
			{
				//AfxMessageBox(_T("保存失败！"));
				return;
			}
		}
		else if (IDNO == iResult)
		{
		}
		else if (IDCANCEL == iResult)
		{
			return; // 不关闭对话框
		}
	}
	
	OnCancel();
}

void CDlgMember::OnBnClickedBtnAddmoney()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgAddMoney dlg;
	if (IDOK == dlg.DoModal())
	{
		_AddMoney(dlg.m_sMoney);
	}
}

BOOL CDlgMember::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_InitDlg();
	// 如果是修改，则姓名无法更改
	if (1 == m_iOperationType)
	{
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	}
	m_bSaved = FALSE;
	_CreateCheckString(m_strCheck);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMember::_CreateCheckString(CString& strCheck)
{
	strCheck = _T("");
	// 会员姓名
	strCheck.Append(m_sName);
	// 快捷拼音
	strCheck.Append(m_sPinYin);
	// 生日
	strCheck.Append(m_sBirth);
	// 地址
	strCheck.Append(m_sAddress);
	// 邮编
	strCheck.Append(m_sCode);
	// EMail
	strCheck.Append(m_sEMail);
	// 手机
	strCheck.Append(m_sMobile);
	// 电话
	strCheck.Append(m_sPhone);
	// 会员卡号
	strCheck.Append(m_sCardNo);
	// 余额
	strCheck.Append(m_sBalance);
	// 卡类型
	CString sComboType;
	m_comboType.GetWindowText(sComboType);
	strCheck.Append(sComboType);
	// 开卡日期
	strCheck.Append(m_sCardDate);
}

void CDlgMember::_InitDlg(void)
{
	// 加载数据库
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL = _T("SELECT * FROM 卡类型 ORDER BY 卡类ID");
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
		pSetRead->GetCollect(_T("卡名称"), sCardName);
		pSetRead->GetCollect(_T("卡类ID"), iCurrentID);
		m_comboType.InsertString(iIndex, sCardName);
		m_comboType.SetItemData(iIndex, (DWORD_PTR)iCurrentID);
		// 继续下一个
		pSetRead->MoveNext();
		iIndex++;
	}
	m_comboType.SetCurSel(iIndex-1);
	pSetRead->Close();
	// 操作类型=0，表示新增
	if (0 == m_iOperationType)
	{
		_AddCard(pSetRead);
	}
	else if (1 == m_iOperationType) // 修改
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
	// 自动生成卡号
	CString sSQL;
	sSQL.Format(_T("SELECT 会员卡号 FROM 会员卡 WHERE 会员卡号 LIKE '%d%%' ORDER BY 会员卡号 DESC"), 
				time.GetYear());
	if(!pSetRead->Open(sSQL))
	{
		return;
	}
	CString sMaxID;
	if (!pSetRead->IsEOF())
	{
		pSetRead->GetCollect(_T("会员卡号"), sMaxID);
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
	// 加载顾客信息
	sSQL.Format(_T("SELECT * FROM 顾客信息 WHERE 顾客ID=%d"), m_iMemberID);
	if(!pSetRead->Open(sSQL))
	{
		return;
	}
	if (pSetRead->IsEOF())
	{
		CString sMsg;
		sMsg.Format(_T("顾客ID等于'%d'的顾客不存在!"), m_iMemberID);
		TRACE(sMsg);
		return;
	}
	pSetRead->GetCollect(_T("姓名"), m_sName);
	pSetRead->GetCollect(_T("地址"), m_sAddress);
	int iCode = 0;
	pSetRead->GetCollect(_T("邮政编码"), iCode);
	m_sCode.Format(_T("%06d"), iCode);
	pSetRead->GetCollect(_T("电子邮件"), m_sEMail);
	pSetRead->GetCollect(_T("联系电话"), m_sPhone);
	pSetRead->GetCollect(_T("生日"), m_sBirth);
	pSetRead->GetCollect(_T("手机"), m_sMobile);
	pSetRead->GetCollect(_T("快捷拼音"), m_sPinYin);
	pSetRead->Close();
	// 加载会员卡信息
	sSQL.Format(_T("SELECT * FROM QueryCard WHERE 顾客ID=%d"), m_iMemberID);
	if(!pSetRead->Open(sSQL))
	{
		return;
	}
	if (pSetRead->IsEOF())
	{
		CString sMsg;
		sMsg.Format(_T("顾客ID等于'%d'的顾客不存在!"), m_iMemberID);
		TRACE(sMsg);
		return;
	}
	pSetRead->GetCollect(_T("会员卡号"), m_sCardNo);
	CString sCardName;
	pSetRead->GetCollect(_T("卡名称"), sCardName);
	int iIndex = m_comboType.FindString(0, sCardName);
	m_comboType.SetCurSel(iIndex);
	double dBalance;
	pSetRead->GetCollect(_T("余额"), dBalance);
	m_sBalance.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("开卡日期"), m_sCardDate);
	pSetRead->Close();

	UpdateData(FALSE);
}

void CDlgMember::_AddMoney(CString& sAddMoney)
{
	LPCTSTR strAddMoney = sAddMoney.GetBuffer(sAddMoney.GetLength());
	LPCTSTR strBalance = m_sBalance.GetBuffer(m_sBalance.GetLength());
	LPTSTR strTemp;
	double dAdd = _tcstod(strAddMoney, &strTemp); // 增加的值
	double dBalance = _tcstod(strBalance, &strTemp); // 增加的值
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
		sSQL.Format(_T("顾客'%s'已经成为会员，请核实！"), m_sName);
		AfxMessageBox(sSQL);
		return FALSE;
	}
	if (!_GetNextMemberID(m_iMemberID))
	{
		sSQL.Format(_T("顾客'%s'已经成为会员，请核实！"), m_sName);
		AfxMessageBox(sSQL);
		return FALSE;
	}
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("INSERT INTO 顾客信息 (顾客ID,姓名) VALUES (%d,'%s')"), m_iMemberID, m_sName);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	sSQL.Format(_T("INSERT INTO 会员卡 (会员卡号,顾客ID) VALUES ('%s',%d)"),	m_sCardNo, m_iMemberID);
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
	// 数据无效，则无需进行数据库保存，直接返回保存失败
	if (!_TextIsValid())
		return FALSE;
	// 没有修改过，或者保存过，则无需进行数据库保存，直接返回保存成功
	if (!_TextIsChanged())
		return TRUE;
	_CreateCheckString(m_strCheck);
	// 保存信息
	CString sSQL;
	// 如果是新增，则先插入顾客信息和会员卡
	if (0 == m_iOperationType)
	{
		if (!_SaveMember_Add())
			return FALSE;
	}
	// 然后更改
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("UPDATE 顾客信息 SET 地址='%s',邮政编码=%s,电子邮件='%s',\
				   联系电话='%s',生日='%s',手机='%s',快捷拼音='%s' WHERE 顾客ID=%d"),
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
	sSQL.Format(_T("UPDATE 会员卡 SET 顾客ID=%d,卡类型ID=%d,余额=%s,\
				   开卡日期='%s' WHERE 会员卡号='%s'"),
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
	sSQL.Format(_T("SELECT * FROM 顾客信息 WHERE 姓名='%s'"), strName);
	if(!pRecordset->Open(sSQL))
	{
		DELETE_POINTER(pRecordset);
		return FALSE;
	}
	if (!pRecordset->IsEOF())
	{
		pRecordset->GetCollect(_T("顾客ID"), iMemberID);
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
	CString sSQL = _T("SELECT MAX(顾客ID) AS MAXID FROM 顾客信息");
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
	// 首先判断姓名
	if (m_sName.IsEmpty())
	{
		AfxMessageBox(_T("错误：姓名不能为空！"), MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_EDIT_NAME)->SetFocus();
		return FALSE;
	}
	// 判断生日
	if (!IsDateFormat(m_sBirth))
	{
		AfxMessageBox(_T("错误：生日格式应该为'2008-08-08'！"), MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_EDIT_BIRTH)->SetFocus();
		return FALSE;
	}

	return TRUE;
}