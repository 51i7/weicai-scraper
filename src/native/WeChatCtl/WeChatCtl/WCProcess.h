#pragma once

/*
* ���ݽ������ƻ�ý���ID
*/
DWORD FindProcessPidByName(const wchar_t *ProcessName);

/*
* �������Ƿ����ָ����dllģ��
*/
BOOL CheckProcessDllExists(DWORD dwProcessid, LPCSTR DllName);
BOOL ProcessDllInject(DWORD dwProcessid, LPCSTR DllPath, LPCSTR DllName);
DWORD GetWechatInstalledPath(WCHAR * InstallLocation);
bool closeAllProcess(const wchar_t *ProcessName);
bool closeProcess(DWORD ProcessID);