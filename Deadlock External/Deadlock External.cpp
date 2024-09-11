
#include <iostream>
#include <Windows.h>
#include "Util/Utils.h"

int main()
{
	DWORD pid = 0;
	HWND hwnd = FindWindow(NULL, L"Deadlock");

	if (hwnd == NULL) {
		std::cerr << "Game window not found" << std::endl;
		return 1;
	}

	GetWindowThreadProcessId(hwnd, &pid);
	g_gameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (g_gameHandle == NULL) {
		std::cerr << "Failed to open process" << std::endl;
		return 1;
	}

	g_moduleBase = Utils::GetModuleBase(pid, L"client.dll");

	//print_entities();
	while (true) {
		print_player_info();
	}
	//99% sure that print_player_info doesn't work because I am trying to read it from CCitadelPlayerController instead of CCitadelPlayerPawn. CCitadelPlayerPawn inherits from C_BasePlayer. Add CCitadelPlayerPawn and all of its inherited classes to the SDK and then try again.

	CloseHandle(g_gameHandle);
	return 0;
}

