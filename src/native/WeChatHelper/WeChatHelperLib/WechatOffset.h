#pragma once

class WechatOffset
{
public:
	WechatOffset();
	~WechatOffset();
	DWORD offsetReciveMessage; //������Ϣ
	DWORD offsetReciveMessageParam; //������Ϣ�Ĳ���
	DWORD offsetReciveMessageParam_MsgType; //��Ϣ����
	DWORD offsetReciveMessageParam_MsgSourceType; //��Ϣ��Դ����
	DWORD offsetReciveMessageParam_MsgReciver;  //��Ϣ�Ľ�����
	DWORD offsetReciveMessageParam_MsgContent; //��Ϣ����
	DWORD offsetReciveMessageParam_MsgSender; //��Ϣ�ķ�����

	DWORD offsetLoginInfoBlock; //��¼�û���Ϣ����ʼ
	DWORD offsetLoginInfoBlock_WxId; // ΢��ID
	DWORD offsetLoginInfoBlock_NickName; //�ǳ�
	DWORD offsetLoginInfoBlock_Email; //����
	DWORD offsetLoginInfoBlock_Mobile; //�ֻ�����
	DWORD offsetLoginInfoBlock_Sex; //�Ա�
	DWORD offsetLoginInfoBlock_IsLogin; //��¼��־λ
	DWORD offsetLoginInfoBlock_Province; //ʡ��
	DWORD offsetLoginInfoBlock_City; //����
	DWORD offsetLoginInfoBlock_Signer; //����ǩ��
	DWORD offsetLoginInfoBlock_WechatName; //΢����
	DWORD offsetLoginInfoBlock_Country; // ����
	DWORD offsetLoginInfoBlock_Avatar; // ͷ��
	DWORD offsetLoginInfoBlock_Device; // ��½�豸

	DWORD offsetAntiRevoke; //��Ϣ����hook

	DWORD offsetGetFriendListCall; //�����б�
	DWORD offsetGetFriendList; //�����б�
};
