// stdafx.cpp : 只包括标准包含文件的源文件
// SPASystem.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

BOOL IsDateFormat(CString& strDate)
{
	// 如果为空，则认为合格
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
	// 没有小数点的情况下
	if (!sFind)
		return !bHaveDot;
	sFind = _tcsstr(sFind+1, _T("."));
	if (sFind)
		return FALSE;
	return TRUE;
}

void SeriousError(void)
{
	AfxMessageBox(_T("系统发生严重错误！请联系软件开发者！"), MB_OK | MB_ICONSTOP);
}