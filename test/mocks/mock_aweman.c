#include <stdio.h>
#include <windows.h>
#include "../../include/sfloader/awe_dll.h"

#pragma aux AWEManager "*" 

static BOOL g_isAweOpen = FALSE;
static AWEHANDLE g_mockAweHandle = 0;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            printf("[MOCK AWEMAN] DLL attached. Mock AWEMAN32.DLL is active.\n");
            break;
        case DLL_PROCESS_DETACH:
            printf("[MOCK AWEMAN] DLL detached. Mock AWEMAN32.DLL is shutting down.\n");
            break;
    }
    return TRUE;
}

LRESULT WINAPI AWEManager(AWEHANDLE hUserID, UINT unMsg, LPARAM lParam1, LPARAM lParam2) {
    if (unMsg == AWE_OPEN) {
        if (g_isAweOpen) {
            printf("[MOCK AWEMAN] AWE_OPEN failed: device is already open.\n");
            return AWE_ERR_DEVICE_BUSY;
        }

        g_isAweOpen = TRUE;
        // Assign a non-zero value as our fake handle.
        g_mockAweHandle = 0xDEADBEEF;
        *(AWEHANDLE*)lParam1 = g_mockAweHandle;

        printf("[MOCK AWEMAN] AWE_OPEN successful. Handle 0x%lX created.\n", g_mockAweHandle);
        return AWE_NO_ERR;
    }

    if (!g_isAweOpen) {
        printf("[MOCK AWEMAN] ERROR: Call with message ID %u failed because AWE is not open.\n", unMsg);
        return AWE_ERR_ACCESS_NOT_PERMITTED;
    }

    if (hUserID != g_mockAweHandle) {
        printf("[MOCK AWEMAN] ERROR: Call with invalid handle (0x%lX). Expected 0x%lX.\n", hUserID, g_mockAweHandle);
        return AWE_ERR_USERID_INVALID;
    }

    switch(unMsg) {
        case AWE_CLOSE:
            g_isAweOpen = FALSE;
            g_mockAweHandle = 0;
            printf("[MOCK AWEMAN] AWE_CLOSE successful.\n");
            return AWE_NO_ERR;
        case AWE_QUERY_DRAM_SIZE: {
            DWORD dwMaxDRAM = 14336000;
            DWORD dwAvailableDRAM = dwMaxDRAM / 2;
            *(DWORD*)lParam1 = dwMaxDRAM;
            *(DWORD*)lParam2 = dwAvailableDRAM;
            printf("[MOCK AWEMAN] AWE_QUERY_DRAM_SIZE successful. Returning Max=%lu, Avail=%lu.\n", dwMaxDRAM, dwAvailableDRAM);
            return AWE_NO_ERR;
        }
        case AWE_SELECT_SYN_BANK:
            printf("[MOCK AWEMAN] AWE_SELECT_SYN_BANK successful for bank ID %ld.\n", (long)lParam1);
            return AWE_NO_ERR;
        default:
            printf("[MOCK AWEMAN] WARNING: Received unhandled message ID: %u\n", unMsg);
            return AWE_ERR_MSG_INVALID;
    }

    return AWE_ERR_UNDEFINE_ERROR;
}
