#pragma once
#include <string>

// ���ڼ�ͨ��Э��

#define WM_ShowQrCode 1		// ȡ��΢�ŵ�½��ά��
#define WM_CheckIsLogin 501	// ����Ƿ��¼
#define WM_HookReciveMsg 502	// hook��Ϣ����
#define WM_ReciveMsg 503	// ��Ϣ����

void AntiRevoke();
// ��ʾ��½��ά��
void WX_CallShowQrCode();
// ж��WX_SaveQrCode
void WX_SaveQrCode_Unhook(DWORD dwHookOffset);
void invokeSaveImg();
void SaveImg(DWORD qrcode);
// ����Ƿ��¼
void CheckIsLogin();


// �����½��ά��
void HOOK_SaveQrCode();

void HOOK_ReciveMsg();
void RecieveWxMesage();
void SendWxMessage();



//��Ϣ�ṹ��
struct Message
{
	DWORD type;		//��Ϣ����
	wchar_t typeStr[80];		//��Ϣ����
	DWORD sourceType;		//��Ϣ��Դ
	wchar_t sourceTypeStr[80];		//��Ϣ��Դ
	wchar_t msgReciver[40];		//΢��ID/ȺID
	wchar_t msgSender[40];	//��Ϣ������
	wchar_t content[1];	//��Ϣ����
};

std::wstring GetMsgByAddress(DWORD memAddress);
bool startWith(const std::string &str, const std::string &head);
bool endWith(const std::string &str, const std::string &tail);
