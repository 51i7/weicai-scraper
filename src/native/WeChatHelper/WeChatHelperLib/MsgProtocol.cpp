#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <shlwapi.h>
#include "MsgProtocol.h"
#include "StringTool.h"
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
WeChatLoginInfo *sWeChatLoginInfo = new WeChatLoginInfo();
std::vector<std::wstring> vUserList;

void HOOK_ReciveMsg() {
	LogRecord(L"HOOK_ReciveMsg", ofs);
	if (sWeChatHookPoint->enable_WX_ReciveMsg_Hook) {
		LogRecord(L"�Ѿ�����WX_ReciveMsg_HOOK", ofs);
		return;
	}
	if (sWechatOffset->offsetReciveMessage == 0x0) {
		LogRecord(L"δ֧�� ReciveMsg_HOOK", ofs);
		return;
	}
	if (IsLogin()!=1) {
		LogRecord(L"��δ��¼", ofs);
		return;
	}
	else {
		LogRecord(L"�ѵ�¼", ofs);
	}

	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	LogRecord(L"WeChatWin.dll ��ַ", ofs);
	LogRecord(char2TCAHR(std::to_string(WeChatWinBaseAddr).c_str()), ofs);

	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = WeChatWinBaseAddr + sWechatOffset->offsetReciveMessage;
	ReciveMsg_dwParam = WeChatWinBaseAddr + sWechatOffset->offsetReciveMessageParam;
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
	LogRecord(L"UnHOOK_ReciveMsg", ofs);

	if (sWechatOffset->offsetReciveMessage == 0x0) {
		LogRecord(L"δ֧�� ReciveMsg_HOOK", ofs);
		return;
	}

	if (!sWeChatHookPoint->enable_WX_ReciveMsg_Hook) {
		LogRecord(L"WX_ReciveMsg_HOOK������", ofs);
		return;
	}

	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = WeChatWinBaseAddr + sWechatOffset->offsetReciveMessage;

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
	DWORD msgType = *((DWORD*)(**msgAddress + sWechatOffset->offsetReciveMessageParam_MsgType));
	//��Ϣ��Դ����
	DWORD msgSource = *((DWORD*)(**msgAddress + sWechatOffset->offsetReciveMessageParam_MsgSourceType));
	//��Ϣ������
	LPVOID pSender = *((LPVOID *)(**msgAddress + sWechatOffset->offsetReciveMessageParam_MsgSender));
	//��Ϣ������
	LPVOID pWxid = *((LPVOID *)(**msgAddress + sWechatOffset->offsetReciveMessageParam_MsgReciver));
	DWORD tl = 0;
	//��������Ϣ����
	std::wstring msgContent = GetMsgByAddress(**msgAddress + sWechatOffset->offsetReciveMessageParam_MsgContent, tl);
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
	case 0x23:
		memcpy(msg->typeStr, L"��������", sizeof(L"��������"));
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
	case 0x2712:
		memcpy(msg->typeStr, L"������Ϣ", sizeof(L"������Ϣ"));
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

std::wstring GetMsgByAddress(DWORD addr, DWORD &clen)
{
	std::wstring tmp;
	DWORD len = *(DWORD*)(addr + 4);
	clen += len;
	if (len > 0) {
		WCHAR* msg = new WCHAR[len + 1]{ 0 };
		wmemcpy_s(msg, len + 1, (WCHAR*)(*(DWORD*)addr), len + 1);
		tmp = msg;
		delete[]msg;
	}
	return  tmp;
}


//====��Ϣ����hook ����===



WeChatLoginInfo * GetWechatLoginInfo() {
	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD infoAddr = WeChatWinBaseAddr + sWechatOffset->offsetLoginInfoBlock;
	DWORD tl = 0;
	wchar_t *wxid = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_WxId, tl);
	wchar_t *wxname = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_WechatName, tl);
	wchar_t *email = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Email, tl);
	wchar_t *mobile = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Mobile, tl);
	DWORD sex = *((DWORD*)(infoAddr + sWechatOffset->offsetLoginInfoBlock_Sex));
	DWORD islogin = *((DWORD*)(infoAddr + sWechatOffset->offsetLoginInfoBlock_IsLogin));
	wchar_t *province = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Province, tl);
	wchar_t *city = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_City, tl);
	wchar_t *signer = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Signer, tl);
	wchar_t *country = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Country, tl);
	wchar_t *avatar = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Avatar, tl);
	wchar_t *device = pToTchar(infoAddr + sWechatOffset->offsetLoginInfoBlock_Device, tl);

	wcscpy_s(sWeChatLoginInfo->WechatName, wcslen(wxname) + 1, wxname);

	return sWeChatLoginInfo;
}

