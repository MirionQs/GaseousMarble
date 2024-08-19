#include <Windows.h>
#include "api.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		gm::api.init();
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}