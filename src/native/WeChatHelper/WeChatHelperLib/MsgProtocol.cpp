#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <shlwapi.h>
#include "MsgProtocol.h"
#include "StringTool.h"
#include "HookOffset.h"
#include "LogRecord.h"
#include "HttpRequest.h"
#include "json.hpp"

using json = nlohmann::json;;

//====��Ϣ����hook ��ʼ===
DWORD ReciveMsg_esp = 0;
DWORD ReciveMsg_eax = 0;
DWORD ReciveMsg_dwParam = 0;
DWORD ReciveMsg_RetAddr = 0;

WeChatHookPoint * sWeChatHookPoint = new WeChatHookPoint();
WeChatHookReg *sWeChatHookReg = new WeChatHookReg();

void HOOK_ReciveMsg() {
	if (sWeChatHookPoint->enable_WX_ReciveMsg_Hook) {
		LogRecord(L"�Ѿ�����WX_ReciveMsg_HOOK", ofs);
		return;
	}
	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	LogRecord(L"WeChatWin.dll ��ַ", ofs);
	LogRecord(char2TCAHR(std::to_string(WeChatWinBaseAddr).c_str()), ofs);

	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = WeChatWinBaseAddr + offset_ReciveMessage;
	ReciveMsg_dwParam = WeChatWinBaseAddr + offset_ReciveMessageParam;
	ReciveMsg_RetAddr = dwHookAddr + 5;

	LogRecord(L"ReciveMsg_HOOK ��ַ", ofs);
	LogRecord(char2TCAHR(std::to_string(dwHookAddr).c_str()), ofs);

	LogRecord(L"ReciveMsg_RetAddr ��ַ", ofs);
	LogRecord(char2TCAHR(std::to_string(ReciveMsg_RetAddr).c_str()), ofs);

	//��װ����
	BYTE bJmpCode[5] = { 0xE9 };
	*(DWORD*)&bJmpCode[1] = (DWORD)RecieveWxMesage - dwHookAddr - 5;

	//���浱ǰλ�õ�ָ��,��unhook��ʱ��ʹ�á�
	bool r= ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, sWeChatHookPoint->WX_ReciveMsg_Hook, 5, 0);

	//����ָ�� B9 E8CF895C //mov ecx,0x5C89CFE8
	bool w = WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, bJmpCode, 5, 0);

	if (r&&w) {
		sWeChatHookPoint->enable_WX_ReciveMsg_Hook = true;
		LogRecord(L"WX_ReciveMsg_HOOK�ɹ�", ofs);
	}
	else {
		LogRecord(L"WX_ReciveMsg_HOOKʧ��", ofs);
	}
}
void UnHOOK_ReciveMsg()
{
	if (!sWeChatHookPoint->enable_WX_ReciveMsg_Hook) {
		LogRecord(L"WX_ReciveMsg_HOOK������", ofs);
		return;
	}

	LogRecord(L"UnHOOK_ReciveMsg", ofs);
	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = WeChatWinBaseAddr + offset_ReciveMessage;

	// ԭ����
	DWORD OldProtext = 0;

	// ���Ŀɶ�д
	VirtualProtect((LPVOID*)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	// ��ԭԭʼָ��
	memcpy((LPVOID*)dwHookAddr, sWeChatHookPoint->WX_ReciveMsg_Hook, 5);

	// ���Ի�ԭ
	VirtualProtect((LPVOID*)dwHookAddr, 5, OldProtext, &OldProtext);
}


__declspec(naked) void RecieveWxMesage()
{
	//�����ֳ�
	__asm
	{
		//���䱻���ǵĴ���
		//mov ecx,WeChatWin.dll+1624908 { (56B097FC) }
		mov ecx, ReciveMsg_dwParam

		//��ȡesp�Ĵ������ݣ�����һ��������
		mov ReciveMsg_esp, esp
		mov ReciveMsg_eax, eax

		pushad
		pushfd
	}

	SendWxMessage();

	//�ָ��ֳ�
	__asm
	{
		popfd
		popad
		//���ر�HOOKָ�����һ��ָ��
		jmp ReciveMsg_RetAddr
	}
}

void SendWxMessage()
{


	LogRecord(L"SendWxMessage", ofs);

	//��Ϣ���λ��
	DWORD** msgAddress = (DWORD * *)ReciveMsg_esp;
	//��Ϣ����
	DWORD msgType = *((DWORD*)(**msgAddress + offset_ReciveMessageParam_MsgType));
	//��Ϣ��Դ����
	DWORD msgSource = *((DWORD*)(**msgAddress + offset_ReciveMessageParam_MsgSourceType));
	//��Ϣ������
	LPVOID pSender = *((LPVOID *)(**msgAddress + offset_ReciveMessageParam_MsgSender));
	//��Ϣ������
	LPVOID pWxid = *((LPVOID *)(**msgAddress + offset_ReciveMessageParam_MsgReciver));
	//��������Ϣ����
	std::wstring msgContent = GetMsgByAddress(**msgAddress + offset_ReciveMessageParam_MsgContent);
	const wchar_t* c = msgContent.c_str();

	//���䳤��
	DWORD len = sizeof(WeChatMessage)+sizeof(wchar_t)*wcslen(c);
	WeChatMessage *msg = (WeChatMessage *)malloc(len);

	msg->type = msgType;
	msg->sourceType = msgSource;
	wcscpy_s(msg->content, wcslen(c) + 1, c);

	swprintf_s(msg->msgReciver, L"%s", (wchar_t*)pWxid);
	swprintf_s(msg->msgSender, L"%s", (wchar_t*)pSender);

	switch (msgType)
	{
	case 0x01:
		memcpy(msg->typeStr, L"����", sizeof(L"����"));
		break;
	case 0x03:
		memcpy(msg->typeStr, L"ͼƬ", sizeof(L"ͼƬ"));
		break;
	case 0x22:
		memcpy(msg->typeStr, L"����", sizeof(L"����"));
		break;
	case 0x25:
		memcpy(msg->typeStr, L"����ȷ��", sizeof(L"����ȷ��"));
		break;
	case 0x28:
		memcpy(msg->typeStr, L"POSSIBLEFRIEND_MSG", sizeof(L"POSSIBLEFRIEND_MSG"));
		break;
	case 0x2A:
		memcpy(msg->typeStr, L"��Ƭ", sizeof(L"��Ƭ"));
		break;
	case 0x2B:
		memcpy(msg->typeStr, L"��Ƶ", sizeof(L"��Ƶ"));
		break;
	case 0x2F:
		//ʯͷ������
		memcpy(msg->typeStr, L"����", sizeof(L"����"));
		break;
	case 0x30:
		memcpy(msg->typeStr, L"λ��", sizeof(L"λ��"));
		break;
	case 0x31:
		//����ʵʱλ��
		//�ļ�
		//ת��
		//����
		//�տ�
		memcpy(msg->typeStr, L"����ʵʱλ�á��ļ���ת�ˡ�����", sizeof(L"����ʵʱλ�á��ļ���ת�ˡ�����"));
		break;
	case 0x32:
		memcpy(msg->typeStr, L"VOIPMSG", sizeof(L"VOIPMSG"));
		break;
	case 0x33:
		memcpy(msg->typeStr, L"΢�ų�ʼ��", sizeof(L"΢�ų�ʼ��"));
		break;
	case 0x34:
		memcpy(msg->typeStr, L"VOIPNOTIFY", sizeof(L"VOIPNOTIFY"));
		break;
	case 0x35:
		memcpy(msg->typeStr, L"VOIPINVITE", sizeof(L"VOIPINVITE"));
		break;
	case 0x3E:
		memcpy(msg->typeStr, L"С��Ƶ", sizeof(L"С��Ƶ"));
		break;
	case 0x270F:
		memcpy(msg->typeStr, L"SYSNOTICE", sizeof(L"SYSNOTICE"));
		break;
	case 0x2710:
		//ϵͳ��Ϣ
		//���
		memcpy(msg->typeStr, L"�����ϵͳ��Ϣ", sizeof(L"�����ϵͳ��Ϣ"));
		break;
	default:
		memcpy(msg->typeStr, std::to_string(msgType).c_str(), sizeof(std::to_string(msgType).c_str()));
		break;
	}

	if (msgSource == 0x01) {
		memcpy(msg->sourceTypeStr, L"������Ϣ", sizeof(L"������Ϣ"));
	}
	else {
		memcpy(msg->sourceTypeStr, L"�Ǻ�����Ϣ", sizeof(L"�Ǻ�����Ϣ"));
	}

	// �����Ƿ�Ϊ���ں���Դ
	char strOut[2 * 40] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, msg->msgReciver, sizeof(msg->msgReciver), strOut, sizeof(strOut), 0, 0);
	if (startWith(strOut, "gh_")) {
		memcpy(msg->typeStr, L"���ں�����", sizeof(L"���ں�����"));
	}

	//���ƴ���
	HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
	if (hWeChatRoot == NULL)
	{
		LogRecord(L"δ���ҵ�WeChatCtl����", ofs);
		return;
	}

	COPYDATASTRUCT chatmsg;
	chatmsg.dwData = WM_ReciveMsg;// ����һ����ֵ, ������������־��
	chatmsg.cbData = len;// �����͵����ݵĳ�
	chatmsg.lpData = msg;// �����͵����ݵ���ʼ��ַ
	SendMessage(hWeChatRoot, WM_COPYDATA, NULL, (LPARAM)&chatmsg);
}

std::wstring GetMsgByAddress(DWORD memAddress)
{
	std::wstring tmp;
	DWORD msgLength = *(DWORD*)(memAddress + 4);
	if (msgLength > 0) {
		WCHAR* msg = new WCHAR[msgLength + 1]{ 0 };
		wmemcpy_s(msg, msgLength + 1, (WCHAR*)(*(DWORD*)memAddress), msgLength + 1);
		tmp = msg;
		delete[]msg;
	}
	return  tmp;
}


//====��Ϣ����hook ����===