#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <shlwapi.h>
#include "MsgProtocol.h"
#include "HookOffset.h"
#include "LogRecord.h"

// hookָ�� 
BYTE WX_SaveQrCode_jmpCode[5] = { 0 };
// ԭʼָ���
BYTE WX_SaveQrCode_BACK_HOOK[5] = { 0 };
// ���ص�ַ
DWORD WX_SaveQrCode_RetAddr = 0;
// ecx�Ĵ���
DWORD qrcode_ecx = 0;

void AntiRevoke()
{
	unsigned char fix[1] = { 0xEB };
	DWORD dwPathcAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll") + offset_WxAntiRevoke;
	//�޸Ĵ��������
	DWORD dwOldAttr = 0;
	VirtualProtect((LPVOID)dwPathcAddr, 1, PAGE_EXECUTE_READWRITE, &dwOldAttr);

	//Patch
	memcpy((LPVOID)dwPathcAddr, fix, 1);

	//�ָ�����
	VirtualProtect((LPVOID)dwPathcAddr, 5, dwOldAttr, &dwOldAttr);
}

// ��ʾ΢�ŵ�½��ά��
void WX_CallShowQrCode()
{

	LogRecord(L"WX_CallShowQrCode", ofs);

	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD dwCallAddr1 = dwWeChatWinAddr + offset_GoToQrCode1;
	DWORD dwCallAddr2 = dwWeChatWinAddr + offset_GoToQrCode2;

	__asm {
		call dwCallAddr1;
		mov ecx, eax;
		call dwCallAddr2;
	}
}

// ����΢�ŵ�½��ά��
void HOOK_SaveQrCode()
{

	LogRecord(L"HOOK_SaveQrCode", ofs);

	//�õ�ģ���ַ
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	//������ҪHOOK�ĵ�ַ
	DWORD dwHookAddr = dwWeChatWinAddr + offset_QrCode;
	//���㷵�ص�ַ
	WX_SaveQrCode_RetAddr = dwWeChatWinAddr + offset_QrCode + 5;
	
	//�������
	WX_SaveQrCode_jmpCode[0] = 0xE9;

	//����ƫ��
	*(DWORD*)(&WX_SaveQrCode_jmpCode[1]) = (DWORD)invokeSaveImg - dwHookAddr - 5;

	// ������ǰ���������ڻ�ԭ
	DWORD OldProtext = 0;

	// ��ΪҪ�������д�����ݣ�����Ϊ������ǲ���д�ģ�������Ҫ�޸�����
	VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	//����ԭ�е�ָ��
	memcpy(WX_SaveQrCode_BACK_HOOK, (void*)dwHookAddr, 5);

	//д���Լ��Ĵ���
	memcpy((void*)dwHookAddr, WX_SaveQrCode_jmpCode, 5);

	// ִ�����˲���֮����Ҫ���л�ԭ
	VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}


void __declspec(naked) invokeSaveImg()
{
	//���ݼĴ���
	__asm pushad;
	__asm pushfd;
	//ȡ��ecx������
	__asm mov qrcode_ecx, ecx;
	SaveImg(qrcode_ecx);
	//�ָ��Ĵ���
	__asm popfd;
	__asm popad;

	//��ת�����ص�ַ
	__asm jmp WX_SaveQrCode_RetAddr;
}

void SaveImg(DWORD qrcode)
{

	LogRecord(L"SaveImg", ofs);
	//��ȡͼƬ����
	DWORD dwPicLen = qrcode + 0x4;
	size_t cpyLen = (size_t)*((LPVOID*)dwPicLen);
	//����ͼƬ������
	char PicData[0xFFF] = { 0 };
	memcpy(PicData, *((LPVOID*)qrcode), cpyLen);

	char szTempPath[MAX_PATH] = { 0 };
	char szPicturePath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, szTempPath);

	sprintf_s(szPicturePath, "%s%s", szTempPath, "qrcode.png");
	//���ļ�д��TempĿ¼��
	HANDLE hFile = CreateFileA(szPicturePath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		LogRecord(L"����ͼƬ�ļ�ʧ��", ofs);
		return;
	}

	DWORD dwRead = 0;
	if (WriteFile(hFile, PicData, cpyLen, &dwRead, NULL) == 0)
	{
		LogRecord(L"д��ͼƬ�ļ�ʧ��", ofs);
		return;
	}
	CloseHandle(hFile);

	//���֮��ж��HOOK
	WX_SaveQrCode_Unhook(offset_QrCode);

}

