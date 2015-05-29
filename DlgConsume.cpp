// DlgConsume.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SPASystem.h"
#include "DlgConsume.h"
#include "SPASystemDlg.h"
#include "./Ado/Ado.h"
#include "CustomGridCtrl.h"
#include "DlgFindItem.h"

// CDlgConsume �Ի���

IMPLEMENT_DYNAMIC(CDlgConsume, CDialog)

CDlgConsume::CDlgConsume(int iMemberID /*= 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConsume::IDD, pParent)
	, m_pGridCtrl(NULL)
	, m_sConsumeDate(_T(""))
	, m_iMemberID(iMemberID)
	, m_sName(_T(""))
	, m_sCardNo(_T(""))
	, m_sBalance(_T("0.00"))
	, m_sCardType(_T(""))
	, m_sNowMoney(_T("0.00"))
	, m_iCardID(0)
{
	m_pConSPA = NULL;
}

CDlgConsume::~CDlgConsume()
{
	DELETE_POINTER(m_pGridCtrl);
}

void CDlgConsume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONSUMEDATE, m_sConsumeDate);
	DDV_MaxChars(pDX, m_sConsumeDate, 10);
}


BEGIN_MESSAGE_MAP(CDlgConsume, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgConsume::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgConsume::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgConsume ��Ϣ�������

BOOL CDlgConsume::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	_InitGrid();
	_InitGridFromDB();
	_CalcMoney();
	_SetDefaultText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgConsume::_SetDefaultText(void)
{
	// ������
	if (m_sCardType.IsEmpty())
		m_sCardType = g_sDefaultCardType;
	_SetGridText(g_iGridRow_CardType-1, g_iGridCol_CardType-1, m_sCardType, FALSE);
}

void CDlgConsume::_InitGrid(void)
{
	DELETE_POINTER(m_pGridCtrl);
	m_pGridCtrl = new CCustomGridCtrl(g_iGridRows, g_iGridCols, 0, 0);
	if (NULL == m_pGridCtrl)
		return;
	// �ֹ�����λ��
	int iHeight = g_iRowHeight_Row0 + g_iRowHeight * (g_iGridRows - 1);
	int iWidth = (int)(g_fColWidth * (float)g_iGridCols);
	int iHeightOff = 80;
	// ���ڵĴ�С
	this->SetWindowPos(NULL, 0, 0, iWidth, iHeight + iHeightOff, SWP_NOMOVE);
	// ��ť��λ��
	CButton* pButton = (CButton*)GetDlgItem(IDOK);
	if (pButton)
	{
		CRect rect;
		GetClientRect(rect);
		CRect rectButton;
		pButton->GetClientRect(rectButton);
		pButton->SetWindowPos(NULL, (rect.right-rectButton.right)/2, 
			rect.bottom - rectButton.bottom - 10, 0, 0, SWP_NOSIZE);
	}
	// ��������
	CRect rectGrid(0, 0, iWidth, iHeight);
	m_pGridCtrl->Create(rectGrid, this, 1000);
	m_pGridCtrl->SetEditable(FALSE);
	m_pGridCtrl->EnableDragAndDrop(FALSE);
	// ���������ı�������ʾ
	for (int ix=0; ix<m_pGridCtrl->GetRowCount(); ix++)
	{
		for (int iy=0; iy<m_pGridCtrl->GetColumnCount(); iy++)
		{
			_SetGridCenter(ix, iy);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// ����̶�����
	//////////////////////////////////////////////////////////////////////////
	int iBeginRow = 0;
	_InitGrid_Title(iBeginRow);	// ��1��-��2��
	_InitGrid_Head(iBeginRow);	// ��3��-��4��
	_InitGrid_Body(iBeginRow);	// ��5��-��10��
	_InitGrid_Tail(iBeginRow);	// ��11��-��12��
	ASSERT(iBeginRow == g_iGridRows);
}

void CDlgConsume::_InitGrid_Title(int& iBeginRow)
{
	// ===== ��2�� =====
	_SetGridText(iBeginRow, 0, _T("�꺭Ů������SPA��"));
	CFont fontText;
	fontText.CreateFont(
		26,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����"));
	LOGFONT lf;
	fontText.GetLogFont(&lf);
	m_pGridCtrl->SetItemFont(iBeginRow, 0, &lf);
	m_pGridCtrl->SetCellCombine(iBeginRow, 0, 1, 7);
	iBeginRow += 2;
}

void CDlgConsume::_InitGrid_Head(int& iBeginRow)
{
	// ===== ��2�� =====
	// ===== ��1�� =====
	// ��Ա����
	_SetGridText(iBeginRow, 0, _T("��Ա����"), FALSE);
	// ��Ա����
	_SetGridText(iBeginRow, 2, _T("��Ա����"), FALSE);
	// ��Ա����
	_SetGridText(iBeginRow, 4, _T("��Ա����"), FALSE);
	// ���ڽ��
	_SetGridText(iBeginRow, 6, _T("���ڽ��"), FALSE);
	iBeginRow++;
	// ===== ��2�� =====
	// ������Ŀ
	_SetGridText(iBeginRow, 0, _T("��     ��     ��     Ŀ"), FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 0, 0, 4);
	// ���Ѽ۸�
	_SetGridText(iBeginRow, 5, _T("���Ѽ۸�"), FALSE);
	// ��Ա�۸�
	_SetGridText(iBeginRow, 6, _T("��Ա�۸�"), FALSE);
	// ����ʦǩ��
	_SetGridText(iBeginRow, 7, _T("����ʦ"), FALSE);
	iBeginRow++;
}

void CDlgConsume::_InitGrid_Body(int& iBeginRow)
{
	// ===== ��10�� =====
	// ������Ŀ����
	int iRows = 0;
	CString sText;
	for ( ; iRows < g_iGridRows_Item; iRows++)
	{
		sText.Format(_T("%d"), iRows+1);
		m_pGridCtrl->SetCellCombine(iBeginRow, 0, 0, 4);
		iBeginRow++;
	}
}

void CDlgConsume::_InitGrid_Tail(int& iBeginRow)
{
	// ===== ��2�� =====
	// ===== ��1�� =====
	// �ܼ�
	_SetGridText(iBeginRow, 0, _T("�ܼ�����"), FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 0, 0, 5);
	iBeginRow++;
	// ===== ��2�� =====
	// ��������
	_SetGridText(iBeginRow, 0, _T("��������"), FALSE);
	CTime time = CTime::GetCurrentTime();
	m_sConsumeDate.Format(_T("%4d��%2d��%2d��"), time.GetYear(), time.GetMonth(), time.GetDay());
	_SetGridText(iBeginRow, 1, m_sConsumeDate, FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 1, 0, 1);
	m_sConsumeDate.Format(_T("%4d-%2d-%2d"), time.GetYear(), time.GetMonth(), time.GetDay());
	UpdateData(FALSE);
	// �������
	_SetGridText(iBeginRow, 3, _T("�������"), FALSE);
	m_pGridCtrl->SetCellCombine(iBeginRow, 4, 0, 1);
	// ��Աǩ��
	_SetGridText(iBeginRow, 6, _T("��Աǩ��"), FALSE);
	iBeginRow++;
}

void CDlgConsume::_SetGridText(int iRow, int iCol, LPCTSTR sText, BOOL bModified /* = TRUE */)
{
	m_pGridCtrl->SetItemText(iRow, iCol, sText);
	m_pGridCtrl->SetModified(bModified, iRow, iCol);
}

