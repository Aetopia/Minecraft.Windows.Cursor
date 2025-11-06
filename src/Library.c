#define _MINAPPMODEL_H_
#include <minhook.h>
#include <appmodel.h>

PBYTE __wrap_memcpy(PBYTE Destination, PBYTE Source, SIZE_T Count)
{
    __movsb(Destination, Source, Count);
    return Destination;
}

PBYTE __wrap_memset(PBYTE Destination, BYTE Data, SIZE_T Count)
{
    __stosb(Destination, Data, Count);
    return Destination;
}

BOOL (*_ClipCursor)(LPRECT) = NULL;
INT (*_ShowCursor)(BOOL) = NULL;
BOOL (*_SetCursorPos)(INT, INT) = NULL;

BOOL $ClipCursor(LPRECT lpRect)
{
    if (lpRect && GetActiveWindow())
    {
        GetClientRect(GetActiveWindow(), lpRect);
        lpRect->left = (lpRect->right - lpRect->left) / 2;
        lpRect->top = (lpRect->bottom - lpRect->top) / 2;

        ClientToScreen(GetActiveWindow(), (LPPOINT)lpRect);
        lpRect->right = lpRect->left;
        lpRect->bottom = lpRect->top;
    }
    return _ClipCursor(lpRect);
}

INT $ShowCursor(BOOL bShow)
{
    if (bShow && GetActiveWindow())
        ClipCursor(&(RECT){});
    return _ShowCursor(bShow);
}

BOOL $SetCursorPos(INT X, INT Y)
{
    if (GetActiveWindow())
    {
        RECT lpRect = {};

        GetClientRect(GetActiveWindow(), &lpRect);
        lpRect.left = (lpRect.right - lpRect.left) / 2;
        lpRect.top = (lpRect.bottom - lpRect.top) / 2;

        ClientToScreen(GetActiveWindow(), (LPPOINT)&lpRect);
        X = lpRect.left;
        Y = lpRect.top;
    }
    return _SetCursorPos(X, Y);
}

BOOL DllMainCRTStartup(HINSTANCE hInstance, DWORD dwReason, PVOID pReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        WCHAR szString[APPLICATION_USER_MODEL_ID_MAX_LENGTH] = {};

        if (GetCurrentPackageFamilyName(&(UINT){ARRAYSIZE(szString)}, szString) ||
            CompareStringOrdinal(szString, 1, L"Microsoft.MinecraftUWP_8wekyb3d8bbwe", -1, TRUE) != CSTR_EQUAL)
            return FALSE;

        if (GetCurrentApplicationUserModelId(&(UINT){ARRAYSIZE(szString)}, szString) ||
            CompareStringOrdinal(szString, -1, L"Microsoft.MinecraftUWP_8wekyb3d8bbwe!Game", -1, TRUE) != CSTR_EQUAL)
            return FALSE;

        DisableThreadLibraryCalls(hInstance);
        MH_Initialize();

        MH_CreateHook(ClipCursor, &$ClipCursor, (PVOID)&_ClipCursor);
        MH_CreateHook(ShowCursor, &$ShowCursor, (PVOID)&_ShowCursor);
        MH_CreateHook(SetCursorPos, &$SetCursorPos, (PVOID)&_SetCursorPos);

        MH_QueueEnableHook(ClipCursor);
        MH_QueueEnableHook(ShowCursor);
        MH_QueueEnableHook(SetCursorPos);

        MH_ApplyQueued();
    }
    return TRUE;
}