// ж��WX_SaveQrCode
void WX_SaveQrCode_Unhook(DWORD dwHookOffset)
{

	LogRecord(L"WX_SaveQrCode_Unhook", ofs);
	//�õ�ģ���ַ
	DWORD dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");

	DWORD dwHookAddr = dwWeChatWinAddr + dwHookOffset;

	// ������ǰ���������ڻ�ԭ
	DWORD OldProtext = 0;

	// ��ΪҪ�������д�����ݣ�����Ϊ������ǲ���д�ģ�������Ҫ�޸�����
	VirtualProtect((LPVOID*)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	// Hook ����������д���Լ��Ĵ���
	memcpy((LPVOID*)dwHookAddr, WX_SaveQrCode_BACK_HOOK, 5);

	// ִ�����˲���֮����Ҫ���л�ԭ
	VirtualProtect((LPVOID*)dwHookAddr, 5, OldProtext, &OldProtext);
}


CHAR WX_ReciveMsg_BACK_HOOK[5] = { 0 };
DWORD ReciveMsg_esp = 0;
DWORD ReciveMsg_eax = 0;
DWORD WX_ReciveMsg_HOOK = 0;
//������ƫ��
DWORD ReciveMsg_dwParam = 0;
//���ص�ַ
DWORD ReciveMsg_RetAddr = 0;


void HOOK_ReciveMsg() {
	if (WX_ReciveMsg_HOOK == 1) {
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
	bool r= ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, WX_ReciveMsg_BACK_HOOK, 5, 0);

	//����ָ�� B9 E8CF895C //mov ecx,0x5C89CFE8
	bool w = WriteProcessMemory(GetCurrentProcess(), (LPVOID)dwHookAddr, bJmpCode, 5, 0);

	if (r&&w) {
		//WX_ReciveMsg_HOOK = 1;
		LogRecord(L"WX_ReciveMsg_HOOK�ɹ�", ofs);
	}
	else {
		LogRecord(L"WX_ReciveMsg_HOOKʧ��", ofs);
	}
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

//��Ϣ�ṹ��
struct Message
{
	DWORD type;		//��Ϣ����
	wchar_t typeStr[10];		//��Ϣ����
	DWORD sourceType;		//��Ϣ��Դ
	wchar_t sourceTypeStr[20];		//��Ϣ��Դ
	wchar_t msgReciver[40];		//΢��ID/ȺID
	wchar_t msgSender[40];	//��Ϣ������
	wchar_t content[1];	//��Ϣ����
};

bool endWith(const std::string &str, const std::string &tail) {
	return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

bool startWith(const std::string &str, const std::string &head) {
	return str.compare(0, head.size(), head) == 0;
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
	DWORD len = sizeof(Message)+sizeof(wchar_t)*wcslen(c);
	Message *msg = (Message *)malloc(len);

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

// ����Ƿ��¼
void CheckIsLogin()
{
	// ��ȡWeChatWin�Ļ�ַ
	DWORD  dwWeChatWinAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD dwIsLogin = dwWeChatWinAddr + offset_LoginSign + 0x194;
	// ���ƴ���
	HWND hWeChatRoot = FindWindow(NULL, L"Login");
	if (hWeChatRoot == NULL)
	{
		OutputDebugStringA("δ���ҵ�WeChatRoot����");
		return;
	}
	COPYDATASTRUCT login_msg;
	login_msg.dwData = WM_CheckIsLogin;
	login_msg.lpData = (LPVOID)dwIsLogin;
	login_msg.cbData = sizeof((DWORD*)dwIsLogin);
	// ������Ϣ�����ƶ�
	SendMessage(hWeChatRoot, WM_COPYDATA, NULL, (LPARAM)&login_msg);
}