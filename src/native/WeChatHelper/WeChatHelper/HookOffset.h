#pragma once
//ƫ�Ƶ�ַ����

// ����΢�Ű汾
#define WECHAT_VERSION 27188

#if WECHAT_VERSION == 26852

#define offset_LoginSign 0x126D860		//��½��־λ

#define offset_ReciveMessage 0x315E98-5			//������Ϣ
#define offset_ReciveMessageParam 0x126D7F8		//������Ϣ�Ĳ���

#define offset_QrCode 0x1F110B			//��ά��
#define offset_QrCodeContect 0x127F618	//��ά������

#define offset_GoToQrCode1 0x1EEE30			//��ת����ά��
#define offset_GoToQrCode2 0x2E3070			//��ת����ά��

#define offset_WxAntiRevoke 0x312565			//������

#elif WECHAT_VERSION == 27188

#define offset_LoginSign 0x13973B4	//��¼��־λ

#define offset_ReciveMessage 0x325373		//������Ϣ
#define offset_ReciveMessageParam 0x13971B8		//������Ϣ�Ĳ���

#define offset_QrCode 0x0			//��ά��
#define offset_QrCodeContect 0x0	//��ά������

#define offset_GoToQrCode1 0x0			//��ת����ά��
#define offset_GoToQrCode2 0x0			//��ת����ά��

#define offset_WxAntiRevoke 0x0			//������

#endif