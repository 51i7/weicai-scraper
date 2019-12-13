#include "stdafx.h"
#include "stdio.h"

#include "WndMsgLoop.h"
#include "HookOffset.h"
#include "MsgProtocol.h"


// ��ʼ����Ϣѭ������
void InitWindow(HMODULE hModule)
{
	RegisterWindow(hModule);
}

// ע�ᴰ�ڼ���Ϣѭ��
void RegisterWindow(HMODULE hModule)
{
	OutputDebugStringA("RegisterWindowMsgLoop\n");
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
		OutputDebugStringA("�յ�WM_COPYDATA����Ϣ\n");
		COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
		switch (pCopyData->dwData)
		{
		case WM_CheckIsLogin: {
			//MessageBoxA(hWnd, "�յ�WM_CheckIsLoginָ��", NULL, 0);
			OutputDebugStringA("�յ�WM_CheckIsLoginָ��\n");
			CheckIsLogin();
			break;
		}
		case WM_HookReciveMsg: {
			//MessageBoxA(hWnd, "�յ�WM_HookReciveMsgָ��", NULL, 0);
			OutputDebugStringA("�յ�WM_HookReciveMsgָ��\n");
			HOOK_ReciveMsg();
			break;
		}
		case WM_ShowQrCode: {
			//MessageBoxA(hWnd, "�յ�WM_ShowQrCodeָ��", NULL, 0);
			OutputDebugStringA("�յ�WM_ShowQrCodeָ��\n");
			WX_CallShowQrCode();
			HOOK_SaveQrCode();
			break;
		}
		default:
			break;
		}
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}