void CDlgConsume::_SetGridCenter(int iRow, int iCol)
{
	m_pGridCtrl->SetItemFormat(iRow, iCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CDlgConsume::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iResult = AfxMessageBox(_T("�����ѵ��Ƿ���ˣ�"), MB_YESNOCANCEL);
	if (IDYES == iResult)
	{
		if (!_SaveConsume())
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
	OnCancel();
}

void CDlgConsume::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!_SaveConsume())
	{
		//AfxMessageBox(_T("����ʧ�ܣ�"));
		return;
	}
	OnOK();
}

void CDlgConsume::_PrintGrid(void)
{
	m_pGridCtrl->Print();
}

BOOL CDlgConsume::_SaveConsume(void)
{
	if (!UpdateData())
		return FALSE;
	// �ж��Ƿ����ѹ�
	if (0 == m_sNowMoney.CompareNoCase(m_sBalance))
	{
		if (IDOK == AfxMessageBox(_T("����û�����ѣ��Ƿ��ӡ��"), MB_OKCANCEL))
		{
			_PrintGrid();
		}
		return TRUE;
	}
	// ���浽���ݿ�
	int iMaxID = 0;
	if (!_GetNextConsumeID(iMaxID))
	{
		ASSERT(0);
		SeriousError();
		return FALSE;
	}
	CString sSQL;
	m_pConSPA->BeginTrans();
	sSQL.Format(_T("INSERT INTO ���ѵ�(ID, ��Ա��ID,��������,�ܼƽ��) VALUES (%d,%d,'%s',%s-%s)"),
		iMaxID, m_iCardID, m_sConsumeDate, m_sBalance, m_sNowMoney);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	sSQL.Format(_T("UPDATE ��Ա�� SET ���=%s WHERE ID=%d"), m_sNowMoney, m_iCardID);
	if (!m_pConSPA->Execute(sSQL))
	{
		m_pConSPA->RollbackTrans();
		return FALSE;
	}
	m_pConSPA->CommitTrans();
	// ������������ϸ
	m_pConSPA->BeginTrans();
	if (!_SaveConsume_Detail(iMaxID))
	{
		ASSERT(0); // Ŀǰ��ϸ������Ҫ�ģ��������۱����Ƿ�ɹ�������ӡ��
		m_pConSPA->RollbackTrans();
	}
	m_pConSPA->CommitTrans();
	// ��ӡ
	_PrintGrid();
	return TRUE;
}

