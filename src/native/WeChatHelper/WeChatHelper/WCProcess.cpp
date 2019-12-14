#include "stdafx.h"
#include <tlhelp32.h>

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

/*
* �������Ƿ����ָ����dllģ��
*/
BOOL CheckProcessDllExists(DWORD dwProcessid, const wchar_t * DllName) {
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
		if (wcscmp(DllName, me32.szModule) == 0)
		{
			return true;
		}
		bModule = Module32Next(hModuleSnap, &me32);
	}

	return false;
}