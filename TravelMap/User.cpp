#include "User.h"

int MyInpuSymbolNum(char *String)
{
    HWND hwnd = GetHWnd();
    char *temp = (char*)calloc(10,sizeof(temp));
    int num = 0;
    while (true) {
        bool IsDigitNum = 1;       
        InputBox(temp, 10, String);
        for (int i = 0; i < strlen(temp) - 1; i++) {
            if (!isdigit(temp[i])) {
                MessageBox(hwnd, "������Ĳ���һ���Ϸ�������", "��ʾ", MB_OK | MB_ICONERROR);
                IsDigitNum = 0;
                break;
            }
        }
        if (IsDigitNum) {
            num = atoi(temp);
            break;
        }
    }
    return num;
}
