#include "stdafx.h"
#include "stdio.h"

#include "WndMsgLoop.h"
#include "HookOffset.h"
#include "MsgProtocol.h"
#include "StringTool.h"
#include "LogRecord.h"
#include "HttpRequest.h"
#include "json.hpp"

std::vector<WeChatHookReg> wehcatHelpers;
using json = nlohmann::json;

// ��ʼ����Ϣѭ������
void InitWindow(HMODULE hModule)
{
	RegisterWindow(hModule);
}

// ע�ᴰ�ڼ���Ϣѭ��
void RegisterWindow(HMODULE hModule)
{
	LogRecord(L"�յ�RegisterWindowMsgLoopָ��", ofs);
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
	wnd.lpszClassName = TEXT("WeChatCtl");
	//2  ע�ᴰ����
	RegisterClass(&wnd);
	//3  ��������
	HWND hWnd = CreateWindow(
		TEXT("WeChatCtl"),	//��������
		TEXT("WeChatCtl"),	//������
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
		switch (pCopyData->dwData)
		{
		case WM_CheckIsLogin: {
			LogRecord(L"�յ�WM_CheckIsLoginָ��", ofs);
			break;
		}
		case WM_HookReciveMsg: {
			LogRecord(L"�յ�WM_HookReciveMsgָ��", ofs);
			break;
		}
		case WM_ReciveMsg: {
			LogRecord(L"�յ�WM_ReciveMsgָ��", ofs);
			WeChatMessage *msg = (WeChatMessage *)malloc(pCopyData->cbData);
			msg = (WeChatMessage*)pCopyData->lpData;
			sendWeChatMessage(msg);
			break;
		}
		case WM_ShowQrCode: {
			LogRecord(L"�յ�WM_ShowQrCodeָ��", ofs);
			break;
		}
		case WM_RegWeChatHelper: {
			LogRecord(L"�յ�WM_RegWeChatHelperָ��", ofs);

			WeChatHookReg *msg = (WeChatHookReg *)malloc(pCopyData->cbData);
			msg = (WeChatHookReg*)pCopyData->lpData;

			bool isex = false;
			std::vector<WeChatHookReg>::iterator it;
			for (it = wehcatHelpers.begin(); it != wehcatHelpers.end();)
			{
				if (strcmp(Wchar_tToString(it->WeChatHelperName).c_str() , Wchar_tToString(msg->WeChatHelperName).c_str())==0)
					isex = true;
				else
					++it;
			}

			if (!isex) {
				wehcatHelpers.push_back(*msg);
			}

			LogRecord(L"wehcatHelpers size:", ofs);
			LogRecord(CharToTchar(std::to_string(wehcatHelpers.size()).c_str()), ofs);

			LogRecord(L"wehcatHelpers list:", ofs);
			LogRecord(CharToTchar(HelperListToString(wehcatHelpers).c_str()), ofs);
			break;
		}
		case WM_UnRegWeChatHelper: {
			LogRecord(L"�յ�WM_UnRegWeChatHelperָ��", ofs);

			WeChatHookReg *msg = (WeChatHookReg *)malloc(pCopyData->cbData);
			msg = (WeChatHookReg*)pCopyData->lpData;

			std::vector<WeChatHookReg>::iterator it;
			for (it=wehcatHelpers.begin(); it!=wehcatHelpers.end();)
			{
				if (strcmp(Wchar_tToString(it->WeChatHelperName).c_str(), Wchar_tToString(msg->WeChatHelperName).c_str())==0)
					it = wehcatHelpers.erase(it);
				else
					++it; 
			}

			// ����ע��
			json o;
			o["WeChatHelperName"] = stringToUTF8(LPCWSTRtoString(msg->WeChatHelperName));
			o["Act"] = "UnRegisterWeChatHelper";
			o["ProcessId"] = msg->pProcessId;
			HttpRequest httpReq("127.0.0.1", 6877);
			std::string res = httpReq.HttpPost("/wechatRegister", o.dump());
			std::string body = httpReq.getBody(res);
			int code = 201;
			if (body != "") {
				auto bd = json::parse(body);
				code = bd["code"].get<int>();
			}

			if (code == 200) {
				LogRecord(L"UnRegisterWeChatHelper:WeChatHelperע���ɹ�", ofs);
			}
			else {
				LogRecord(L"UnRegisterWeChatHelper:WeChatHelperע��ʧ��", ofs);
			}


			LogRecord(L"wehcatHelpers size:", ofs);
			LogRecord(CharToTchar(std::to_string(wehcatHelpers.size()).c_str()), ofs);

			LogRecord(L"wehcatHelpers list:", ofs);
			LogRecord(CharToTchar(HelperListToString(wehcatHelpers).c_str()), ofs);
			break;
		}
		default:
			break;
		}
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
}
