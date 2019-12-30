#pragma once
#include <string>

//��Ϣ�ṹ��
struct WeChatMessage
{
	DWORD type;		//��Ϣ����
	wchar_t typeStr[80];		//��Ϣ����
	DWORD sourceType;		//��Ϣ��Դ
	wchar_t sourceTypeStr[80];		//��Ϣ��Դ
	wchar_t msgReciver[40];		//΢��ID/ȺID
	wchar_t msgSender[40];	//��Ϣ������
	wchar_t content[1];	//��Ϣ����
};

//====��Ϣ״̬����Ϣ��ʾ====

//====������������Ϣ��ʾ====
#define WM_HookReciveMsg 502	// hook��Ϣ����
#define WM_HookAntiRevoke 504 // hook��Ϣ����
#define WM_ShowQrCode 1		// ȡ��΢�ŵ�½��ά��
#define WM_CheckIsLogin 501	// ����Ƿ��¼
#define WM_ReciveMsg 503	// ��Ϣ����
#define WM_RegWeChatHelper 505 // WeChatHelperע��
#define WM_UnRegWeChatHelper 506 // WeChatHelperע��


//====���븲д��HOOK====
void HOOK_ReciveMsg();
void HOOK_AntiRevoke();
void HOOK_SaveQrCode();

//====����ִ����HOOK====
// ��ʾ��½��ά��
void WX_CallShowQrCode();
// ж��WX_SaveQrCode
void WX_SaveQrCode_Unhook(DWORD dwHookOffset);
void invokeSaveImg();
void SaveImg(DWORD qrcode);
// ����Ƿ��¼
void CheckIsLogin();

void RecieveWxMesage();
void SendWxMessage();

std::wstring GetMsgByAddress(DWORD memAddress);