int IsLogin() {
	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD infoAddr = WeChatWinBaseAddr + sWechatOffset->offsetLoginInfoBlock;
	int ret = (int) * (int*)(infoAddr + sWechatOffset->offsetLoginInfoBlock_IsLogin);
	return ret;
}

void HOOK_AntiRevoke()
{
	LogRecord(L"ִ�� HOOK_AntiRevoke", ofs);
	if (sWechatOffset->offsetAntiRevoke == 0x0) {
		LogRecord(L"δ֧�� HOOK_AntiRevoke", ofs);
		return;
	}
	unsigned char fix[1] = { 0xEB };
	DWORD dwPathcAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll") + sWechatOffset->offsetAntiRevoke;
	DWORD dwOldAttr = 0;

	VirtualProtect((LPVOID)dwPathcAddr, 1, PAGE_EXECUTE_READWRITE, &dwOldAttr);
	memcpy((LPVOID)dwPathcAddr, fix, 1);
	VirtualProtect((LPVOID)dwPathcAddr, 5, dwOldAttr, &dwOldAttr);
}


DWORD GetItemInfoCall_RetAddr;
DWORD GetItemInfoCall_Add;
DWORD dwItemInfo_eax;
void HOOK_GetItemInfo() {
	LogRecord(L"HOOK_GetItemInfo", ofs);
	if (sWeChatHookPoint->enable_GetItemInfo_Hook) {
		LogRecord(L"�Ѿ�����HOOK_GetItemInfo", ofs);
		return;
	}
	if (sWechatOffset->offsetInfoCall == 0x0) {
		LogRecord(L"δ֧��HOOK_GetItemInfo", ofs);
		return;
	}

	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD dwHookAddr = WeChatWinBaseAddr + sWechatOffset->offsetInfoCall;
	GetItemInfoCall_Add = WeChatWinBaseAddr + sWechatOffset->offsetInfoCallAdd;
	GetItemInfoCall_RetAddr = dwHookAddr + 5;

	//��װ����
	BYTE bJmpCode[5] = { 0xE9 };
	*(DWORD*)&bJmpCode[1] = (DWORD)GetItemInfo - dwHookAddr - 5;

	//���浱ǰλ�õ�ָ��,��unhook��ʱ��ʹ�á�
	bool r = ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, sWeChatHookPoint->GetItemInfo_Hook, 5, 0);

	//����ָ�� B9 E8CF895C //mov ecx,0x5C89CFE8
	bool w = WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, bJmpCode, 5, 0);

	if (r&&w) {
		sWeChatHookPoint->enable_GetItemInfo_Hook = true;
		LogRecord(L"HOOK_GetItemInfo�ɹ�", ofs);
	}
	else {
		LogRecord(L"HOOK_GetItemInfoʧ��", ofs);
	}
}
__declspec(naked) void GetItemInfo()
{
	__asm
	{
		// ִ�б����ǵ�ָ��
		call GetItemInfoCall_Add;

		// ������Ϣ��ַ
		mov dwItemInfo_eax, eax

		pushad
		pushf
	}

	SendItemInfo();

	__asm
	{
		popf
		popad

		//���ر�HOOKָ�����һ��ָ��
		jmp GetItemInfoCall_RetAddr
	}
}

