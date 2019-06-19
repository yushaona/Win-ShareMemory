// MainProc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
using namespace std;

#define FILENAME L"Fussen Mapping File Name"

#define MAP_SIZE 4096  //文件映射对象的大小
#define VIEW_SIZE 1024 //文件映射对象，映射到进程中的大小 ,所以VIEW_SIZE要小于等于MAP_SIZE
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMapFile = NULL;
	PVOID pView = NULL;
	
	//创建文件映射对象,相当于将INVALID_HANDLE_VALUE转换为hMapFile对象数值
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

	//将hMapFile文件映射对象，再映射为进程内存空间的视图对象
	pView = MapViewOfFile(hMapFile,// Handle of the map object
		FILE_MAP_ALL_ACCESS,// Read and write access
		0,		  // High-order DWORD of the file offset   表示从哪个位置开始，映射VIEW_SIZE大小的数据到进程空间中，剩下的空间，可以继续映射
		0,		  // Low-order DWORD of the file offset
		VIEW_SIZE // The number of bytes to map to view
		);

	if (pView  == NULL)
	{
		printf("MapViewOfFile failed");
		goto CleanUp;
	}

	//将内容写入到pView对象中
	TCHAR *Message = _T("中国单独的11yyynash");
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

