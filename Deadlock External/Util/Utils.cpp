#include "Utils.h"
#include <iostream>
#include <unordered_set>
#include <string>


HANDLE g_gameHandle = nullptr;
uintptr_t g_moduleBase = 0;

uintptr_t Utils::FindSignature(HANDLE hProcess, uintptr_t start, uintptr_t size, const char* signature, const char* mask) {
    std::vector<BYTE> buffer(size);
    SIZE_T bytesRead;
    ReadProcessMemory(hProcess, (LPCVOID)start, buffer.data(), size, &bytesRead);

    for (uintptr_t i = 0; i < size - strlen(mask); i++) {
        bool found = true;
        for (uintptr_t j = 0; j < strlen(mask); j++) {
            if (mask[j] != '?' && buffer[i + j] != (BYTE)signature[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            uintptr_t relativeOffset = *(int*)(buffer.data() + i + 5);  // Adjust if needed based on where the offset is located
            return start + i + relativeOffset + 9;  // Adjust to point to the final address of qword_7FF946932628
        }
    }
    return 0;
}

bool is_player_entity(ULONG_PTR entity) {
    std::string class_name = get_schema_name(entity);
    return class_name == "C_CitadelPlayerPawn";
}

uintptr_t Utils::GetModuleBase(DWORD processID, const wchar_t* moduleName) {
    uintptr_t moduleBaseAddress = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(snapshot, &moduleEntry)) {
            do {
                if (!_wcsicmp(moduleEntry.szModule, moduleName)) {
                    moduleBaseAddress = (uintptr_t)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(snapshot, &moduleEntry));
        }
        CloseHandle(snapshot);
    }
    return moduleBaseAddress;
}

size_t Utils::GetModuleSize(DWORD processID, const wchar_t* moduleName) {
    size_t moduleSize = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(snapshot, &moduleEntry)) {
            do {
                if (!_wcsicmp(moduleEntry.szModule, moduleName)) {
                    moduleSize = moduleEntry.modBaseSize;
                    break;
                }
            } while (Module32Next(snapshot, &moduleEntry));
        }
        CloseHandle(snapshot);
    }
    return moduleSize;
}

template<typename T>
T Utils::readMemory(HANDLE hProcess, uintptr_t address) {
	T buffer;
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), nullptr);
	return buffer;
}

template<typename T>
T Utils::readMemory(uintptr_t address) {
    T buffer;
    ReadProcessMemory(g_gameHandle, (LPCVOID)address, &buffer, sizeof(T), nullptr);
    return buffer;
}

// Global signature variables
const char* ENTITY_LIST_SIGNATURE = "\x74\x3E\x48\x8B\x0D\x00\x00\x00\x00\xB2\x01";
const char* ENTITY_LIST_MASK = "xxxxx????xx";

uintptr_t GetEntityListBase(HANDLE hProcess) {
    DWORD processID = GetProcessId(hProcess);
    uintptr_t clientBaseAddress = Utils::GetModuleBase(processID, L"client.dll");
    uintptr_t moduleSize = Utils::GetModuleSize(processID, L"client.dll");

    uintptr_t instructionAddress = Utils::FindSignature(hProcess, clientBaseAddress, moduleSize, ENTITY_LIST_SIGNATURE, ENTITY_LIST_MASK);

    if (instructionAddress == 0) {
        std::cerr << "Failed to find the instruction address!" << std::endl;
        return 0;
    }

    int32_t relativeOffset = Utils::readMemory<int32_t>(hProcess, instructionAddress + 3);

    uintptr_t entityListPointer = instructionAddress + 7 + relativeOffset;

    return entityListPointer;
}

uintptr_t GetEntityAtIndex(HANDLE hProcess, uintptr_t entityListBase, int index) {
    return Utils::readMemory<uintptr_t>(hProcess, entityListBase + (index * sizeof(uintptr_t)));
}

uintptr_t GetEntityListPointer(DWORD pid, HANDLE hProcess) {
    uintptr_t clientBase = Utils::GetModuleBase(pid, L"client.dll");
    if (!clientBase) {
        std::cerr << "Failed to get client.dll base address!" << std::endl;
        return 0;
    }

    uintptr_t entityListPointerAddress = clientBase + GameOffsets::dwEntityList;

    std::cout << "Entity List Pointer Address: 0x" << std::hex << entityListPointerAddress << std::endl;

    uintptr_t entityListPointer = Utils::readMemory<uintptr_t>(hProcess, entityListPointerAddress);
    if (!entityListPointer) {
        std::cerr << "Failed to read entity list pointer!" << std::endl;
        return 0;
    }

    return entityListPointer;
}

int GetMaxEntities(HANDLE hProcess, uintptr_t entityListBase) {
    DWORD maxEntities;
    ReadProcessMemory(hProcess, (LPCVOID)(entityListBase + 5408), &maxEntities, sizeof(DWORD), nullptr);
    return static_cast<int>(maxEntities);
}