void SendItemInfo()
{
	DWORD tl = 0;
	std::wstring wxid = GetMsgByAddress(dwItemInfo_eax + 0x10, tl);
	std::vector<std::wstring>::iterator it;
	DWORD ishas = 0;
	for (it = vUserList.begin(); it != vUserList.end();)
	{
		if (strcmp(Wchar_tToString((wchar_t *)it->c_str()).c_str(), Wchar_tToString((wchar_t *)wxid.c_str()).c_str()) == 0) {
			ishas = 1;
			break;
		}
			++it;
	}
	if (ishas == 1) {
		return;
	}
	else {
		vUserList.push_back(wxid);
	}

	UserInfo *user = new UserInfo;
	LPVOID pUserWxid = *((LPVOID *)(dwItemInfo_eax + 0x10));
	LPVOID pUserWxname = *((LPVOID *)(dwItemInfo_eax + 0x44));
	LPVOID pUserWxv1 = *((LPVOID *)(dwItemInfo_eax + 0x58));
	DWORD unflag = *((DWORD*)(dwItemInfo_eax + 0x74));
	LPVOID pUserRealname = *((LPVOID *)(dwItemInfo_eax + 0x78));
	DWORD unflag1 = *((DWORD*)(dwItemInfo_eax + 0x78));

	LPVOID pUserNickname = *((LPVOID *)(dwItemInfo_eax + 0x8C));
	DWORD accflag = *((DWORD*)(dwItemInfo_eax + 0xC8));
	LPVOID pUserFNickname = *((LPVOID *)(dwItemInfo_eax + 0xCC));
	LPVOID pUserPNickname = *((LPVOID *)(dwItemInfo_eax + 0xE0));
	LPVOID pUserFRealname = *((LPVOID *)(dwItemInfo_eax + 0xF4));
	LPVOID pUserPRealname = *((LPVOID *)(dwItemInfo_eax + 0x108));

	swprintf_s(user->wxid, L"%s", (wchar_t*)pUserWxid);
	swprintf_s(user->wxname, L"%s", (wchar_t*)pUserWxname);
	swprintf_s(user->wxv1, L"%s", (wchar_t*)pUserWxv1);
	swprintf_s(user->realname, L"%s", (wchar_t*)pUserRealname);
	swprintf_s(user->nickname, L"%s", (wchar_t*)pUserNickname);
	swprintf_s(user->f_nickname, L"%s", (wchar_t*)pUserFNickname);
	swprintf_s(user->p_nickname, L"%s", (wchar_t*)pUserPNickname);
	swprintf_s(user->f_realname, L"%s", (wchar_t*)pUserFRealname);
	swprintf_s(user->p_realname, L"%s", (wchar_t*)pUserPRealname);
	user->sex = 0;
	user->unflag = unflag;
	if (unflag == 0x0) {
		// ���� Ⱥ��
		user->type = 1;
	}
	else {
		if (unflag1 == 0x0) {
			// ���ں�
			user->type = 2;
		}
	}

	//���ƴ���
	HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
	if (hWeChatRoot == NULL)
	{
		LogRecord(L"δ���ҵ�WeChatCtl����", ofs);
		return;
	}

	COPYDATASTRUCT chatmsg;
	chatmsg.dwData = WM_GetFriendList;
	chatmsg.cbData = sizeof(UserInfo);
	chatmsg.lpData = user;
	SendMessage(hWeChatRoot, WM_COPYDATA, NULL, (LPARAM)&chatmsg);
}

void UnHOOK_GetItemInfo()
{
	LogRecord(L"UnHOOK_GetFriendList", ofs);

	LogRecord(L"GetFriendList", ofs);
	if (!sWeChatHookPoint->enable_GetItemInfo_Hook) {
		LogRecord(L"������GetFriendList_HOOK", ofs);
		return;
	}
	if (sWechatOffset->offsetGetFriendListCall == 0x0) {
		LogRecord(L"δ֧�� GetFriendList_HOOK", ofs);
		return;
	}

	DWORD WeChatWinBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = WeChatWinBaseAddr + sWechatOffset->offsetGetFriendList;

	// ԭ����
	DWORD OldProtext = 0;

	// ���Ŀɶ�д
	VirtualProtect((LPVOID*)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	// ��ԭԭʼָ��
	memcpy((LPVOID*)dwHookAddr, sWeChatHookPoint->GetItemInfo_Hook, 5);

	// ���Ի�ԭ
	VirtualProtect((LPVOID*)dwHookAddr, 5, OldProtext, &OldProtext);
}