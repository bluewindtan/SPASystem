// DlgSearchCard.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgSearchCard.h"


// CDlgSearchCard 对话框

IMPLEMENT_DYNAMIC(CDlgSearchCard, CDialog)

CDlgSearchCard::CDlgSearchCard(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchCard::IDD, pParent)
	, m_sInput(_T(""))
	, m_iInputType(0)
{

}

CDlgSearchCard::~CDlgSearchCard()
{
}

void CDlgSearchCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_sInput);
	DDV_MaxChars(pDX, m_sInput, 20);
}


BEGIN_MESSAGE_MAP(CDlgSearchCard, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSearchCard::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSearchCard 消息处理程序

void CDlgSearchCard::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;
	// 确定 查找类型和内容
	m_iInputType = _GetRadioType();
	OnOK();
}

BOOL CDlgSearchCard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_InitRadioType(m_iInputType);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSearchCard::_InitRadioType(int iType)
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1+iType);
}

int CDlgSearchCard::_GetRadioType(void)
{
	int iID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	int iType = iID - IDC_RADIO1;
	if (iType < 0)
	{
		ASSERT(0);
		iType = 0;
	}

	return iType;
}
