#pragma once
//ƫ�Ƶ�ַ����

#if WECHAT_WINDLL_VERSION == 26852

#define offset_ReciveMessage 0x315E98-5			//������Ϣ
#define offset_ReciveMessageParam 0x126D7F8		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x114 //��Ϣ�ķ�����

#define offset_LoginInfoBlock 0x0 //��¼�û���Ϣ����ʼ
#define offset_LoginInfoBlock_WxId 0x0 // ΢��ID
#define offset_LoginInfoBlock_NickName 0x0 //�ǳ�
#define offset_LoginInfoBlock_Email 0x0 //����
#define offset_LoginInfoBlock_Mobile 0x0 //�ֻ�����
#define offset_LoginInfoBlock_Sex 0x0 //�Ա�
#define offset_LoginInfoBlock_IsLogin 0x0 //��¼��־λ
#define offset_LoginInfoBlock_Province 0x0 //ʡ��
#define offset_LoginInfoBlock_City 0x0 //����
#define offset_LoginInfoBlock_Signer 0x0 //����ǩ��
#define offset_LoginInfoBlock_WechatName 0x0 //΢����
#define offset_LoginInfoBlock_Country 0x0 // ����
#define offset_LoginInfoBlock_Avatar 0x0 // ͷ��
#define offset_LoginInfoBlock_Device 0x0 // ��½�豸

#elif WECHAT_WINDLL_VERSION == 27188

#define offset_ReciveMessage 0x325373		//������Ϣ
#define offset_ReciveMessageParam 0x13971B8		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x114 //��Ϣ�ķ�����

#define offset_LoginInfoBlock 0x0 //��¼�û���Ϣ����ʼ
#define offset_LoginInfoBlock_WxId 0x0 // ΢��ID
#define offset_LoginInfoBlock_NickName 0x0 //�ǳ�
#define offset_LoginInfoBlock_Email 0x0 //����
#define offset_LoginInfoBlock_Mobile 0x0 //�ֻ�����
#define offset_LoginInfoBlock_Sex 0x0 //�Ա�
#define offset_LoginInfoBlock_IsLogin 0x0 //��¼��־λ
#define offset_LoginInfoBlock_Province 0x0 //ʡ��
#define offset_LoginInfoBlock_City 0x0 //����
#define offset_LoginInfoBlock_Signer 0x0 //����ǩ��
#define offset_LoginInfoBlock_WechatName 0x0 //΢����
#define offset_LoginInfoBlock_Country 0x0 // ����
#define offset_LoginInfoBlock_Avatar 0x0 // ͷ��
#define offset_LoginInfoBlock_Device 0x0 // ��½�豸

#elif WECHAT_WINDLL_VERSION == 27276

#define offset_ReciveMessage 0x34DF18-5		//������Ϣ
#define offset_ReciveMessageParam 0x1624908		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x118 //��Ϣ�ķ�����

#define offset_LoginInfoBlock 0x0 //��¼�û���Ϣ����ʼ
#define offset_LoginInfoBlock_WxId 0x0 // ΢��ID
#define offset_LoginInfoBlock_NickName 0x0 //�ǳ�
#define offset_LoginInfoBlock_Email 0x0 //����
#define offset_LoginInfoBlock_Mobile 0x0 //�ֻ�����
#define offset_LoginInfoBlock_Sex 0x0 //�Ա�
#define offset_LoginInfoBlock_IsLogin 0x0 //��¼��־λ
#define offset_LoginInfoBlock_Province 0x0 //ʡ��
#define offset_LoginInfoBlock_City 0x0 //����
#define offset_LoginInfoBlock_Signer 0x0 //����ǩ��
#define offset_LoginInfoBlock_WechatName 0x0 //΢����
#define offset_LoginInfoBlock_Country 0x0 // ����
#define offset_LoginInfoBlock_Avatar 0x0 // ͷ��
#define offset_LoginInfoBlock_Device 0x0 // ��½�豸


#elif WECHAT_WINDLL_VERSION == 27278

#define offset_ReciveMessage 0x34E238-5		//������Ϣ
#define offset_ReciveMessageParam 0x16268B0		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x118 //��Ϣ�ķ�����

#define offset_LoginInfoBlock 0x1396E38 //��¼�û���Ϣ����ʼ
#define offset_LoginInfoBlock_WxId 0x42C // ΢��ID
#define offset_LoginInfoBlock_NickName 0x4A4 //�ǳ�
#define offset_LoginInfoBlock_Email 0x4C0 //����
#define offset_LoginInfoBlock_Mobile 0x4D8 //�ֻ�����
#define offset_LoginInfoBlock_Sex 0x574 //�Ա�
#define offset_LoginInfoBlock_IsLogin 0x57C //��¼��־λ
#define offset_LoginInfoBlock_Province 0x590 //ʡ��
#define offset_LoginInfoBlock_City 0x5A8 //����
#define offset_LoginInfoBlock_Signer 0x5c0 //����ǩ��
#define offset_LoginInfoBlock_WechatName 0x608 //΢����
#define offset_LoginInfoBlock_Country 0x680 // ����
#define offset_LoginInfoBlock_Avatar 0x76C // ͷ��
#define offset_LoginInfoBlock_Device 0x8E0 // ��½�豸

#elif WECHAT_WINDLL_VERSION == 28088

#define offset_ReciveMessage 0x350878-5		//������Ϣ
#define offset_ReciveMessageParam 0x16288F8		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x118 //��Ϣ�ķ�����

#define offset_LoginInfoBlock 0x1611300 //��¼�û���Ϣ����ʼ
#define offset_LoginInfoBlock_WxId 0x3D4 // ΢��ID
#define offset_LoginInfoBlock_NickName 0x44C //�ǳ�
#define offset_LoginInfoBlock_Email 0x468 //����
#define offset_LoginInfoBlock_Mobile 0x480 //�ֻ�����
#define offset_LoginInfoBlock_Sex 0x534 //�Ա�
#define offset_LoginInfoBlock_IsLogin 0x53C //��¼��־λ
#define offset_LoginInfoBlock_Province 0x538 //ʡ��
#define offset_LoginInfoBlock_City 0x550 //����
#define offset_LoginInfoBlock_Signer 0x568 //����ǩ��
#define offset_LoginInfoBlock_WechatName 0x5B0 //΢����
#define offset_LoginInfoBlock_Country 0x628 // ����
#define offset_LoginInfoBlock_Avatar 0x714 // ͷ��
#define offset_LoginInfoBlock_Device 0x888 // ��½�豸

#endif