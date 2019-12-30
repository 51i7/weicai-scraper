#include "stdafx.h"
#include "stdio.h"
#include "WndMsgLoop.h"
#include "HookOffset.h"
#include "MsgProtocol.h"
#include "StringTool.h"
#include "LogRecord.h"
#include "WCProcess.h"
#include "HttpRequest.h"
#include "json.hpp"


#pragma comment(lib, "rpcrt4.lib")
#include <rpc.h>


using json = nlohmann::json;

using namespace std;

DWORD isRegisterWnd;
LPCWSTR WeChatHelper;

// ��ʼ����Ϣѭ������
void InitWindow(HMODULE hModule)
{
	LogRecord(L"InitWindow", ofs);

	UUID uuid;
	UuidCreate(&uuid);
	char *str;
	UuidToStringA(&uuid, (RPC_CSTR*)&str);

	char ty[1024] = { 0 };
	sprintf_s(ty, sizeof(ty), "%s%s", "WeChatHelper", "");//str);
	TCHAR tstr[1024] = TEXT("");
	CharToTchar(ty, tstr);
	WeChatHelper = (LPCWSTR)tstr;
	LogRecord(WeChatHelper, ofs);

	RpcStringFreeA((RPC_CSTR*)&str);

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
	wnd.lpszClassName = WeChatHelper;
	//2  ע�ᴰ����
	RegisterClass(&wnd);
	//3  ��������
	HWND hWnd = CreateWindow(
		WeChatHelper,	//��������
		WeChatHelper,	//������
		WS_OVERLAPPEDWINDOW,	//���ڷ��
		10, 10, 500, 300,	//����λ��
		NULL,	//�����ھ��
		NULL,	//�˵����
		hModule,	//ʵ�����
		NULL	//����WM_CREATE��Ϣʱ�ĸ��Ӳ���
	);


	SetTimer(hWnd, 1, 1000, RegisterWnd);

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

// ������Ϣע��
void  CALLBACK RegisterWnd(HWND   hwnd, UINT   uMsg, UINT   idEvent, DWORD   dwTime)
{
	// ע��ɹ�
	if (isRegisterWnd) {
		KillTimer(hwnd, 1);
	}
	else {
		//���ƴ���
		HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
		if (hWeChatRoot == NULL)
		{
			LogRecord(L"δ���ҵ�WeChatCtl����", ofs);
			return;
		}

		COPYDATASTRUCT chatmsg;
		chatmsg.dwData = WM_RegWeChatHelper;// ����һ����ֵ, ������������־��
		chatmsg.cbData = sizeof(WeChatHelper);// �����͵����ݵĳ�
		chatmsg.lpData = (LPVOID)WeChatHelper;// �����͵����ݵ���ʼ��ַ
		SendMessage(hWeChatRoot, WM_COPYDATA, NULL, (LPARAM)&chatmsg);


		// ����ע��
		json o;
		o["WeChatHelperName"] = stringToUTF8(LPCWSTRtoString(WeChatHelper));
		HttpRequest httpReq("127.0.0.1", 6877);
		std::string res = httpReq.HttpPost("/wechatRegister", o.dump());
		std::string body = httpReq.getBody(res);
		int code = 201;
		if (body != "") {
			auto bd = json::parse(body);
			code = bd["code"].get<int>();
		}

		if (code==200) {
			isRegisterWnd = true;
			LogRecord(L"ע��ɹ�", ofs);
		}
		else {
			LogRecord(L"ע��ʧ��", ofs);
		}
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
		case WM_HookAntiRevoke: {
			LogRecord(L"�յ�WM_HookAntiRevokeָ��", ofs);
			HOOK_AntiRevoke();
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
