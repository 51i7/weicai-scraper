#pragma once
#include <string>
#include <vector>


//消息结构体
struct WeChatMessage
{
	DWORD type;		//消息类型
	wchar_t typeStr[80];		//消息类型
	DWORD sourceType;		//消息来源
	wchar_t sourceTypeStr[80];		//消息来源
	wchar_t msgReciver[40];		//微信ID/群ID
	wchar_t msgSender[40];	//消息发送者
	wchar_t content[1];	//消息内容
};

// hook状态
struct WeChatHookPoint
{
	//====接收消息HOOK====
	bool enable_WX_ReciveMsg_Hook = false;
	CHAR WX_ReciveMsg_Hook[5] = { 0 };

	//====读取信息HOOK====
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
	wchar_t wxid[120]; // 微信ID
	wchar_t wxname[120]; // 微信号
	wchar_t wxv1[120]; //v1_
	wchar_t realname[120]; // 姓名
	wchar_t nickname[120]; // 昵称
	DWORD sex; // 性别
	wchar_t f_nickname[80]; // 昵称首字母
	wchar_t p_nickname[200]; // 昵称拼音
	wchar_t f_realname[80]; // 姓名首字母
	wchar_t p_realname[200]; // 姓名拼音
	DWORD type; // 类型
	DWORD unflag; 
};

extern WeChatHookPoint *sWeChatHookPoint;
extern WeChatHookReg *sWeChatHookReg;
extern WeChatLoginInfo *sWeChatLoginInfo;
extern std::vector<std::wstring> vUserList;

//====消息状态类消息表示====

//====动作请求类消息表示====
#define WM_HookReciveMsg 502	// hook消息接收
#define WM_HookAntiRevoke 504 // hook消息撤回
#define WM_ShowQrCode 1		// 取得微信登陆二维码
#define WM_CheckIsLogin 501	// 检查是否登录
#define WM_ReciveMsg 503	// 消息接收
#define WM_RegWeChatHelper 505 // WeChatHelper注册
#define WM_UnRegWeChatHelper 506 // WeChatHelper注销
#define WM_OpenWeChat 507 //多开微信
#define WM_GetFriendList 508	// 好友列表接收

int IsLogin();
WeChatLoginInfo * GetWechatLoginInfo();

//====接收消息HOOK====
void HOOK_ReciveMsg();
void UnHOOK_ReciveMsg();
void RecieveWxMesage();
void SendWxMessage();

void HOOK_AntiRevoke();

void HOOK_GetItemInfo();
void GetItemInfo();
void SendItemInfo();
void UnHOOK_GetItemInfo();