// SPASystemDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSPASystemDlg �Ի���




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


// CSPASystemDlg ��Ϣ�������

BOOL CSPASystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->ShowWindow(SW_MAXIMIZE);

	// ���ذ�ť
	GetDlgItem(IDC_BTN_CARD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_MEMBER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ITEM)->ShowWindow(SW_HIDE);

	//CDlgConsume dlgConsume(DEFUALT_MEMBERID);
	//dlgConsume.DoModal();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSPASystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
			_T("����"));
		CFont *pOldFont = dc.SelectObject(&fontText);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(200, 55, 180));
		dc.DrawText(_T("�꺭Ů������SPA��"),-1,rectText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		dc.SelectObject(pOldFont);

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		AfxMessageBox(_T("�����������ݿ�ʧ��!�������Ա����!"), MB_OK | MB_ICONERROR);
	}
	return pConn;
}

BOOL CSPASystemDlg::CreateADO(void)
{
	// ����ADO����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnCardAll();
	OnCardConsume();
}

void CSPASystemDlg::OnBnClickedBtnCard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnSystemCardtype();
	OnCardConsume();
}

void CSPASystemDlg::OnBnClickedBtnItem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnSystemItem();
	OnCardConsume();
}

void CSPASystemDlg::OnCardAll()
{
	// TODO: �ڴ���������������
	CDlgCard dlgCard;
	dlgCard.DoModal();
}

void CSPASystemDlg::OnCardConsume()
{
	// TODO: �ڴ���������������
	CDlgSearchCard dlgSearchCard;
	if (IDOK == dlgSearchCard.DoModal())
	{
		int iMemberID = _GetMemberID(dlgSearchCard.m_iInputType, dlgSearchCard.m_sInput);
		// {Delete By TanFeng 2008-11-18
		// �ı�����������Ч�Ĺ˿�ID
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
	// TODO: �ڴ���������������
	//MENUITEM "������(&P)\tCtrl+P",             ID_SYSTEM_CARDTYPE
}

void CSPASystemDlg::OnSystemItem()
{
	// TODO: �ڴ���������������
	CDlgCareItem dlgCareItem;
	dlgCareItem.DoModal();
}

void CSPASystemDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (IDOK == AfxMessageBox(_T("��ȷ���˳���ϵͳ��"), MB_OKCANCEL))
	{
		CDialog::OnCancel();
		//AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

void CSPASystemDlg::OnCardInfo()
{
	// TODO: �ڴ���������������
	CDlgSearchCard dlgSearchCard;
	if (IDOK == dlgSearchCard.DoModal())
	{
		int iMemberID = _GetMemberID(dlgSearchCard.m_iInputType, dlgSearchCard.m_sInput);
		_CallCard(1, iMemberID);
	}
}

void CSPASystemDlg::OnCardNew()
{
	// TODO: �ڴ���������������
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
	if (0 == iType) // ��Ա����
	{
		sSQL.Format(_T("SELECT �˿�ID FROM ��Ա�� WHERE ��Ա����='%s'"), strInput);
		sMsg.Format(_T("��Ա����%s�����ڣ�������Ĭ����ͨ�û����棡"), strInput);
	}
	else if (1 == iType) // ��Ա����
	{
		sSQL.Format(_T("SELECT �˿�ID FROM �˿���Ϣ WHERE ����='%s'"), strInput);
		sMsg.Format(_T("��Ա%s�����ڣ�������Ĭ����ͨ�û����棡"), strInput);
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
		pSetRead->GetCollect(_T("�˿�ID"), iMemberID);
	}
	else
	{
		if (!strInput.IsEmpty())
		{
			AfxMessageBox(sMsg);
		}
	}
	pSetRead->Close();

	// ɾ��
	DELETE_POINTER(pSetRead);
	return iMemberID;
}

void CSPASystemDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();

	OnCardConsume();
}
