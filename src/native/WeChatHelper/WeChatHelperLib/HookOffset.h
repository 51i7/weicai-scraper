#pragma once
//ƫ�Ƶ�ַ����

#if WECHAT_WINDLL_VERSION == 26852

#define offset_LoginSign 0x126D860		//��½��־λ

#define offset_ReciveMessage 0x315E98-5			//������Ϣ
#define offset_ReciveMessageParam 0x126D7F8		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x114 //��Ϣ�ķ�����

#define offset_QrCode 0x1F110B			//��ά��
#define offset_QrCodeContect 0x127F618	//��ά������

#define offset_GoToQrCode1 0x1EEE30			//��ת����ά��
#define offset_GoToQrCode2 0x2E3070			//��ת����ά��

#define offset_WxAntiRevoke 0x312565			//������

#elif WECHAT_WINDLL_VERSION == 27188

#define offset_LoginSign 0x13973B4	//��¼��־λ

#define offset_ReciveMessage 0x325373		//������Ϣ
#define offset_ReciveMessageParam 0x13971B8		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x114 //��Ϣ�ķ�����

#elif WECHAT_WINDLL_VERSION == 27276

#define offset_LoginSign 0x0	//��¼��־λ

#define offset_ReciveMessage 0x34DF18-5		//������Ϣ
#define offset_ReciveMessageParam 0x1624908		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x118 //��Ϣ�ķ�����


#elif WECHAT_WINDLL_VERSION == 27278

#define offset_LoginSign 0x0	//��¼��־λ

#define offset_ReciveMessage 0x34E238-5		//������Ϣ
#define offset_ReciveMessageParam 0x16268B0		//������Ϣ�Ĳ���
#define offset_ReciveMessageParam_MsgType 0x30 //��Ϣ����
#define offset_ReciveMessageParam_MsgSourceType 0x34 //��Ϣ��Դ����
#define offset_ReciveMessageParam_MsgReciver 0x40 //��Ϣ�Ľ�����
#define offset_ReciveMessageParam_MsgContent 0x68 //��Ϣ����
#define offset_ReciveMessageParam_MsgSender 0x118 //��Ϣ�ķ�����

#elif WECHAT_WINDLL_VERSION == 28088

#define offset_LoginSign 0x0	//��¼��־λ

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
#define offset_LoginInfoBlock_Province 0x538 //ʡ��
#define offset_LoginInfoBlock_City 0x550 //����
#define offset_LoginInfoBlock_Signer 0x568 //����ǩ��
#define offset_LoginInfoBlock_WechatName 0x5B0 //΢����
#define offset_LoginInfoBlock_Country 0x628 // ����
#define offset_LoginInfoBlock_Avatar 0x714 // ͷ��
#define offset_LoginInfoBlock_Device 0x888 // ��½�豸

#endif

#ifndef offset_QrCode
#define offset_QrCode 0x0			//��ά��
#endif
#ifndef offset_QrCodeContect
#define offset_QrCodeContect 0x0	//��ά������
#endif
#ifndef offset_GoToQrCode1
#define offset_GoToQrCode1 0x0			//��ת����ά��
#endif
#ifndef offset_GoToQrCode2
#define offset_GoToQrCode2 0x0			//��ת����ά��
#endif

#ifndef offset_WxAntiRevoke
#define offset_WxAntiRevoke 0x0
#endif
#ifndef offset_LoginInfoBlock
#define offset_LoginInfoBlock 0x0
#endif
#ifndef offset_LoginInfoBlock_WxId
#define offset_LoginInfoBlock_WxId 0x0
#endif
#ifndef offset_LoginInfoBlock_NickName
#define offset_LoginInfoBlock_NickName 0x0
#endif
#ifndef offset_LoginInfoBlock_Email
#define offset_LoginInfoBlock_Email 0x0
#endif
#ifndef offset_LoginInfoBlock_Mobile
#define offset_LoginInfoBlock_Mobile 0x0
#endif
#ifndef offset_LoginInfoBlock_Sex
#define offset_LoginInfoBlock_Sex 0x0
#endif
#ifndef offset_LoginInfoBlock_Province
#define offset_LoginInfoBlock_Province 0x0
#endif
#ifndef offset_LoginInfoBlock_City
#define offset_LoginInfoBlock_City 0x0
#endif
#ifndef offset_LoginInfoBlock_Signer
#define offset_LoginInfoBlock_Signer 0x0
#endif
#ifndef offset_LoginInfoBlock_WechatName
#define offset_LoginInfoBlock_WechatName 0x0
#endif
#ifndef offset_LoginInfoBlock_Country
#define offset_LoginInfoBlock_Country 0x0
#endif
#ifndef offset_LoginInfoBlock_Avatar
#define offset_LoginInfoBlock_Avatar 0x0
#endif
#ifndef offset_LoginInfoBlock_Device
#define offset_LoginInfoBlock_Device 0x0
#endif