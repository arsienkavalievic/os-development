#include <iostream>
#include <windows.h>
#include <conio.h>
#include <random>
#include <time.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS_GLOBALS
#pragma warning(desable : 4996)
#pragma warning(suppress : 4996)
using namespace std;

int main(int argc, char* argv[]) {

    HANDLE small_event;
    small_event = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)"small_event");
    WaitForSingleObject(small_event, INFINITY);
    /*HANDLE hReadPipe, hWritePipe;
    sscanf(argv[0], "%d", &hWritePipe);
    sscanf(argv[1], "%d", &hReadPipe);*/
    HANDLE hWritePipe = (HANDLE)atoi(argv[0]), hReadPipe = (HANDLE)atoi(argv[1]);

    
    LPDWORD dwBytesWrite = 0, dwBytesRead = 0;
    int arl;

    if (!ReadFile(hReadPipe, &arl, sizeof(long), dwBytesRead, NULL)) {
        cout << "size  read is failed " << endl;char e = _getch();
        return GetLastError();
    }

    int n, m;
        if (!ReadFile(hReadPipe, &n, sizeof(long), dwBytesRead, NULL))
        {
            cout << "n read is failed " << endl;
            return GetLastError();
        }
        if (!ReadFile(hReadPipe, &m, sizeof(long), dwBytesRead, NULL))
        {
            cout << "m read is failed " << endl;
            return GetLastError();
        }

        

    long* mas = new long[arl];
    srand(time(NULL));
    for (int i = 0; i < arl; i++) {
        mas[i] = long(rand() % (m - n + 1) + n);
    }

    // Create pipes
    for (int i = 0; i < arl; i++) {
        if (!WriteFile(hWritePipe, &mas[i], sizeof(long), dwBytesWrite, NULL)) {
            cout << "Write of array failed\n";
            return GetLastError();
        }
    }

    _cprintf("\nTo exit press any key ");
    char t = _getch();


    return 0;
}