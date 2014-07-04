
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "sac_resource.h"
#include "WinSacView.h"

extern "C" {
    #include "debug.h"
    /* C Code */
    char*    getline_stdin();
    void     main_command(char *kmsg, int n);
    void     win_init();
    SacView *SacViewInit();
    void     SacViewDraw(SacView *view);
    void     win_set_current(SacView *view);
    void     sleep(int seconds);
    char *   ImageToBounds(SacImage *im, SacRect bounds);

    void     SacViewWindowsAdd(SacViewWindows *wins, SacView *view);
    SacView *SacViewWindowsGetByHandle(SacViewWindows *wins, HWND handle);
    SacViewWindows *SacViewWindowsInit();

    /* C++ Code */
    void      sac_draw_line (HDC hdc, 
                             float x1, float y1, float x2, float y2,
                             float red, float green, float blue, 
                             int width);
    void      sac_draw_image (HDC hdc, int x, int y, int w, int h, char *data);
    void      SacWindowShow  (SacView *view);
    SacView * SacWindow      (int id);
    void      SacWindowAdd   (int id);

    int snprintf(char *str, size_t size, const char *format, ...);
    SacViewWindows *wins = NULL;
}

#define MAX_CONSOLE_LINES 500

#define SAC_WINDOW_CREATE (WM_APP + 1)
#define SAC_WINDOW_SHOW   (WM_APP + 2)

char Application[] = "SAC";
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
DWORD ThreadId;

//SacView *view;

DWORD main_thread;

int
show_prompt() {
  fprintf(stdout, "SAC> ");
  return TRUE;	
}

DWORD WINAPI ConsoleIO(LPVOID lpArg) {
  char *line;
  while(show_prompt() && (line = getline_stdin())) {
      main_command(line, strlen(line));
      free(line);
      line = NULL;
  }
  return 0;
}

void
SacWindowAdd(int id) {
    DEBUG("\n");
    if(GetCurrentThreadId() == main_thread) {
        SacView *view;
        DEBUG("main thread\n");
        view = SacWindow( id );
        SacViewWindowsAdd(wins, view);
        ShowWindow(view->window_handle, SW_SHOWNORMAL);
        UpdateWindow(view->window_handle);
        PostThreadMessage(ThreadId, SAC_WINDOW_CREATE, id, 0);
    } else {
      MSG msg;
      DEBUG("post message: CREATE WINDOW\n");
      if(!PostThreadMessage(main_thread, SAC_WINDOW_CREATE, id,0)) {
            fprintf(stderr, "Error attempting to create window on GUI Thread\n");
      }
      GetMessage(&msg, NULL, 0,0);
      if(msg.message != SAC_WINDOW_CREATE) {
        fprintf(stderr, "Error creating window\n");
      }
      DEBUG("post message: CREATE WINDOW: DONE\n");
    }
}

SacView * 
SacWindow(int id) {
    SacView *view;
    HWND hwnd;
    TCHAR title[100];
    int n;

    memset(&title[0], 0, 100);

#ifdef UNICODE
    n = swprintf(&title[0], 100, "Sac Plot Window: %d", id);
#else
    n = snprintf(&title[0], 100, "Sac Plot Window: %d", id);
#endif

    hwnd = CreateWindow(Application, 
                        title,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 
                        400, 300,
                        NULL,NULL,
                        NULL,//hInstance,
                        NULL);
    
    if (!hwnd) {
        fprintf(stderr, "Error creating window\n");
        return 0;
    }
    view = SacViewInit();
    view->window_handle = hwnd;
    view->id            = id;
    win_set_current(view);

    return view;
}

void
SacWindowShow(SacView *view) {
    if(!view) {
        return;
    }
    if(GetCurrentThreadId() == main_thread) {
        //ShowWindow(view->window_handle, SW_SHOW);
        SetWindowPos(view->window_handle, HWND_TOP, 0,0, 0,0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
    } else {
        if(!PostThreadMessage(main_thread, SAC_WINDOW_SHOW, (WPARAM)view, 0)) {
            fprintf(stderr, "Error attempting to show_window on GUI Thread\n");
        }
    }
}

int 
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR pCmdLine,
        int nCmdShow)  {
    
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    if(hPrevInstance) {} 
    if(pCmdLine){}
    
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL,IDI_WINLOGO);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
    wc.lpszMenuName  = MAKEINTRESOURCE(SAC_MENU);
    wc.lpszClassName = Application;
    
    if (!RegisterClass(&wc))
        return 0;
    
    {
        /* Create Console */
        AllocConsole();    
        freopen("CONIN$", "r", stdin); 
        freopen("CONOUT$", "w", stdout); 
        freopen("CONOUT$", "w", stderr); 
    }

    main_thread = GetCurrentThreadId();

    /* Initialize the Window List */
    wins = SacViewWindowsInit();

    /* Initialize SAC */
    win_init();

    /* Put Console on its own Thread */
    CreateThread(NULL, 0, ConsoleIO, NULL, 0, &ThreadId);

    /* Loop over Events */
    while (GetMessage(&msg,NULL,0,0) > 0) {
        if(msg.message == SAC_WINDOW_CREATE) {
            SacWindowAdd(msg.wParam);
        } else if(msg.message == SAC_WINDOW_SHOW) {
            SacWindowShow((SacView *)msg.wParam);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);

    return 0;
}

void
sac_draw_line(HDC hdc, 
              float x1, float y1, 
              float x2, float y2, 
              float red, float green, float blue, 
              int width) {
    Graphics graphics(hdc);
    Pen      pen(Color(255,red,green,blue), width);
    graphics.DrawLine(&pen, x1,y1, x2,y2);
}

void
sac_draw_image(HDC hdc, int x, int y, int w, int h, char *data) {
    Graphics graphics(hdc);
    Bitmap   bitmap(w,h, w*4, PixelFormat32bppARGB, (BYTE *) data);
    graphics.DrawImage(&bitmap, x, y);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    SacView *view;
    switch (msg) {
    case WM_COMMAND:
        switch(LOWORD(wparam)) {
        case ID_FILE_EXIT:
            fprintf(stderr, "File Exit\n");
            break;
        case ID_STUFF_GO:
            fprintf(stderr, "Stuff Go\n");
            break;
        }
        break;
    case WM_PAINT:
        view = SacViewWindowsGetByHandle(wins, hwnd);
        if(view) {
            SacViewDraw(view);
        } else {
            fprintf(stderr, "View undefined\n");
        }
        break;
    case WM_DESTROY:
        DEBUG("WINDOW DESTORY\n");
        PostQuitMessage(0);
        TerminateThread(&ThreadId, 0);
        break;
        
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
} 


