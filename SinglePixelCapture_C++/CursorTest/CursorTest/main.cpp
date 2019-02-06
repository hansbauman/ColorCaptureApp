#include<windows.h>
#include<stdio.h>

//https://stackoverflow.com/questions/40367973/how-to-use-getpixel-to-check-different-positions

int main(int argc, char** argv)
{
	while (1)
	{
		HDC _hdc = GetDC(NULL);
		if (_hdc)
		{
			POINT _cursor;
			GetCursorPos(&_cursor);

			//COLORREF _color = (*pGetPixel) (_hdc, _cursor.x, _cursor.y);
			COLORREF _color = GetPixel(_hdc, _cursor.x, _cursor.y);

			int _red = GetRValue(_color);
			int _green = GetGValue(_color);
			int _blue = GetBValue(_color);

			printf("Red: 0x%02x\n", _red);
			printf("Green: 0x%02x\n", _green);
			printf("Blue: 0x%02x\n", _blue);

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD pos = {0, 0};
			SetConsoleCursorPosition(hConsole, pos);
		}
	}

	return 0;
}