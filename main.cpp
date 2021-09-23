#include <windows.h>
#include <cmath>
#include <cstring>
#include "const.h"
#include "window/Window.h"


char dataList[NUM_OF_ROWS * NUM_OF_COLUMN][SIZE_OF_CELL_DATA];

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init_dataList(HANDLE &hFile);
void draw_table(HWND &hwnd, RECT &winRect);
void implDrawTable(HDC &hdc, RECT &rect, RECT &dataCell, RECT &winRect, int rectTop, int cellWidth);
void draw_line(HDC &hdc, int xLeft, int yLeft, int xRight, int yRight);
void draw_vertical_lines(HDC &hdc, INT cellWidth, INT rectTop);


int WinMain(HINSTANCE hinstance, HINSTANCE, LPTSTR, int mode) {
    Window window = Window();
    window.reg_window(hinstance, "Window", WndProc);
    window.create_window(hinstance);
    window.show_window();

    while (GetMessage(&window.msg, nullptr, 0, 0)) {
        TranslateMessage(&window.msg);
        DispatchMessage(&window.msg);
    }
    return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            HANDLE hFile;
            hFile = CreateFile(PATH_TO_DATAFILE, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                               nullptr);
            if (INVALID_HANDLE_VALUE == hFile) {
                MessageBox(hwnd, ERROR_FILE_NOT_FOUND_MESSAGE, ERROR, MB_OK);
            } else {
                init_dataList(hFile);
                CloseHandle(hFile);
            }
            break;
        case WM_PAINT:
            RECT wndRect;
            GetClientRect(hwnd, &wndRect);
            draw_table(hwnd, wndRect);
            UpdateWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


void init_dataList(HANDLE &hFile) {
    DWORD lpNumberOfBytesRead;
    DWORD nNumberOfBytesToRead;
    char buffer[SIZE_OF_CELL_DATA];

    for (auto &item : dataList) {
        nNumberOfBytesToRead = rand() % SIZE_OF_CELL_DATA;
        ReadFile(hFile, buffer, nNumberOfBytesToRead, &lpNumberOfBytesRead, nullptr);
        buffer[nNumberOfBytesToRead] = 0;
        strcpy(item, buffer);
    }
}


void draw_table(HWND &hwnd, RECT &winRect) {
    PAINTSTRUCT ps;
    RECT rect;
    RECT dataCell;
    int rectTop = 0;
    int cellWidth = winRect.right / NUM_OF_COLUMN;
    HDC hdc = BeginPaint(hwnd, &ps);
    HBRUSH brush = CreateSolidBrush(COLOR_OF_BACKGROUND);

    SelectObject(hdc, brush);
    Rectangle(hdc, winRect.left, winRect.top, winRect.right, winRect.bottom);
    DeleteObject(brush);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, COLOR_OF_TEXT);

    implDrawTable(hdc, rect, dataCell, winRect, rectTop, cellWidth);
    EndPaint(hwnd, &ps);
}

void implDrawTable(HDC &hdc, RECT &rect, RECT &dataCell, RECT &winRect, int rectTop, int cellWidth) {
    for (int i = 0; i < NUM_OF_ROWS; i++) {
        rect.top = rectTop;
        for (int j = 0; j < NUM_OF_COLUMN; j++) {
            rect.left = cellWidth * j;
            rect.right = cellWidth * (j + 1);

            dataCell.top = rect.top + MARGIN;
            dataCell.left = rect.left + MARGIN;
            dataCell.right = rect.right - MARGIN;

            DrawText(hdc, (LPCSTR) dataList[NUM_OF_COLUMN * i + j], -1, &dataCell,
                     DT_CALCRECT | DT_WORDBREAK | DT_LEFT | DT_EDITCONTROL);

            rect.bottom = dataCell.bottom + MARGIN;

            DrawText(hdc, (LPCSTR) dataList[NUM_OF_COLUMN * i + j], -1, &dataCell, DT_WORDBREAK);

            rectTop = (rect.bottom > rectTop) ? rect.bottom : rectTop;
        }
        draw_line(hdc, 0, rectTop, winRect.right, rectTop);
    }
    draw_vertical_lines(hdc, cellWidth, rectTop);
}


void draw_line(HDC &hdc, int xLeft, int yLeft, int xRight, int yRight) {
    POINT point;
    HPEN pHpen = CreatePen(PS_INSIDEFRAME, 1, COLOR_OF_LINE);
    SelectObject(hdc, pHpen);
    MoveToEx(hdc, xLeft, yLeft, &point);
    LineTo(hdc, xRight, yRight);
    DeleteObject(pHpen);
}


void draw_vertical_lines(HDC &hdc, int cellWidth, int rectTop) {
    for (int i = 0; i <= NUM_OF_COLUMN; i++) {
        draw_line(hdc, i * cellWidth, 0, i * cellWidth, rectTop);
    }
}



