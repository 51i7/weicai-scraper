#include "stdafx.h"
#include "stdio.h"
#include "WndMsgLoop.h"
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
HWND hWnd;
HMODULE dlModule;
HANDLE hThread;
DWORD checkFailNum;
DWORD pProcessId;

// ��ʼ����Ϣѭ������
void InitWindow(HMODULE hModule)
{
	LogRecord(L"InitWindow", ofs);
	dlModule = hModule;
	checkFailNum = 0;

	UUID uuid;
	UuidCreate(&uuid);
	char *str;
	UuidToStringA(&uuid, (RPC_CSTR*)&str);

	char ty[1024] = { 0 };
	sprintf_s(ty, sizeof(ty), "%s%s", "WeChatHelper", str);
	TCHAR tstr[1024] = TEXT("");
	CharToTchar(ty, tstr);
	WeChatHelper = (LPCWSTR)tstr;
	LogRecord(WeChatHelper, ofs);

	RpcStringFreeA((RPC_CSTR*)&str);

	RegisterWindow(hModule);
}

void UnloadProc(HMODULE hModule) {
	LogRecord(L"����ж��dll", ofs);
	FreeLibraryAndExitThread(dlModule, 0);
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
	hWnd = CreateWindow(
		WeChatHelper,	//��������
		WeChatHelper,	//������
		WS_OVERLAPPEDWINDOW,	//���ڷ��
		10, 10, 500, 300,	//����λ��
		NULL,	//�����ھ��
		NULL,	//�˵����
		hModule,	//ʵ�����
		NULL	//����WM_CREATE��Ϣʱ�ĸ��Ӳ���
	);

	SetTimer(hWnd, 1, 1*1000, Do_RegisterWeChatHelper);
	SetTimer(hWnd, 2, 60*1000, Do_CheckWeChatCtrl);

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
void UnRegisterWeChatHelper() {
	//���ƴ���
	HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
	if (hWeChatRoot == NULL)
	{
		LogRecord(L"UnRegisterWeChatHelper:δ���ҵ�WeChatCtl����", ofs);
		return;
	}

	const wchar_t* c = StringToWchar_t(LPCWSTRtoString(WeChatHelper));

	//���䳤��
	DWORD len = sizeof(WeChatHookReg) + sizeof(wchar_t*)*wcslen(c) + 1;
	WeChatHookReg *msg = (WeChatHookReg *)malloc(len);

	COPYDATASTRUCT chatmsg;
	chatmsg.dwData = WM_UnRegWeChatHelper;

	msg->pProcessId = pProcessId;
	wcscpy_s(msg->WeChatHelperName, wcslen(c) + 1, c);

	chatmsg.cbData = len;// �����͵����ݵĳ�
	chatmsg.lpData = msg;// �����͵����ݵ���ʼ��ַ

	SendMessage(hWeChatRoot, WM_COPYDATA, NULL, (LPARAM)&chatmsg);

	isRegisterWnd = false;

	if(hThread)
	CloseHandle(hThread);

	// δ֪ԭ������ڴ˴�ʹ��sock���͵Ļ����޷����͡�
}

void RegisterWeChatHelper() {
	//���ƴ���
	HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
	if (hWeChatRoot == NULL)
	{
		LogRecord(L"RegisterWeChatHelper:δ���ҵ�WeChatCtl����", ofs);
		return;
	}

	const wchar_t* c = StringToWchar_t(LPCWSTRtoString(WeChatHelper));

	//���䳤��
	DWORD len = sizeof(WeChatHookReg) + sizeof(wchar_t*)*wcslen(c) + 1;
	WeChatHookReg *msg = (WeChatHookReg *)malloc(len);

	COPYDATASTRUCT chatmsg;
	chatmsg.dwData = WM_RegWeChatHelper;// ����һ����ֵ, ������������־��

	msg->pProcessId = pProcessId;
	wcscpy_s(msg->WeChatHelperName, wcslen(c) + 1, c);

	chatmsg.cbData = len;// �����͵����ݵĳ�
	chatmsg.lpData = msg;// �����͵����ݵ���ʼ��ַ

	SendMessage(hWeChatRoot, WM_COPYDATA, NULL, (LPARAM)&chatmsg);

	WeChatLoginInfo *sWeChatLoginInfo = GetWechatLoginInfo();

	// ����ע��
	json o;
	o["WeChatHelperName"] = stringToUTF8(LPCWSTRtoString(WeChatHelper));
	o["Act"] = "RegisterWeChatHelper";
	o["ProcessId"] = pProcessId;
	o["Login_WechatName"] = stringToUTF8(TcharToChar(sWeChatLoginInfo->WechatName));

	HttpRequest httpReq("127.0.0.1", 6877);
	std::string res = httpReq.HttpPost("/wechatRobot", o.dump());
	std::string body = httpReq.getBody(res);
	int code = 201;
	if (body != "") {
		auto bd = json::parse(body);
		code = bd["code"].get<int>();
	}

	if (code == 200) {
		isRegisterWnd = true;
		LogRecord(L"RegisterWeChatHelper:WeChatHelperע��ɹ�", ofs);
	}
	else {
		isRegisterWnd = false;
		LogRecord(L"RegisterWeChatHelper:WeChatHelperע��ʧ��", ofs);
	}
}

// ������Ϣע��
void  CALLBACK Do_RegisterWeChatHelper(HWND   hwnd, UINT   uMsg, UINT   idEvent, DWORD   dwTime)
{
	// ע��ɹ�
	if (isRegisterWnd) {
		KillTimer(hwnd, 1);
	}
	else {
		RegisterWeChatHelper();
	}
}

// ���ƶ��������
void  CALLBACK Do_CheckWeChatCtrl(HWND   hwnd, UINT   uMsg, UINT   idEvent, DWORD   dwTime)
{
	HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
	if (hWeChatRoot == NULL)
	{
		// ���ʧ��������1
		LogRecord(L"Do_CheckWeChatCtrl:���WeChatCtl:������", ofs);
		checkFailNum = checkFailNum + 1;
	}
	else {
		LogRecord(L"Do_CheckWeChatCtrl:���WeChatCtl:����,����ע��WeChatHelper", ofs);
		// һ���������ٴλָ�
		checkFailNum = 0;
		RegisterWeChatHelper();
	}
	
	// ʧ�ܴ�������
	if (checkFailNum > 5) {
		LogRecord(L"Do_CheckWeChatCtrl:���WeChatCtl:ʧ�ܴ�������,��ʼ����DLLж��", ofs);

		checkFailNum = 0;
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		LogRecord(L"Do_CheckWeChatCtrl:��ԭ���е�HOOK��", ofs);
		if (sWeChatHookPoint->enable_WX_ReciveMsg_Hook) {
			LogRecord(L"Do_CheckWeChatCtrl:��ԭWX_ReciveMsg_Hook", ofs);
			UnHOOK_ReciveMsg();
		}
		if (sWeChatHookPoint->enable_GetItemInfo_Hook) {
			LogRecord(L"Do_CheckWeChatCtrl:��ԭUnHOOK_GetFriendList", ofs);
			UnHOOK_GetItemInfo();
		}
		LogRecord(L"Do_CheckWeChatCtrl:ж��DLL", ofs);
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UnloadProc, NULL, 0, NULL);
		CloseHandle(hThread);
	}
}

// ������Ϣ�ص�
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// ������WM_COPYDATA����Ϣ
	if (Message == WM_COPYDATA)
	{
		LogRecord(L"�յ�WM_COPYDATA����Ϣ-��ʼ", ofs);
		COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
		LogRecord(L"switch type", ofs);
		switch (pCopyData->dwData)
		{
		case WM_CheckIsLogin: {
			LogRecord(L"�յ�WM_CheckIsLoginָ��", ofs);
			break;
		}
		case WM_HookReciveMsg: {
			LogRecord(L"�յ�WM_HookReciveMsgָ��", ofs);
			HOOK_ReciveMsg();
			HOOK_AntiRevoke();
			HOOK_GetItemInfo();
			break;
		}
		case WM_HookAntiRevoke: {
			LogRecord(L"�յ�WM_HookAntiRevokeָ��", ofs);
			break;
		}
		case WM_ShowQrCode: {
			LogRecord(L"�յ�WM_ShowQrCodeָ��", ofs);
			break;
		}
		default:

			LogRecord(L"Not_Support_Msg", ofs);
			break;
		}
		LogRecord(L"�յ�WM_COPYDATA����Ϣ-����", ofs);
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}
