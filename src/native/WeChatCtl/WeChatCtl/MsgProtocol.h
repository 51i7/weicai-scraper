#pragma once
#include <string>

# define WeChatMessageA(size) (WeChatMessage*) std::malloc(size)

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

// ���ڼ�ͨ��Э��

#define WM_ShowQrCode 1		// ȡ��΢�ŵ�½��ά��
#define WM_CheckIsLogin 501	// ����Ƿ��¼
#define WM_HookReciveMsg 502	// hook��Ϣ����
#define WM_ReciveMsg 503	// ��Ϣ����


void sendWeChatMessage(WeChatMessage *msg);