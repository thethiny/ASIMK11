#include <windows.h>
#include <iostream>
using namespace std;

#define FGPIPE TEXT("\\\\.\\pipe\\MK11Unlocker")

int main()
{

    HANDLE hPipe;
    DWORD dwWritten;
    DWORD MAX_BUF_SIZE = 8;

    hPipe = CreateNamedPipe(FGPIPE,
                            PIPE_ACCESS_OUTBOUND,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            2,
                            MAX_BUF_SIZE*16,
                            0,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL
                            );

    cout<<"Pipe? "<<hPipe<<endl;
    cout<<"Awaiting Connection"<<endl;
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PythonHandler, NULL, NULL, NULL);
    cout<<ConnectNamedPipe(hPipe, NULL)<<endl;
    cout<<"Connected"<<endl;
    WriteFile(hPipe, "MSG1\n", 5, &dwWritten, NULL);
    WriteFile(hPipe, "MSG2\n", 5, &dwWritten, NULL);
    FlushFileBuffers(hPipe);
    cout<<"Disconnecting"<<endl;
    DisconnectNamedPipe(hPipe);

}
