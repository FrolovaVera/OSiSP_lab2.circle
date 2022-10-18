#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <corecrt_math_defines.h>
using namespace std;

#define ROW_COUNT 3
#define COLUMN_COUNT 6

LPCTSTR windowName = TEXT("OSiSP_lab_2");
int     coorY[ROW_COUNT * COLUMN_COUNT][2];
int     coorX[ROW_COUNT * COLUMN_COUNT][2];

void WriteWord(HANDLE hFile, HDC hdc, RECT windowSize);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR cmdLine, _In_ int showMode)
{
    MSG msg;

    WNDCLASSEX windowClassEx;
    windowClassEx.cbSize = sizeof(WNDCLASSEX);
    windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
    windowClassEx.lpfnWndProc = WndProc;
    windowClassEx.cbClsExtra = 0;
    windowClassEx.cbWndExtra = 0;
    windowClassEx.hInstance = hInstance;
    windowClassEx.hIcon = LoadIcon(0, IDI_WINLOGO);;
    windowClassEx.hCursor = LoadCursor(0, IDC_ARROW);
    windowClassEx.hbrBackground = (HBRUSH)0;
    windowClassEx.lpszMenuName = 0;
    windowClassEx.lpszClassName = windowName;
    windowClassEx.hIconSm = 0;
    RegisterClassEx(&windowClassEx);
    
    HWND hWnd = CreateWindow(windowName,  // Указатель на зарегистрированное имя класса.
        windowName,                       // Указатель на имя окна.
        WS_OVERLAPPEDWINDOW,              // Стиль окна.
        CW_USEDEFAULT,                    // Горизонтальная позиция окна.
        CW_USEDEFAULT,                    // Вертикальная позиция окна.
        CW_USEDEFAULT,                    // Ширина окна.
        CW_USEDEFAULT,                    // Высота окна.
        NULL,                             // Дескриптор родительского или окна владельца.
        NULL,                             // Дескриптор меню или ID дочернего окна.
        hInstance,                        // Дескриптор экземпляра приложения.
        NULL);

    ShowWindow(hWnd, showMode);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
      case (WM_PAINT):
      {
          const char FileName[] = "C:/Users/Vera/Desktop/3 курс/5 сем/осисп/OSiSP_lab2.circle/data.txt";
          HANDLE hFile = CreateFile(FileName, // имя файла
              GENERIC_READ,                   // режим доступа(ч/з/ч-з)
              0,                              // совместный доступ(ч/з/ч-з)
              NULL,                           // дескр. защиты
              OPEN_EXISTING,                  // как действовать(Открывает файл)
              FILE_ATTRIBUTE_NORMAL,          // атрибуты файла(У файла нет других установленных атрибутов)
              NULL);                          // дескриптор файла шаблона
          

          RECT windowSize;
          GetClientRect(hWnd, &windowSize);

          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hWnd, &ps);
          
          HBRUSH rectangleBrush = CreateSolidBrush(RGB(255, 255, 255));
          SelectObject(hdc, rectangleBrush);
          Rectangle(hdc, windowSize.left, windowSize.top, windowSize.right, windowSize.bottom);
          DeleteObject(rectangleBrush);

          HBRUSH brush = CreateSolidBrush(RGB(204, 153, 210));
          SelectObject(hdc, brush);
          Ellipse(hdc, (windowSize.right- windowSize.bottom)/2, windowSize.top, (windowSize.right - windowSize.bottom) / 2 + windowSize.bottom, windowSize.bottom);
          DeleteObject(brush);

          SetBkMode(hdc, TRANSPARENT);
          SetTextColor(hdc, RGB(101, 82, 85));
          const auto pen = CreatePen(PS_SOLID, 4, RGB(101, 82, 85));//создает логическое перо с заданным стилем, шириной и цветом.
          SelectObject(hdc, pen);
          WriteWord(hFile, hdc, windowSize); 
          DeleteObject(hdc);
          EndPaint(hWnd, &ps);
          CloseHandle(hFile);
      }
      break;

      case WM_SIZE:
      {
          InvalidateRect(hWnd, NULL, FALSE);
      }
      break;

      case WM_GETMINMAXINFO:
      {
          MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
          POINT ptMin = { 486, 486 }, ptMax = { 2*486, 2*486 };
          pInfo->ptMinTrackSize = ptMin;
          pInfo->ptMaxTrackSize = ptMax;
          return 0;
      }
      
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

string ConcatenationStr(char cBufferText[255], const int num) {
    string str;
    for (int i = 0; i < num; i++) {
        str = str + cBufferText[i];
    }
    return str;
}

void WriteWord(HANDLE hFile, HDC hdc,RECT windowSize)
{

    double x0 = windowSize.right / 2, x;
    double y0 = windowSize.bottom / 2, y;
    int    strLen = GetFileSize(hFile, NULL); int id = 0;
    std::string massiveStr;
    char*  cBufferText = new char[strLen];
    DWORD  dwByte;
    RECT   cells;
    

    ReadFile(hFile, cBufferText, strLen, &dwByte, NULL);
    std::string copid(cBufferText);
    massiveStr = copid;
    
    for (double i = 0; i < 360; i=i+(360/strLen)) 
    {
       
        cells.left = x0 + (windowSize.bottom / 2 - 20) * cos(M_PI * 2 * i / 360) - 7;
        cells.top = y0 + (windowSize.bottom / 2 - 20) * sin(M_PI * 2 * i / 360) - 7;
        auto font = CreateFont(20, 0, i, 0, FW_DEMIBOLD, true, false, false, 0, 0, 0, 0, FF_ROMAN, nullptr);
        SelectObject(hdc, font);
        DrawText(hdc, massiveStr.c_str()+id, 1, &cells, NULL); id = id + 1;
        DeleteObject(font);
    }
}
