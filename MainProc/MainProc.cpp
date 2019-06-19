// MainProc.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
using namespace std;

#define FILENAME L"Fussen Mapping File Name"

#define MAP_SIZE 4096  //�ļ�ӳ�����Ĵ�С
#define VIEW_SIZE 1024 //�ļ�ӳ�����ӳ�䵽�����еĴ�С ,����VIEW_SIZEҪС�ڵ���MAP_SIZE
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMapFile = NULL;
	PVOID pView = NULL;
	
	//�����ļ�ӳ�����,�൱�ڽ�INVALID_HANDLE_VALUEת��ΪhMapFile������ֵ
	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,// Use paging file - shared memory
		NULL,			// Default security attributes
		PAGE_READWRITE,// Allow read and write access
		0,		 // High-order DWORD of file mapping max size
		MAP_SIZE,    // Low-order DWORD of file mapping max size
		FILENAME // Name of the file mapping object
		);
	if (hMapFile == NULL)
	{
		_tprintf("CreateFileMapping failed");
		goto CleanUp;
	}

	//��hMapFile�ļ�ӳ�������ӳ��Ϊ�����ڴ�ռ����ͼ����
	pView = MapViewOfFile(hMapFile,// Handle of the map object
		FILE_MAP_ALL_ACCESS,// Read and write access
		0,		  // High-order DWORD of the file offset   ��ʾ���ĸ�λ�ÿ�ʼ��ӳ��VIEW_SIZE��С�����ݵ����̿ռ��У�ʣ�µĿռ䣬���Լ���ӳ��
		0,		  // Low-order DWORD of the file offset
		VIEW_SIZE // The number of bytes to map to view
		);

	if (pView  == NULL)
	{
		printf("MapViewOfFile failed");
		goto CleanUp;
	}

	//������д�뵽pView������
	TCHAR *Message = _T("�й�������11yyynash");
	DWORD MessageSize = (_tcslen(Message) + 1) *sizeof(TCHAR);

	memcpy_s(pView, VIEW_SIZE, Message, MessageSize);

	getchar();
CleanUp:
	if (hMapFile)
	{
		if (pView)
		{
			UnmapViewOfFile(pView);
			pView = NULL;
		}
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	return 0;
}

