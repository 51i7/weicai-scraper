#pragma once
#include <string>

# define WeChatMessageA(size) (WeChatMessage*) std::malloc(size)

//��Ϣ�ṹ��
struct WeChatMessage
{
	wchar_t type[10];		//��Ϣ����
	wchar_t source[20];		//��Ϣ��Դ
	wchar_t wxid[40];		//΢��ID/ȺID
	wchar_t msgSender[40];	//��Ϣ������
	wchar_t content[0x8000];	//��Ϣ����
	BOOL isMoney = FALSE;	//�Ƿ����տ���Ϣ
};

// ���ڼ�ͨ��Э��

#define WM_ShowQrCode 1		// ȡ��΢�ŵ�½��ά��
#define WM_CheckIsLogin 501	// ����Ƿ��¼
#define WM_HookReciveMsg 502	// hook��Ϣ����
#define WM_ReciveMsg 503	// ��Ϣ����


void sendWeChatMessage(WeChatMessage *msg);