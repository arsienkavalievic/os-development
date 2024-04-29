#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS_GLOBALS
#pragma warning(desable : 4996)
#pragma warning(suppress : 4996)
using namespace std;

int main() {
    cout << "Enter size of array: ";
    long size; cin >> size;
    cout << "Enter N and M: ";
    long n, m; 
    cin >> n >> m;
   
    HANDLE hWritePipe, hReadPipe;
    HANDLE hWritePipe2, hReadPipe2;

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create pipes
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        cout << "Create pipe failed" << endl;
        return GetLastError();
    }
    if (!CreatePipe(&hReadPipe2, &hWritePipe2, &saAttr, 0)) {
        cout << "Create pipe failed" << endl;
        return GetLastError();
    }
    
    HANDLE small_event;
    small_event = CreateEvent(NULL, FALSE, FALSE, "small_event");

    DWORD dwBytesWrite;
    // Write array data to the pipe

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char lpszComLine[80];
    wsprintf(lpszComLine, (LPCSTR)"%d %d", (int)hWritePipe2, (int)hReadPipe);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Create Consume process
    if (!CreateProcessA("F:\\BSU-FAMCs\\2-course\\OS\\big-brain-laba-5\\x64\\Release\\Client.exe", (LPSTR)lpszComLine,
        NULL, NULL, TRUE,
        CREATE_NEW_CONSOLE,
        NULL, NULL,
        (LPSTARTUPINFOA)&si, &pi))
    {
        cout << "Client.exe was not launched.\n";
        return GetLastError();
    }

    if (!WriteFile(hWritePipe, &size, sizeof(long), &dwBytesWrite, NULL)) {
        cout << "Write of length failed\n" << GetLastError();
        return GetLastError();
    }
        if (!WriteFile(hWritePipe, &n, sizeof(long), &dwBytesWrite, NULL)) {
            cout << "Write of array failed\n" << GetLastError();
            return GetLastError();
        }
        if (!WriteFile(hWritePipe, &m, sizeof(long), &dwBytesWrite, NULL)) {
            cout << "Write of array failed\n" << GetLastError();
            return GetLastError();
        }

        SetEvent(small_event);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD byteRwad;
    
    long* new_arr = new long[size];
    for (int i = 0; i < size; i++) {
        if (!ReadFile(hReadPipe2, &new_arr[i], sizeof(long), &byteRwad, NULL))
        {
            cout << "New array read is failed " << endl;
            return GetLastError();
        }
    }

    cout << "New array: " << endl;
    for (int i = 0; i < size; i++) {
        cout << new_arr[i] << " ";
    }

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Release the dynamically allocated memory
    delete[] new_arr;

    return 0;
}