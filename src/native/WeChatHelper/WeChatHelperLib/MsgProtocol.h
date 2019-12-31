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

// hook״̬
struct WeChatHookPoint
{
	//====������ϢHOOK====
	bool enable_WX_ReciveMsg_Hook = false;
	CHAR WX_ReciveMsg_Hook[5] = { 0 };

};

// WeChatHookReg
struct WeChatHookReg
{
	DWORD pProcessId;
	wchar_t WeChatHelperName[1];
};

extern WeChatHookPoint *sWeChatHookPoint;
extern WeChatHookReg *sWeChatHookReg;

//====��Ϣ״̬����Ϣ��ʾ====

//====������������Ϣ��ʾ====
#define WM_HookReciveMsg 502	// hook��Ϣ����
#define WM_HookAntiRevoke 504 // hook��Ϣ����
#define WM_ShowQrCode 1		// ȡ��΢�ŵ�½��ά��
#define WM_CheckIsLogin 501	// ����Ƿ��¼
#define WM_ReciveMsg 503	// ��Ϣ����
#define WM_RegWeChatHelper 505 // WeChatHelperע��
#define WM_UnRegWeChatHelper 506 // WeChatHelperע��


//====������ϢHOOK====
void HOOK_ReciveMsg();
void UnHOOK_ReciveMsg();
void RecieveWxMesage();
void SendWxMessage();
std::wstring GetMsgByAddress(DWORD memAddress);

