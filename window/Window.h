#include <windows.h>

#ifndef OSISP_LAB1_WINDOW_H
#define OSISP_LAB1_WINDOW_H

class Window {
public:
    HWND hwnd;
    MSG msg{};
    WNDCLASS wcl{};

    Window(HWND hwnd, MSG msg, WNDCLASS wcl);

    Window();

    bool reg_window(HINSTANCE hinstance, LPCSTR lpcstr, WNDPROC wndProc);

    void create_window(HINSTANCE hinstance);

    void show_window() const;
};


#endif
