#include "stdafx.h"
#include <tlhelp32.h>
#include "StringTool.h"
#include "LogRecord.h"

/*
* ���ݽ������ƻ�ý���ID
*/
DWORD FindProcessPidByName(const wchar_t *ProcessName)
{
	//PROCESSENTRY32�ṹ�壬������̾�����Ϣ
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(pe32);

	//���ϵͳ���̿��յľ��
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	//���Ȼ�õ�һ������
	BOOL bProcess = Process32First(hProcessSnap, &pe32);
	//ѭ��������н���
	while (bProcess)
	{
		if (wcscmp(ProcessName, pe32.szExeFile) == 0)
		{
			return pe32.th32ProcessID;
		}
		bProcess = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
	return 0;
}

DWORD GetWechatInstalledPath(WCHAR * InstallLocation) {

	HKEY hKey;
	LONG lRet = 0;

	DWORD dwBuflen = MAX_PATH;

	lRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WeChat", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_EXECUTE | KEY_WOW64_64KEY, NULL, &hKey, NULL);

	//���濪ʼ��ѯ
	lRet = RegQueryValueEx(hKey, //��ע���ʱ���صľ��
		TEXT("InstallLocation"), //Ҫ��ѯ������,��ѯ�������װĿ¼������
		NULL, //һ��ΪNULL����0
		NULL,
		(LPBYTE)InstallLocation, //����Ҫ�Ķ�����������
		&dwBuflen);

	RegCloseKey(hKey);

	return lRet;
}

/*
* �������Ƿ����ָ����dllģ��
*/
BOOL CheckProcessDllExists(DWORD dwProcessid, LPCSTR DllName) {
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	//��ʼ��ģ����Ϣ�ṹ��
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	//����ģ����� 1 �������� 2 ����ID
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessid);
	//��������Ч�ͷ���false
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	//ͨ��ģ����վ����ȡ��һ��ģ�����Ϣ
	if (!Module32First(hModuleSnap, &me32))
	{
		//��ȡʧ����رվ��
		CloseHandle(hModuleSnap);
		return false;
	}

	//���Ȼ�õ�һ��
	BOOL bModule = Module32First(hModuleSnap, &me32);
	//ѭ���������
	while (bModule)
	{
		if (strcmp(DllName,Wchar_tToString(me32.szModule).c_str()) == 0)
		{
			return true;
		}
		bModule = Module32Next(hModuleSnap, &me32);
	}

	return false;
}


/*
* ����Dllע��
*/
BOOL ProcessDllInject(DWORD dwProcessid, LPCSTR DllPath, LPCSTR DllName) {
	if (dwProcessid == 0)
	{
		return false;
	}
	if (CheckProcessDllExists(dwProcessid, DllName))
	{
		return false;
	}

	char szPath[MAX_PATH] = { 0 };

	//�򿪽���
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessid);
	if (hProcess == NULL)
	{
		return false;
	}

	//��΢�Ž����������ڴ�
	LPVOID pAddress = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pAddress == NULL)
	{
		return false;
	}

	//д��dll·����΢�Ž���
	LogRecord(L"д��dll·����΢�Ž���", ofs);
	if (WriteProcessMemory(hProcess, pAddress, szPath, MAX_PATH, NULL) == 0)
	{
		return false;
	}

	//��ȡLoadLibraryA������ַ
	LogRecord(L"��ȡLoadLibraryA������ַ", ofs);
	FARPROC pLoadLibraryAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (pLoadLibraryAddress == NULL)
	{
		return false;
	}
	//Զ���߳�ע��dll
	LogRecord(L"Զ���߳�ע��dl", ofs);
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryAddress, pAddress, 0, NULL);
	if (hRemoteThread == NULL)
	{
		return false;
	}

	/*WaitForSingleObject(hRemoteThread, -1);
	VirtualFreeEx(hProcess, pAddress, 1, MEM_DECOMMIT);*/
	CloseHandle(hRemoteThread);
	CloseHandle(hProcess);

	return true;
}

bool closeAllProcess(const wchar_t *ProcessName)
{
	char debugInfo[0x1000] = { 0 };
	HANDLE ProcesssAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 proessInfo = { 0 };
	proessInfo.dwSize = sizeof(PROCESSENTRY32);
	do
	{
		if (wcscmp(ProcessName, proessInfo.szExeFile) == 0) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, proessInfo.th32ProcessID);
			if (hProcess != NULL) {
				TerminateProcess(hProcess, 0);
			}
		}
	} while (Process32Next(ProcesssAll, &proessInfo));

	return true;
}

bool closeProcess(DWORD ProcessID)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
	if (hProcess != NULL) {
		TerminateProcess(hProcess, 0);
	}
	return true;
}