BOOL CDlgConsume::_GetNextConsumeID(int& iMaxID)
{
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return FALSE;
	CString sSQL = _T("SELECT MAX(ID)+1 AS MaxID FROM ���ѵ�");
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return FALSE;
	}
	pSetRead->GetCollect(_T("MaxID"), iMaxID);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	return TRUE;
}


BOOL CDlgConsume::_GetItemIDByName(int& iItemID, const CString& sItemName)
{
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return FALSE;
	CString sSQL;
	sSQL.Format(_T("SELECT ��ĿID FROM ������Ŀ WHERE ��Ŀ����='%s'"), sItemName);
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return FALSE;
	}
	pSetRead->GetCollect(_T("��ĿID"), iItemID);
	pSetRead->Close();
	DELETE_POINTER(pSetRead);
	return TRUE;
}

BOOL CDlgConsume::_SaveConsume_Detail(int iID)
{
	// ����ȷ��������Ŀ��ʼ�к�����
	double dTemp = 0.00;
	LPTSTR strTemp;
	CString strMoney;
	CString sItemName;
	CString sSQL;
	int iItemID = 0;
	int iRowItem = g_iBeginRow_Item - 1;
	for (int ix=0; ix<g_iGridRows_Item; ix++)
	{
		sItemName = m_pGridCtrl->GetItemText(iRowItem+ix, 0);
		strMoney = m_pGridCtrl->GetItemText(iRowItem+ix, g_iGridCol_Money-1);
		dTemp = _tcstod(strMoney, &strTemp);
		if (0.00 >= dTemp)
			continue;
		// ������ϸ
		if (!_GetItemIDByName(iItemID, sItemName))
		{	
			return FALSE;
		}
		sSQL.Format(_T("INSERT INTO ������ϸ��¼(���ѵ�ID,������ĿID,���ѽ��) VALUES (%d,%d,%s)"),
			iID, iItemID, strMoney);
		if (!m_pConSPA->Execute(sSQL))
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CDlgConsume::_InitGridFromDB(void)
{
	CSPASystemDlg* pMainDlg = dynamic_cast<CSPASystemDlg*>(AfxGetMainWnd());
	if(!pMainDlg || NULL == pMainDlg->m_pConSPA)
		return;
	m_pConSPA = pMainDlg->m_pConSPA;
	CAdoRecordSet* pSetRead = new CAdoRecordSet(m_pConSPA);
	if (!pSetRead)
		return;
	CString sSQL;
	sSQL.Format(_T("SELECT * FROM QueryCard WHERE �˿�ID=%d"), m_iMemberID);
	if(!pSetRead->Open(sSQL))
	{
		DELETE_POINTER(pSetRead);
		return;
	}
	int iMemberID = 0;
	if (!pSetRead->IsEOF())
	{
		pSetRead->GetCollect(_T("��Ա��ID"), m_iCardID);
		pSetRead->GetCollect(_T("��Ա����"), m_sCardNo);
		_SetGridText(2, 1, m_sCardNo, FALSE);
		pSetRead->GetCollect(_T("����"), m_sName);
		_SetGridText(2, 3, m_sName, FALSE);
		pSetRead->GetCollect(_T("������"), m_sCardType);
		double dBalance;
		pSetRead->GetCollect(_T("���"), dBalance);
		m_sBalance.Format(_T("%.2f"), dBalance);
		_SetGridText(2, 7, m_sBalance, FALSE);
	}
	pSetRead->Close();

	// ɾ��
	DELETE_POINTER(pSetRead);
}

void CDlgConsume::_CalcMoney(void)
{
	// ����ǰ���ڽ��
	LPTSTR strTemp;
	double dMoney = _tcstod(m_sBalance, &strTemp);
	m_sNowMoney = m_sBalance;
	// �����ܼ����ѽ��
	double dConsume = 0.00;
	double dTemp = 0.00;
	// ����ȷ��������Ŀ��ʼ�к�����
	CString strMoney;
	int iRowItem = g_iBeginRow_Item - 1;
	for (int ix=0; ix<g_iGridRows_Item; ix++)
	{
		strMoney = m_pGridCtrl->GetItemText(iRowItem+ix, g_iGridCol_Money-1);
		dTemp = _tcstod(strMoney, &strTemp);
		dConsume += dTemp;
	}
	strMoney.Format(_T("%.2f"), dConsume);
	_SetGridText(g_iGridRows-2, g_iGridCol_Money-1, strMoney, FALSE);
	m_pGridCtrl->RedrawCell(g_iGridRows-2, g_iGridCol_Money-1);
	// �������
	strMoney.Format(_T("%.2f"), dMoney - dConsume);
	_SetGridText(g_iGridRows-1, 4, strMoney, FALSE);
	m_sNowMoney = strMoney;
	m_pGridCtrl->RedrawCell(g_iGridRows-1, 4);
	if (dMoney - dConsume < 0)
	{
		AfxMessageBox(_T("�������㣬��Ҫ��ֵ��"));
	}
}

void CDlgConsume::_ProcMsgGrid(int iRow, int iCol)
{
	// Ѱ����Ŀ
	CDlgFindItem dlgFind;
	dlgFind.m_strCardType = m_pGridCtrl->GetItemText(g_iGridRow_CardType-1, g_iGridCol_CardType-1);
	if (IDOK == dlgFind.DoModal())
	{
		FindItem* pItem = dlgFind.m_pSelItem;
		if (pItem)
		{
			_SetGridText(iRow, 0, pItem->sItemName);
			_SetGridText(iRow, g_iGridCol_Cost-1, pItem->sCost);
			_SetGridText(iRow, g_iGridCol_Money-1, pItem->sMoney);
			m_pGridCtrl->RedrawRow(iRow);
			_CalcMoney();
		}
	}
}

void CDlgConsume::_DeleteGrid(int iRow)
{
	_SetGridText(iRow, 0, _T(""));
	_SetGridText(iRow, g_iGridCol_Cost-1, _T(""));
	_SetGridText(iRow, g_iGridCol_Money-1, _T(""));
	m_pGridCtrl->RedrawRow(iRow);
	_CalcMoney();
}

BOOL CDlgConsume::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (wParam == m_pGridCtrl->GetDlgCtrlID())
	{
		NM_GRIDVIEW* pNmgv = (NM_GRIDVIEW*)lParam;
		if (pNmgv)
		{		
			//if (GVN_SELCHANGED == pNmgv->hdr.code)
			if (GRIDMSG_DBCLICK == pNmgv->hdr.code)
			{
				_ProcMsgGrid(pNmgv->iRow, pNmgv->iColumn);
			}
			else if (GVN_ENDLABELEDIT == pNmgv->hdr.code)
			{
				_DeleteGrid(pNmgv->iRow);
			}
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}
