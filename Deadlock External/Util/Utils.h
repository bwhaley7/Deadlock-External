#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>
#include <cmath>
#include "../SDK/SDK.h"

class Utils {
public:
	static uintptr_t FindSignature(HANDLE hProcess, uintptr_t start, uintptr_t size, const char* sig, const char* mask);
	static uintptr_t GetModuleBase(DWORD procID, const wchar_t* moduleName);
	static size_t GetModuleSize(DWORD procID, const wchar_t* moduleName);

	template<typename T>
	static T readMemory(HANDLE hProcess, uintptr_t address);
	template<typename T>
	static T readMemory(uintptr_t address);
};

extern HANDLE g_gameHandle;
extern uintptr_t g_moduleBase;

ULONG_PTR get_entity_list();
int get_max_entities();
ULONG_PTR get_base_entity_from_index(int index, ULONG_PTR entity_list = 0);
std::string get_schema_name(const ULONG_PTR& entity);
std::string get_designer_name(const ULONG_PTR& entity);
void print_player_info();
bool is_player_entity(ULONG_PTR entity);
void print_entities();
