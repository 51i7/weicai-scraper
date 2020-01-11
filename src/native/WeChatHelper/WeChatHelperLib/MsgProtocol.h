#pragma once
#include <string>
#include <vector>


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

	//====��ȡ��ϢHOOK====
	bool enable_GetItemInfo_Hook = false;
	CHAR GetItemInfo_Hook[5] = { 0 };

};

// WeChatHookReg
struct WeChatHookReg
{
	DWORD pProcessId;
	wchar_t WeChatHelperName[1];
};

// 
struct WeChatLoginInfo
{
	wchar_t WechatName[100];
};

struct UserInfo
{
	wchar_t wxid[120]; // ΢��ID
	wchar_t wxname[120]; // ΢�ź�
	wchar_t wxv1[120]; //v1_
	wchar_t realname[120]; // ����
	wchar_t nickname[120]; // �ǳ�
	DWORD sex; // �Ա�
	wchar_t f_nickname[80]; // �ǳ�����ĸ
	wchar_t p_nickname[200]; // �ǳ�ƴ��
	wchar_t f_realname[80]; // ��������ĸ
	wchar_t p_realname[200]; // ����ƴ��
};

extern WeChatHookPoint *sWeChatHookPoint;
extern WeChatHookReg *sWeChatHookReg;
extern WeChatLoginInfo *sWeChatLoginInfo;
extern std::vector<std::wstring> vUserList;

//====��Ϣ״̬����Ϣ��ʾ====

//====������������Ϣ��ʾ====
#define WM_HookReciveMsg 502	// hook��Ϣ����
#define WM_HookAntiRevoke 504 // hook��Ϣ����
#define WM_ShowQrCode 1		// ȡ��΢�ŵ�½��ά��
#define WM_CheckIsLogin 501	// ����Ƿ��¼
#define WM_ReciveMsg 503	// ��Ϣ����
#define WM_RegWeChatHelper 505 // WeChatHelperע��
#define WM_UnRegWeChatHelper 506 // WeChatHelperע��
#define WM_OpenWeChat 507 //�࿪΢��
#define WM_GetFriendList 508	// �����б����

int IsLogin();
WeChatLoginInfo * GetWechatLoginInfo();

//====������ϢHOOK====
void HOOK_ReciveMsg();
void UnHOOK_ReciveMsg();
void RecieveWxMesage();
void SendWxMessage();

void HOOK_AntiRevoke();

void HOOK_GetItemInfo();
void GetItemInfo();
void SendItemInfo();
void UnHOOK_GetItemInfo();