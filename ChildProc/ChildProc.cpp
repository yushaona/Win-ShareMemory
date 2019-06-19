// ChildProc.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <locale.h>   
#include <windows.h>
#define FILENAME L"Fussen Mapping File Name"
#define MAP_SIZE 4096  //�ļ�ӳ�����Ĵ�С
#define VIEW_SIZE 1024 //�ļ�ӳ�����ӳ�䵽�����еĴ�С ,����VIEW_SIZEҪС�ڵ���MAP_SIZE

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale(LC_ALL, _T("chs"));
	HANDLE hMapFile = NULL;
	PVOID pView = NULL;
	hMapFile = OpenFileMapping(
		FILE_MAP_READ,
		false,
		FILENAME);
	if (hMapFile == NULL)
	{
		goto CleanUp;
	}

	pView = MapViewOfFile(hMapFile,
		FILE_MAP_READ,
		0,
		0,
		VIEW_SIZE
		);

	if (pView == NULL)
	{
		goto CleanUp;
	}

	TCHAR *pt = (TCHAR*)pView;
	int i = 0;
	while (true)
	{
		TCHAR a = pt[i];
		if (a == '\0')
		{
			break;
		}
		i++;
		_tprintf(_T("%c"), a);
	}
	getchar();
CleanUp:

	if (hMapFile)
	{
		if (pView)
		{
			// Unmap the file view.
			UnmapViewOfFile(pView);
			pView = NULL;
		}
		// Close the file mapping object.
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	return 0;
}

