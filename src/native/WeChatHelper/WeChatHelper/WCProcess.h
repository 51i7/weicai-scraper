#pragma once
#include <atlbase.h>  

/*
* ���ݽ������ƻ�ý���ID
*/
DWORD FindProcessPidByName(LPCSTR ProcessName);

/*
* �������Ƿ����ָ����dllģ��
*/
BOOL CheckProcessDllExists(DWORD dwProcessid, LPCSTR DllName);