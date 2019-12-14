#include "stdafx.h"
#include "stdio.h"

#include "WndMsgLoop.h"
#include "HookOffset.h"
#include "MsgProtocol.h"
#include "LogRecord.h"

// ��ʼ����Ϣѭ������
void InitWindow(HMODULE hModule)
{
	LogRecord(L"InitWindow", ofs);

	RegisterWindow(hModule);
}

// ע�ᴰ�ڼ���Ϣѭ��
void RegisterWindow(HMODULE hModule)
{
	LogRecord(L"RegisterWindowMsgLoop", ofs);

	//1  ���һ��������
	WNDCLASS wnd;
	wnd.style = CS_VREDRAW | CS_HREDRAW;	//���
	wnd.lpfnWndProc = WndProc;	//������Ϣ�ص�����ָ��.
	wnd.cbClsExtra = NULL;
	wnd.cbWndExtra = NULL;
	wnd.hInstance = hModule;
	wnd.hIcon = NULL;
	wnd.hCursor = NULL;
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = TEXT("WeChatHelper");
	//2  ע�ᴰ����
	RegisterClass(&wnd);
	//3  ��������
	HWND hWnd = CreateWindow(
		TEXT("WeChatHelper"),	//��������
		TEXT("WeChatHelper"),	//������
		WS_OVERLAPPEDWINDOW,	//���ڷ��
		10, 10, 500, 300,	//����λ��
		NULL,	//�����ھ��
		NULL,	//�˵����
		hModule,	//ʵ�����
		NULL	//����WM_CREATE��Ϣʱ�ĸ��Ӳ���
	);
	//4  ������ʾ����
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	//5  ��Ϣѭ������Ϣ�ã�
	MSG  msg = {};
	//	5.1��ȡ��Ϣ
	while (GetMessage(&msg, 0, 0, 0))
	{
		//	5.2������Ϣ
		TranslateMessage(&msg);
		//	5.3ת������Ϣ�ص�����
		DispatchMessage(&msg);
	}
}

// ������Ϣ�ص�
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// ������WM_COPYDATA����Ϣ
	if (Message == WM_COPYDATA)
	{
		LogRecord(L"�յ�WM_COPYDATA����Ϣ", ofs);
		COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
		LogRecord(L"switch type", ofs);
		switch (pCopyData->dwData)
		{
		case WM_CheckIsLogin: {
			LogRecord(L"�յ�WM_CheckIsLoginָ��", ofs);
			CheckIsLogin();
			break;
		}
		case WM_HookReciveMsg: {
			LogRecord(L"�յ�WM_HookReciveMsgָ��", ofs);
			HOOK_ReciveMsg();
			break;
		}
		case WM_ShowQrCode: {
			LogRecord(L"�յ�WM_ShowQrCodeָ��", ofs);
			WX_CallShowQrCode();
			HOOK_SaveQrCode();
			break;
		}
		default:

			char ty[34] = { 0 };
			TCHAR * tchar = { 0 };
			int iLength;

			sprintf_s(ty, sizeof(ty), "%d", pCopyData->dwData);
			iLength = MultiByteToWideChar(CP_ACP, 0, ty, strlen(ty) + 1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, ty, strlen(ty) + 1, tchar, iLength);

			LogRecord(tchar, ofs);
			break;
		}
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}
