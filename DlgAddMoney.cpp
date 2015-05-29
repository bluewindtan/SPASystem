// DlgAddMoney.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgAddMoney.h"


// CDlgAddMoney �Ի���

IMPLEMENT_DYNAMIC(CDlgAddMoney, CDialog)

CDlgAddMoney::CDlgAddMoney(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddMoney::IDD, pParent)
	, m_sMoney(_T(""))
{

}

CDlgAddMoney::~CDlgAddMoney()
{
}

void CDlgAddMoney::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MONEY, m_sMoney);
	DDV_MaxChars(pDX, m_sMoney, MONEY_MAX_DIGIT-3);
}

BEGIN_MESSAGE_MAP(CDlgAddMoney, CDialog)
END_MESSAGE_MAP()


// CDlgAddMoney ��Ϣ�������
