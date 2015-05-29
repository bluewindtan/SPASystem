// DlgItemInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgItemInfo.h"
#include "./Ado/Ado.h"

// CDlgItemInfo 对话框

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


// CDlgItemInfo 消息处理程序

void CDlgItemInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!_SaveItem())
	{
		//AfxMessageBox(_T("保存失败！"));
		return;
	}
	OnOK();
}

void CDlgItemInfo::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (_TextIsChanged())
	{
		int iResult = AfxMessageBox(_T("信息已经更改，退出前是否保存？"), MB_YESNOCANCEL);
		if (IDYES == iResult)
		{
			if (!_SaveItem())
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

BOOL CDlgItemInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_InitDlg();
	// 如果是修改，则姓名无法更改
	if (1 == m_iOperationType)
	{
		GetDlgItem(IDC_EDIT_ITEMNAME)->EnableWindow(FALSE);
	}
	_CreateCheckString(m_strCheck);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgItemInfo::_CreateCheckString(CString& strCheck)
{
	// 项目名称
	strCheck.Append(m_sItemName);
	// 快捷拼音
	strCheck.Append(m_sPY);
	// 普通卡
	strCheck.Append(m_sCardPT);
	// 银卡
	strCheck.Append(m_sCardY);
	// 金卡
	strCheck.Append(m_sCardJ);
	// 白金卡
	strCheck.Append(m_sCardBJ);
}

void CDlgItemInfo::_InitDlg(void)
{
	// 操作类型=0，表示新增
	if (0 == m_iOperationType)
		return;
	// 加载数据库
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL;
	sSQL.Format(_T("SELECT * FROM 护理项目 WHERE 项目ID=%d"), m_iItemID);
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
	pSetRead->GetCollect(_T("项目名称"), m_sItemName);
	pSetRead->GetCollect(_T("快捷拼音"), m_sPY);
	double dBalance;
	pSetRead->GetCollect(_T("普通卡"), dBalance);
	m_sCardPT.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("银卡"), dBalance);
	m_sCardY.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("金卡"), dBalance);
	m_sCardJ.Format(_T("%.2f"), dBalance);
	pSetRead->GetCollect(_T("白金卡"), dBalance);
	m_sCardBJ.Format(_T("%.2f"), dBalance);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	UpdateData(FALSE);
}

BOOL CDlgItemInfo::_TextIsValid(void)
{
	if (!UpdateData())
		return FALSE;
	// 首先判断名称
	if (m_sItemName.IsEmpty())
	{
		AfxMessageBox(_T("错误：项目名称不能为空！"), MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_EDIT_ITEMNAME)->SetFocus();
		return FALSE;
	}
	// 判断普通卡
	if (!IsNum_Dot(m_sCardPT) || !IsNum_Dot(m_sCardY) || 
		!IsNum_Dot(m_sCardJ) || !IsNum_Dot(m_sCardBJ))
	{
		AfxMessageBox(_T("错误：金额格式应该为'100.01'！"), MB_OK | MB_ICONSTOP);
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
	// 数据无效，则无需进行数据库保存，直接返回保存失败
	if (!_TextIsValid())
		return FALSE;
	// 没有修改过，或者保存过，则无需进行数据库保存，直接返回保存成功
	if (!_TextIsChanged())
		return TRUE;
	_CreateCheckString(m_strCheck);
	// 如果是新增，则先插入顾客信息和会员卡
	if (0 == m_iOperationType)
	{
		if (!_SaveItem_Add())
			return FALSE;
	}
	// 然后更改
	m_pConSPA->BeginTrans();
	CString sSQL;
	sSQL.Format(_T("UPDATE 护理项目 SET 项目名称='%s',快捷拼音='%s',普通卡=%s,\
				   银卡=%s,金卡=%s,白金卡=%s WHERE 项目ID=%d"),
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
	// 首先确定项目ID
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return FALSE;
	CString sSQL = _T("SELECT MAX(项目ID)+1 AS MaxID FROM 护理项目");
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
	// 开始新增
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("INSERT INTO 护理项目(项目ID) VALUES (%d)"), m_iItemID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	return TRUE;
}
