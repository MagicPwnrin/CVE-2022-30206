#include <windows.h>
#include <string> 
#include <atlstr.h>
int main()
{
    HANDLE hPrinter;
    BOOL bRes;
    PRINTER_DEFAULTS printerDefaults;
    DWORD dwJobId;
    DOC_INFO_1 docInfo;
    LPWSTR g_PrinterName = L"PWN";
    // OpenPrinter
    printerDefaults.pDatatype = NULL;
    printerDefaults.pDevMode = NULL;
    printerDefaults.DesiredAccess = PRINTER_ALL_ACCESS;
    bRes = OpenPrinter(g_PrinterName, &hPrinter, &printerDefaults);
    DWORD lFileSize = 2;
    BYTE* hDllBuffer = (BYTE*)malloc(lFileSize);
    //Writing to the printer
    DWORD dwNeeded;
    docInfo.pDatatype = L"RAW";
    docInfo.pOutputFile = NULL;
    docInfo.pDocName = L"NorthSea";
    dwJobId = StartDocPrinter(hPrinter, 1, (LPBYTE)&docInfo);
    return 0;
}
    