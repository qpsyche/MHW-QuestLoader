// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <loader.h>
#include <hooks.h>

#include <set>

using namespace loader;

#define MonsterAddPartTimerAddress 0x1578993d0

template<typename T>
inline T* offsetPtr(void* ptr, int offset) { return (T*)(((char*)ptr) + offset); }

static void* offsetPtr(void* ptr, int offset) { return offsetPtr<void>(ptr, offset); }

HOOKFUNC(AddPartTimer, void*, void* timerMgr, unsigned int index, float timerStart)
{
	float* duration = offsetPtr<float>(timerMgr, 0x4a0);
	LOG(INFO) << "AddPArtTimer Increasing tenderize timer duration";
	*duration = 120;
	return originalAddPartTimer(timerMgr, index, timerStart);
}

void onLoad()
{

	LOG(INFO) << "LongerTenderize Loading...";
	if (std::string(GameVersion) != "402862") {
		LOG(ERR) << "Wrong version";
		return;
	}

    MH_Initialize();

	AddHook(AddPartTimer, MonsterAddPartTimerAddress);

	MH_ApplyQueued();

	LOG(INFO) << "DONE !";
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        onLoad();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

