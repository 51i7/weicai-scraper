#pragma once

/*
* ���ݽ������ƻ�ý���ID
*/
DWORD FindProcessPidByName(const wchar_t *ProcessName);

/*
* �������Ƿ����ָ����dllģ��
*/
BOOL CheckProcessDllExists(DWORD dwProcessid, LPCSTR DllName);