void PrintEntityInfo(HANDLE hProcess, uintptr_t entityBase) {
    std::cout << "Entity Pointer: 0x" << std::hex << entityBase << std::endl;
}

std::string read_str(uintptr_t address) {
    char buffer[256]; // Adjust the buffer size as necessary
    SIZE_T bytesRead;

    // Ensure g_gameHandle is valid before attempting to read memory
    if (g_gameHandle && ReadProcessMemory(g_gameHandle, (LPCVOID)address, buffer, sizeof(buffer) - 1, &bytesRead)) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        return std::string(buffer);
    }
    else {
        return std::string();
    }
}

ULONG_PTR get_entity_list() {
    uintptr_t tf = Utils::readMemory<uintptr_t>(g_gameHandle, g_moduleBase + GameOffsets::dwEntityList);
    return static_cast<ULONG_PTR>(tf);
}

int get_max_entities() {
    ULONG_PTR entity_list = get_entity_list();
    int max_entities = Utils::readMemory<int>(g_gameHandle, entity_list + GameOffsets::dwGameEntitySystem_highestEntityIndex);
    return max_entities;
}

ULONG_PTR get_base_entity_from_index(int index, ULONG_PTR entity_list) {
    if (entity_list == 0) {
        entity_list = get_entity_list();
    }

    ULONG_PTR entity_base = entity_list + 8LL * ((index & 0x7FFF) >> 9) + 16;
    ULONG_PTR entity = Utils::readMemory<uint64_t>(entity_base);
    if (!entity) {
        return 0;
    }

    entity += 120LL * (index & 0x1FF);
    entity = Utils::readMemory<ULONG_PTR>(entity);
    return entity;
}

std::string get_schema_name(const ULONG_PTR& entity) {
    ULONG_PTR entity_identity = Utils::readMemory<ULONG_PTR>(entity + GameSDK::CEntityInstance::m_pEntity);
    if (!entity_identity) {
        return "";
    }

    ULONG_PTR entity_class_info = Utils::readMemory<ULONG_PTR>(entity_identity + 0x8);
    ULONG_PTR schema_class_info_data = Utils::readMemory<ULONG_PTR>(entity_class_info + 0x28);
    ULONG_PTR class_name = Utils::readMemory<ULONG_PTR>(schema_class_info_data + 0x8);

    return read_str(class_name); // Assume read_str is a utility function that reads a string from memory
}

std::string get_designer_name(const ULONG_PTR& entity) {
    ULONG_PTR entity_identity = Utils::readMemory<ULONG_PTR>(entity + GameSDK::CEntityInstance::m_pEntity);
    if (!entity_identity) {
        return "";
    }

    ULONG_PTR designer_name = Utils::readMemory<ULONG_PTR>(entity_identity + GameSDK::CEntityIdentity::m_designerName);
    return read_str(designer_name);
}

std::string get_name(const ULONG_PTR& entity) {
    ULONG_PTR entity_identity = Utils::readMemory<ULONG_PTR>(entity + GameSDK::CEntityInstance::m_pEntity);
    if (!entity_identity) {
        return "";
    }

    ULONG_PTR name = Utils::readMemory<ULONG_PTR>(entity_identity + GameSDK::CEntityIdentity::m_name);
    return read_str(name);
}

Vector get_player_pos(const ULONG_PTR& entity) {
    ULONG_PTR entity_cGameNode = Utils::readMemory<ULONG_PTR>(entity + GameSDK::C_BaseEntity::m_pGameSceneNode);
    Vector absPos = Utils::readMemory<Vector>(entity_cGameNode + GameSDK::CGameSceneNode::m_vecAbsOrigin);

    return absPos;
}

void print_player_info()
{
    std::unordered_set<ULONG_PTR> processed_ents;
    const int max_ents = 5000;

    for (size_t i = 1; i <= static_cast<size_t>(max_ents); ++i) {
        auto entity = get_base_entity_from_index(i);

        if (!entity || !processed_ents.insert(entity).second)
            continue;
        
        if (is_player_entity(entity)) {
            int player_health = Utils::readMemory<int32_t>(entity + GameSDK::C_BaseEntity::m_iHealth);

            Vector position = get_player_pos(entity);
            std::string name = get_name(entity);
            printf("%s Health: %d | Position: (%f, %f, %f)\n",
                name.c_str(),
                player_health,
                position.x,
                position.y,
                position.z);
        }
    }
    system("CLS");
}

void print_entities() {
    std::unordered_set<ULONG_PTR> processed_ents;
    int max_ents = get_max_entities();

    for (size_t i = 1; i <= static_cast<size_t>(max_ents); ++i) {
        ULONG_PTR entity = get_base_entity_from_index(i);

        if (!entity || !processed_ents.insert(entity).second) {
            continue;
        }

        printf("[>] %s | %s -> %p\n", get_schema_name(entity).c_str(), get_designer_name(entity).c_str(), reinterpret_cast<void*>(entity));
    }
}