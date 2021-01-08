#include "xmasImpl.h"

#ifdef DEBUG_CONSOLE
CHAR getch()
{
    DWORD mode, cc;
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);

    if (h == NULL)
    {
        return 0; // console not found
    }

    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    TCHAR c = 0;
    ReadConsole(h, &c, 1, &cc, NULL);
    SetConsoleMode(h, mode);
    return c;
}

int main()
{
    setup();
    do
    {
        loop();

        /*
            TCHAR key = getch();
            printf("| Pressed: %02x |\n", key);
            switch (key) {
                case 0x31: {
                    setter_00(0xff);
                    break;
                }
                case 0x32: {
                    setter_01(0xff);
                    break;
                }
                case 0x33: {
                    setter_02(0xff);
                    break;
                }
                case 0x34: {
                    setter_00(0x00);
                    break;
                }
                case 0x35: {
                    setter_01(0x00);
                    break;
                }
                case 0x36: {
                    setter_02(0x00);
                    break;
                }
            }
            printf("| +Pressed: %02x | %02x | %02x | %02x |\n", key, states[0], states[1], states[2]);
    */
    } while (1);
    return 1;
}
#endif