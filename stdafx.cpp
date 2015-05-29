// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// SPASystem.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

BOOL IsDateFormat(CString& strDate)
{
	// ���Ϊ�գ�����Ϊ�ϸ�
	if (strDate.IsEmpty())
		return TRUE;
	if (strDate.GetLength() != 10)
		return FALSE;
	if ('-' != strDate.GetAt(4) || '-' != strDate.GetAt(7))
		return FALSE;

	COleDateTime oleTime;
	return oleTime.ParseDateTime(strDate);
}

BOOL IsNum_Dot(CString& strText, BOOL bHaveDot)
{
	if (strText.IsEmpty())
		return FALSE;
	int iLength = strText.GetLength();
	if (iLength > MONEY_MAX_DIGIT)
		return FALSE;
	if (strText.GetAt(0) == '.' || strText.GetAt(iLength-1) == '.')
		return FALSE;
	TCHAR szTmp[MONEY_MAX_DIGIT+1];
	memset(szTmp, 0, MONEY_MAX_DIGIT+1);
	_stscanf_s(strText.GetBuffer(), _T("%[.,0-9]"), szTmp, MONEY_MAX_DIGIT+1);
	strText.ReleaseBuffer();
	if(_tcsncmp(strText, szTmp, strText.GetLength()))
		return FALSE;
	TCHAR* sFind = _tcsstr(strText.GetBuffer(), _T("."));
	strText.ReleaseBuffer();
	// û��С����������
	if (!sFind)
		return !bHaveDot;
	sFind = _tcsstr(sFind+1, _T("."));
	if (sFind)
		return FALSE;
	return TRUE;
}

void SeriousError(void)
{
	AfxMessageBox(_T("ϵͳ�������ش�������ϵ��������ߣ�"), MB_OK | MB_ICONSTOP);
}