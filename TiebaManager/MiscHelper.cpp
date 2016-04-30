#include "stdafx.h"
#include "MiscHelper.h"
#include <Dbghelp.h>

// 不阻塞消息的延迟
void Delay(DWORD time)
{
#pragma warning(suppress: 28159)
	DWORD startTime = GetTickCount();
#pragma warning(suppress: 28159)
	while (GetTickCount() - startTime < time)
	{
		DoEvents();
		Sleep(1); // 防止占用CPU
	}
}

// 处理消息
void DoEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
}

// 异常处理
LONG WINAPI ExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	CFile file;
	if (file.Open(_T("exception.dmp"), CFile::modeCreate | CFile::modeWrite))
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId = GetCurrentThreadId();
		einfo.ExceptionPointers = ExceptionInfo;
		einfo.ClientPointers = FALSE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpWithIndirectlyReferencedMemory,
			&einfo, NULL, NULL);
	}
	AfxMessageBox(_T("程序崩溃了，请把exception.dmp文件发到xfgryujk@126.com帮助调试"), MB_ICONERROR);
	return EXCEPTION_CONTINUE_SEARCH;
}

// 创建目录
BOOL CreateDir(const CString& path)
{
	if (PathFileExists(path))
		return TRUE;
	int pos = path.ReverseFind(_T('\\'));
	if (pos != -1)
	{
		CString parent = path.Left(pos);
		if (!CreateDir(parent))
			return FALSE;
	}
	return CreateDirectory(path, NULL);
}

// 初始化COM库
BOOL CoInitializeHelper()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		CString tmp;
		tmp.Format(_T("CoInitializeEx失败！\r\n错误代码0x%08X"), hr);
		AfxMessageBox(tmp, MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}
