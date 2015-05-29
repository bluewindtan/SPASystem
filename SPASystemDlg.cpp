// SPASystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SPASystem.h"
#include "SPASystemDlg.h"
#include "./Ado/Ado.h"
#include "DlgCard.h"
#include "DlgSearchCard.h"
#include "DlgMember.h"
#include "DlgConsume.h"
#include "DlgConsume.h"
#include "DlgCareItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSPASystemDlg 对话框




CSPASystemDlg::CSPASystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSPASystemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSPASystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSPASystemDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_MEMBER, &CSPASystemDlg::OnBnClickedBtnMember)
	ON_BN_CLICKED(IDC_BTN_CARD, &CSPASystemDlg::OnBnClickedBtnCard)
	ON_BN_CLICKED(IDC_BTN_ITEM, &CSPASystemDlg::OnBnClickedBtnItem)
	ON_COMMAND(ID_CARD_ALL, &CSPASystemDlg::OnCardAll)
	ON_COMMAND(ID_CARD_CONSUME, &CSPASystemDlg::OnCardConsume)
	ON_COMMAND(ID_SYSTEM_CARDTYPE, &CSPASystemDlg::OnSystemCardtype)
	ON_COMMAND(ID_SYSTEM_ITEM, &CSPASystemDlg::OnSystemItem)
	ON_COMMAND(ID_CARD_INFO, &CSPASystemDlg::OnCardInfo)
	ON_COMMAND(ID_CARD_NEW, &CSPASystemDlg::OnCardNew)
END_MESSAGE_MAP()


// CSPASystemDlg 消息处理程序

BOOL CSPASystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->ShowWindow(SW_MAXIMIZE);

	// 隐藏按钮
	GetDlgItem(IDC_BTN_CARD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_MEMBER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ITEM)->ShowWindow(SW_HIDE);

	//CDlgConsume dlgConsume(DEFUALT_MEMBERID);
	//dlgConsume.DoModal();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSPASystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSPASystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rectText;
		rectText.top = 0;
		rectText.left = 0;
		rectText.right = 1024;
		rectText.bottom = 500;
		CFont fontText;
		fontText.CreateFont(
			100,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			TRUE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			GB2312_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体"));
		CFont *pOldFont = dc.SelectObject(&fontText);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(200, 55, 180));
		dc.DrawText(_T("雨涵女子美容SPA馆"),-1,rectText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		dc.SelectObject(pOldFont);

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CSPASystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CAdoConnection* CSPASystemDlg::_CreaterAdoConnection(void)
{
	CAdoConnection* pConn = new CAdoConnection;
	if (!pConn || !pConn->ConnectDefault())
	{
		DELETE_POINTER(pConn);
		AfxMessageBox(_T("错误：连接数据库失败!请与管理员连联!"), MB_OK | MB_ICONERROR);
	}
	return pConn;
}

BOOL CSPASystemDlg::CreateADO(void)
{
	// 创建ADO连接
	m_pConSPA = _CreaterAdoConnection();
	if (!m_pConSPA)
		return FALSE;

	return TRUE;
}

BOOL CSPASystemDlg::ReleaseADO(void)
{
	//if (m_pConSPA && m_pConSPA->IsOpen())
	//	m_pConSPA->Close();
	DELETE_POINTER(m_pConSPA);

	return TRUE;
}

void CSPASystemDlg::OnBnClickedBtnMember()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCardAll();
	OnCardConsume();
}

void CSPASystemDlg::OnBnClickedBtnCard()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnSystemCardtype();
	OnCardConsume();
}

void CSPASystemDlg::OnBnClickedBtnItem()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnSystemItem();
	OnCardConsume();
}

void CSPASystemDlg::OnCardAll()
{
	// TODO: 在此添加命令处理程序代码
	CDlgCard dlgCard;
	dlgCard.DoModal();
}

void CSPASystemDlg::OnCardConsume()
{
	// TODO: 在此添加命令处理程序代码
	CDlgSearchCard dlgSearchCard;
	if (IDOK == dlgSearchCard.DoModal())
	{
		int iMemberID = _GetMemberID(dlgSearchCard.m_iInputType, dlgSearchCard.m_sInput);
		// {Delete By TanFeng 2008-11-18
		// 改变需求，无需有效的顾客ID
		//if (iMemberID > 0)
		//{
		//}		 
		// ~}Delete By TanFeng
		CDlgConsume dlgConsume(iMemberID);
		dlgConsume.DoModal();
	}
}

void CSPASystemDlg::OnSystemCardtype()
{
	// TODO: 在此添加命令处理程序代码
	//MENUITEM "卡类型(&P)\tCtrl+P",             ID_SYSTEM_CARDTYPE
}

void CSPASystemDlg::OnSystemItem()
{
	// TODO: 在此添加命令处理程序代码
	CDlgCareItem dlgCareItem;
	dlgCareItem.DoModal();
}

void CSPASystemDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (IDOK == AfxMessageBox(_T("您确定退出本系统吗？"), MB_OKCANCEL))
	{
		CDialog::OnCancel();
		//AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

void CSPASystemDlg::OnCardInfo()
{
	// TODO: 在此添加命令处理程序代码
	CDlgSearchCard dlgSearchCard;
	if (IDOK == dlgSearchCard.DoModal())
	{
		int iMemberID = _GetMemberID(dlgSearchCard.m_iInputType, dlgSearchCard.m_sInput);
		_CallCard(1, iMemberID);
	}
}

void CSPASystemDlg::OnCardNew()
{
	// TODO: 在此添加命令处理程序代码
	_CallCard(0);
}

void CSPASystemDlg::_CallCard(int iType, int iMemberID /* = 0 */)
{
	CDlgMember dlg(iType, iMemberID);
	dlg.m_pConSPA = m_pConSPA;
	dlg.DoModal();
}

int CSPASystemDlg::_GetMemberID(int iType, const CString& strInput)
{
	CString sSQL;
	CString sMsg;
	if (0 == iType) // 会员卡号
	{
		sSQL.Format(_T("SELECT 顾客ID FROM 会员卡 WHERE 会员卡号='%s'"), strInput);
		sMsg.Format(_T("会员卡号%s不存在，将会以默认普通用户代替！"), strInput);
	}
	else if (1 == iType) // 会员姓名
	{
		sSQL.Format(_T("SELECT 顾客ID FROM 顾客信息 WHERE 姓名='%s'"), strInput);
		sMsg.Format(_T("会员%s不存在，将会以默认普通用户代替！"), strInput);
	}
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return -1;
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return -1;
	}
	int iMemberID = 0;
	if (!pSetRead->IsEOF())
	{
		pSetRead->GetCollect(_T("顾客ID"), iMemberID);
	}
	else
	{
		if (!strInput.IsEmpty())
		{
			AfxMessageBox(sMsg);
		}
	}
	pSetRead->Close();

	// 删除
	DELETE_POINTER(pSetRead);
	return iMemberID;
}

void CSPASystemDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();

	OnCardConsume();
}
