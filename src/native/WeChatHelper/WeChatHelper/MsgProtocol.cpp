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
		//5B950573  |.  B9 E8CF895C           mov ecx,WeChatWi.5C89CFE8
		//mov ecx,0x5C89CFE8
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
	wchar_t type[10];		//��Ϣ����
	wchar_t source[20];		//��Ϣ��Դ
	wchar_t wxid[40];		//΢��ID/ȺID
	wchar_t msgSender[40];	//��Ϣ������
	wchar_t content[0x8000];	//��Ϣ����
	BOOL isMoney = FALSE;	//�Ƿ����տ���Ϣ
};

void SendWxMessage()
{


	LogRecord(L"SendWxMessage", ofs);
	Message *msg = new Message;

	//��Ϣ���λ��
	DWORD** msgAddress = (DWORD * *)ReciveMsg_esp;
	//��Ϣ����
	DWORD msgType = *((DWORD*)(**msgAddress + 0x30));

	BOOL isFriendMsg = FALSE;		//�Ƿ��Ǻ�����Ϣ
	BOOL isImageMessage = FALSE;	//�Ƿ���ͼƬ��Ϣ
	BOOL isRadioMessage = FALSE;	//�Ƿ�����Ƶ��Ϣ
	BOOL isVoiceMessage = FALSE;	//�Ƿ���������Ϣ
	BOOL isBusinessCardMessage = FALSE;	//�Ƿ�����Ƭ��Ϣ
	BOOL isExpressionMessage = FALSE;	//�Ƿ�����Ƭ��Ϣ
	BOOL isLocationMessage = FALSE;	//�Ƿ���λ����Ϣ
	BOOL isSystemMessage = FALSE;	//�Ƿ���ϵͳ������Ϣ
	BOOL isFriendRequestMessage = FALSE;	//�Ƿ��Ǻ���������Ϣ
	BOOL isOther = FALSE;	//�Ƿ���������Ϣ

	switch (msgType)
	{
	case 0x01:
		memcpy(msg->type, L"����", sizeof(L"����"));
		break;
	case 0x03:
		memcpy(msg->type, L"ͼƬ", sizeof(L"ͼƬ"));
		isImageMessage = TRUE;
		break;
	case 0x22:
		memcpy(msg->type, L"����", sizeof(L"����"));
		isVoiceMessage = TRUE;
		break;
	case 0x25:
		memcpy(msg->type, L"����ȷ��", sizeof(L"����ȷ��"));
		isFriendRequestMessage = TRUE;
		break;
	case 0x28:
		memcpy(msg->type, L"POSSIBLEFRIEND_MSG", sizeof(L"POSSIBLEFRIEND_MSG"));
		isOther = TRUE;
		break;
	case 0x2A:
		memcpy(msg->type, L"��Ƭ", sizeof(L"��Ƭ"));
		isBusinessCardMessage = TRUE;
		break;
	case 0x2B:
		memcpy(msg->type, L"��Ƶ", sizeof(L"��Ƶ"));
		isRadioMessage = TRUE;
		break;
	case 0x2F:
		//ʯͷ������
		memcpy(msg->type, L"����", sizeof(L"����"));
		isExpressionMessage = TRUE;
		break;
	case 0x30:
		memcpy(msg->type, L"λ��", sizeof(L"λ��"));
		isLocationMessage = TRUE;
		break;
	case 0x31:
		//����ʵʱλ��
		//�ļ�
		//ת��
		//����
		//�տ�
		memcpy(msg->type, L"����ʵʱλ�á��ļ���ת�ˡ�����", sizeof(L"����ʵʱλ�á��ļ���ת�ˡ�����"));
		isOther = TRUE;
		break;
	case 0x32:
		memcpy(msg->type, L"VOIPMSG", sizeof(L"VOIPMSG"));
		isOther = TRUE;
		break;
	case 0x33:
		memcpy(msg->type, L"΢�ų�ʼ��", sizeof(L"΢�ų�ʼ��"));
		isOther = TRUE;
		break;
	case 0x34:
		memcpy(msg->type, L"VOIPNOTIFY", sizeof(L"VOIPNOTIFY"));
		isOther = TRUE;
		break;
	case 0x35:
		memcpy(msg->type, L"VOIPINVITE", sizeof(L"VOIPINVITE"));
		isOther = TRUE;
		break;
	case 0x3E:
		memcpy(msg->type, L"С��Ƶ", sizeof(L"С��Ƶ"));
		isRadioMessage = TRUE;
		break;
	case 0x270F:
		memcpy(msg->type, L"SYSNOTICE", sizeof(L"SYSNOTICE"));
		isOther = TRUE;
		break;
	case 0x2710:
		//ϵͳ��Ϣ
		//���
		memcpy(msg->type, L"�����ϵͳ��Ϣ", sizeof(L"�����ϵͳ��Ϣ"));
		isSystemMessage = TRUE;
		break;
	default:
		memcpy(msg->type, std::to_string(msgType).c_str(), sizeof(std::to_string(msgType).c_str()));
		isOther = TRUE;
		break;
	}
	const wchar_t* c = L"";
	//��Ϣ����
	std::wstring fullmessgaedata = GetMsgByAddress(**msgAddress + 0x68);	//��������Ϣ����
	//�ж���Ϣ��Դ��Ⱥ��Ϣ���Ǻ�����Ϣ
	std::wstring msgSource2 = L"<msgsource />\n";
	std::wstring msgSource = L"";
	msgSource.append(GetMsgByAddress(**msgAddress + 0x168));
	//������Ϣ
	if (msgSource.length() <= msgSource2.length())
	{
		memcpy(msg->source, L"������Ϣ", sizeof(L"������Ϣ"));
		isFriendMsg = TRUE;
	}
	else
	{
		//Ⱥ��Ϣ
		memcpy(msg->source, L"Ⱥ��Ϣ", sizeof(L"Ⱥ��Ϣ"));
	}

	//��ʾ΢��ID/ȺID
	LPVOID pWxid = *((LPVOID *)(**msgAddress + 0x40));
	swprintf_s(msg->wxid, L"%s", (wchar_t*)pWxid);

	//�����Ⱥ��Ϣ
	if (isFriendMsg == FALSE)
	{
		//��ʾ��Ϣ������
		LPVOID pSender = *((LPVOID *)(**msgAddress + 0x114));
		swprintf_s(msg->msgSender, L"%s", (wchar_t*)pSender);
	}
	else
	{
		memcpy(msg->msgSender, L"NULL", sizeof(L"NULL"));
	}

	if (wcscmp(msg->wxid, L"gh")) {
		if ((wcscmp(msg->wxid, L"gh_3dfda90e39d6") == 0))
		{
			c = L"΢���տ��";
			msg->isMoney = TRUE;
		}
		else
		{
			memcpy(msg->type, L"���ں�����", sizeof(L"���ں�����"));
			c = fullmessgaedata.c_str();
		}
	}//����ͼƬ��Ϣ 
	else if (isImageMessage == TRUE)
	{
		c = L"�յ�ͼƬ��Ϣ,�����ֻ��ϲ鿴";
	}
	else if (isRadioMessage == TRUE)
	{
		c = L"�յ���Ƶ��Ϣ,�����ֻ��ϲ鿴";
	}
	else if (isVoiceMessage == TRUE)
	{
		c = L"�յ�������Ϣ,�����ֻ��ϲ�";
	}
	else if (isBusinessCardMessage == TRUE)
	{
		c = L"�յ���Ƭ��Ϣ,���Զ���Ӻ���";
	}
	else if (isExpressionMessage == TRUE)
	{
		c = L"�յ�������Ϣ,�����ֻ��ϲ鿴";
	}
	//�Զ�ͨ����������
	else if (isFriendRequestMessage == TRUE)
	{
		c = L"���յ���������,���Զ�ͨ��";
	}
	else if (isOther == TRUE)
	{
		//ȡ����Ϣ����
		wchar_t tempcontent[0x10000] = { 0 };
		LPVOID pContent = *((LPVOID *)(**msgAddress + 0x68));
		swprintf_s(tempcontent, L"%s", (wchar_t*)pContent);
		//�ж��Ƿ���ת����Ϣ
		if (wcscmp(tempcontent, L"΢��ת��"))
		{
			c = L"�յ�ת����Ϣ,���Զ��տ�";
		}
		else
		{
			c = L"�յ�����ʵʱλ�á��ļ������ӵ�������Ϣ,�����ֻ��ϲ鿴";
			c = fullmessgaedata.c_str();

		}
	}
	else if (isLocationMessage == TRUE)
	{
		c = L"�յ�λ����Ϣ,�����ֻ��ϲ鿴";
	}
	else if (isSystemMessage == TRUE)
	{
		wchar_t tempbuff[0x1000];
		LPVOID pContent = *((LPVOID *)(**msgAddress + 0x68));
		swprintf_s(tempbuff, L"%s", (wchar_t*)pContent);

		//�����ﴦ�����Ⱥ����Ϣ
		if ((wcscmp(tempbuff, L"�Ƴ���Ⱥ��") || wcscmp(tempbuff, L"������Ⱥ��")))
		{
			c = (wchar_t*)tempbuff;
		}
		else
		{
			c = L"�յ������ϵͳ��Ϣ,�����ֻ��ϲ鿴";
		}

	}
	//������������Ϣ֮��
	else
	{
		c = fullmessgaedata.c_str();
	}
	//swprintf_s(msg->content, wcslen(c) * sizeof(wchar_t), (wchar_t*)c);

	//msg->content = (wchar_t*)std::malloc(wcslen(c) * sizeof(wchar_t));
	wcscpy_s(msg->content, wcslen(c) + 1, c);

	//���ƴ���
	HWND hWeChatRoot = FindWindow(NULL, L"WeChatCtl");
	if (hWeChatRoot == NULL)
	{
		LogRecord(L"δ���ҵ�WeChatCtl����", ofs);
		return;
	}

	COPYDATASTRUCT chatmsg;
	chatmsg.dwData = WM_ReciveMsg;//����һ����ֵ, ������������־��
	chatmsg.cbData = sizeof(Message);// strlen(szSendBuf);//�����͵����ݵĳ�
	chatmsg.lpData = msg;// szSendBuf;//�����͵����ݵ���ʼ��ַ(����ΪNULL)
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