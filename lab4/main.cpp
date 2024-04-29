#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main()
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, LPCWSTR("WriterMutex"));
    HANDLE hSemaphore = CreateSemaphore(NULL, 1, 1, LPCWSTR("ReaderSemaphore"));
    if (hMutex == NULL || hSemaphore == NULL) {
        return GetLastError();
    }
    HANDLE writerEvents[5];
    HANDLE readerEvents[3];
    for (int i = 0; i < 3; ++i) {
        writerEvents[i] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR(("WriterEvent" + to_string(i)).c_str()));
        readerEvents[i] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR(("ReaderEvent" + to_string(i)).c_str()));
        if (writerEvents[i] == NULL || readerEvents[i] == NULL) {
            return GetLastError();
        }
    }
    writerEvents[3] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("WriterEndEvent"));
    writerEvents[4] = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("ReaderEndEvent"));
    int processNum, messageNum;
    cout << "Enter number of Writer and Reader processes: ";
    cin >> processNum;
    cout << "Enter number of messages: ";
    cin >> messageNum;
    STARTUPINFO si;
    PROCESS_INFORMATION* writer_pi = new PROCESS_INFORMATION[processNum];
    PROCESS_INFORMATION* reader_pi = new PROCESS_INFORMATION[processNum];
    string writerInfo = to_string(messageNum);
    string readerInfo;

    for (int i = 0; i < processNum; ++i) {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess(LPCWSTR("Debug\\Writer.exe"), (LPWSTR)(char*)writerInfo.c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &writer_pi[i])) {
            cout << "\nwriter process is not created\n";
            return GetLastError();
        }
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        if (!CreateProcess(LPCWSTR("Debug\\Reader.exe.recipe"), (LPWSTR)(char*)writerInfo.c_str(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &reader_pi[i])) {
            cout << "\nreader process is not created\n";
            return GetLastError();
        }
    }

    int writerNum = processNum;
    int readerNum = processNum;
    while (writerNum || readerNum) {
        DWORD message = WaitForMultipleObjects(5, writerEvents, FALSE, INFINITE);
        switch (message) {
        case 0:
            cout << "Recieved message: A\n";
            PulseEvent(readerEvents[0]);
            break;
        case 1:
            cout << "Recieved message: B\n";
            PulseEvent(readerEvents[1]);
            break;
        case 2:
            std::cout << "Writer Process ended\n";
            --writerNum;
            break;
        case 3:
            std::cout << "Reader Process ended\n";
            --readerNum;
            break;

        }
    }


    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
    for (int i = 0; i < 3; ++i) {
        CloseHandle(writerEvents[i]);
        CloseHandle(readerEvents[i]);
    }
    CloseHandle(writerEvents[3]);
    CloseHandle(writerEvents[4]);
    for (int i = 0; i < processNum; ++i) {
        CloseHandle(writer_pi[i].hThread);
        CloseHandle(writer_pi[i].hProcess);
        CloseHandle(reader_pi[i].hThread);
        CloseHandle(reader_pi[i].hProcess);
    }
    return 0;
}
