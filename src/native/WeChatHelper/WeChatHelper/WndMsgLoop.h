#pragma once

void InitWindow(HMODULE hModule);
void RegisterWindow(HMODULE hModule);
void CheckIsLogin();
void GotoQrCode();
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void HookChatRecord();		//HOOK�����¼
void RecieveWxMesage();		//����΢����Ϣ
void SendWxMessage();		//����΢����Ϣ���ͻ���