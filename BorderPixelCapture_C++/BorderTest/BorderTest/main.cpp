#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <chrono>

#define HORIZONTAL_LEDS	16
#define VERTICAL_LEDS	9
#define DISTANCE 128

//https://stackoverflow.com/questions/40367973/how-to-use-getpixel-to-check-different-positions
//https://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c

using namespace std::chrono;
using std::cout;
using std::endl;

//User Packet Struct
struct user_packet
{
	unsigned packet_type	: 4;
	unsigned x_coordinate	: 14;
	unsigned y_coordinate	: 14;
	unsigned r_value		: 8;
	unsigned g_value		: 8;
	unsigned b_value		: 8;
	unsigned a_value		: 8;
};

void GetDesktopResolution(int &horizontal, int &vertical);

int main(int argc, char** argv)
{
	int horizontal_resolution = 0;
	int vertical_resolution = 0;

	//Pointers to packet arrays
	user_packet * top_pixels = NULL;
	user_packet * left_pixels = NULL;
	user_packet * bot_pixels = NULL;
	user_packet * right_pixels = NULL;

	//Determine desktop resolution
	//(0, 0) is the top left corner
	//(horizontal_resolution, vertical_resolution) is the lower right corner
	GetDesktopResolution(horizontal_resolution, vertical_resolution);

	//Create Packet Array based on resolution boundaries
	//top_pixels = (user_packet *)malloc(horizontal_resolution / DISTANCE);
	//left_pixels = (user_packet *)malloc(vertical_resolution / DISTANCE);
	//bot_pixels = (user_packet *)malloc(horizontal_resolution / DISTANCE);
	//right_pixels = (user_packet *)malloc(vertical_resolution / DISTANCE);

	top_pixels = new user_packet[horizontal_resolution / DISTANCE];
	left_pixels = new user_packet[vertical_resolution / DISTANCE];
	bot_pixels = new user_packet[horizontal_resolution / DISTANCE];
	right_pixels = new user_packet[vertical_resolution / DISTANCE];

	while (1)
	{
		auto start = high_resolution_clock::now();

		HDC _hdc = GetDC(NULL);
		if (_hdc)
		{
			//Iterate through top row of pixels to fetch RGB values
			for (int horiz_iter = 0; horiz_iter <= horizontal_resolution; horiz_iter += DISTANCE)
			{
				COLORREF _color = GetPixel(_hdc, horiz_iter, 0);

				//printf("top %d\r\n", horiz_iter);

				//Get RGB values from specified pixel
				int _red = GetRValue(_color);
				int _green = GetGValue(_color);
				int _blue = GetBValue(_color);

				//0x1 indicates a color packet
				top_pixels[horiz_iter / DISTANCE].packet_type = 0x1;

				//Assign pixel coordinates
				top_pixels[horiz_iter / DISTANCE].x_coordinate = horiz_iter;
				top_pixels[horiz_iter / DISTANCE].y_coordinate = 0;

				//Assign RGBA values
				top_pixels[horiz_iter / DISTANCE].r_value = _red;
				top_pixels[horiz_iter / DISTANCE].g_value = _green;
				top_pixels[horiz_iter / DISTANCE].b_value = _blue;
				top_pixels[horiz_iter / DISTANCE].a_value = ((_red + _green + _blue) / 3);
			}

			//Iterate through left row of pixels to fetch RGB values
			for (int vert_iter = 0; vert_iter <= vertical_resolution; vert_iter += DISTANCE)
			{
				COLORREF _color = GetPixel(_hdc, 0, vert_iter);

				//printf("left %d\r\n", vert_iter);

				//Get RGB values from specified pixel
				int _red = GetRValue(_color);
				int _green = GetGValue(_color);
				int _blue = GetBValue(_color);

				//0x1 indicates a color packet
				left_pixels[vert_iter / DISTANCE].packet_type = 0x1;

				//Assign pixel coordinates
				left_pixels[vert_iter / DISTANCE].x_coordinate = 0;
				left_pixels[vert_iter / DISTANCE].y_coordinate = vert_iter;

				//Assign RGBA values
				left_pixels[vert_iter / DISTANCE].r_value = _red;
				left_pixels[vert_iter / DISTANCE].g_value = _green;
				left_pixels[vert_iter / DISTANCE].b_value = _blue;
				left_pixels[vert_iter / DISTANCE].a_value = ((_red + _green + _blue) / 3);
			}

			//Iterate through bottom row of pixels to fetch RGB values
			for (int horiz_iter = 0; horiz_iter <= horizontal_resolution; horiz_iter += DISTANCE)
			{
				COLORREF _color = GetPixel(_hdc, horiz_iter, vertical_resolution);

				//printf("bottom %d\r\n", horiz_iter);

				//Get RGB values from specified pixel
				int _red = GetRValue(_color);
				int _green = GetGValue(_color);
				int _blue = GetBValue(_color);

				//0x1 indicates a color packet
				bot_pixels[horiz_iter / DISTANCE].packet_type = 0x1;

				//Assign pixel coordinates
				bot_pixels[horiz_iter / DISTANCE].x_coordinate = horiz_iter;
				bot_pixels[horiz_iter / DISTANCE].y_coordinate = vertical_resolution;

				//Assign RGBA values
				bot_pixels[horiz_iter / DISTANCE].r_value = _red;
				bot_pixels[horiz_iter / DISTANCE].g_value = _green;
				bot_pixels[horiz_iter / DISTANCE].b_value = _blue;
				bot_pixels[horiz_iter / DISTANCE].a_value = ((_red + _green + _blue) / 3);
			}

			//Iterate through right row of pixels to fetch RGB values
			for (int vert_iter = 0; vert_iter <= vertical_resolution; vert_iter += DISTANCE)
			{
				COLORREF _color = GetPixel(_hdc, horizontal_resolution, vert_iter);

				//printf("right %d\r\n", vert_iter);

				//Get RGB values from specified pixel
				int _red = GetRValue(_color);
				int _green = GetGValue(_color);
				int _blue = GetBValue(_color);

				//0x1 indicates a color packet
				right_pixels[vert_iter / DISTANCE].packet_type = 0x1;

				//Assign pixel coordinates
				right_pixels[vert_iter / DISTANCE].x_coordinate = horizontal_resolution;
				right_pixels[vert_iter / DISTANCE].y_coordinate = vert_iter;

				//Assign RGBA values
				right_pixels[vert_iter / DISTANCE].r_value = _red;
				right_pixels[vert_iter / DISTANCE].g_value = _green;
				right_pixels[vert_iter / DISTANCE].b_value = _blue;
				right_pixels[vert_iter / DISTANCE].a_value = ((_red + _green + _blue) / 3);
			}

			//printf("Scan Complete\r\n");
		}
		
		/*{
			POINT _cursor;
			GetCursorPos(&_cursor);

			COLORREF _color = GetPixel(_hdc, _cursor.x, _cursor.y);

			int _red = GetRValue(_color);
			int _green = GetGValue(_color);
			int _blue = GetBValue(_color);

			printf("Red: 0x%02x\n", _red);
			printf("Green: 0x%02x\n", _green);
			printf("Blue: 0x%02x\n", _blue);

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD pos = { 0, 0 };
			SetConsoleCursorPosition(hConsole, pos);
		}*/

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		cout << "Scan Duration: " << duration.count() / 1000000 << "." << duration.count() % 1000000 << " Sec." << endl;
	}

	return 0;
}

void GetDesktopResolution(int &horizontal, int &vertical)
{
	//Create a rectangle object to contain desktop resolution values
	RECT desktop_area;

	//Get a handle to the desktop window
	const HWND Desktop = GetDesktopWindow();

	//Read values from Desktop and copy them to the Rectangle object
	GetWindowRect(Desktop, &desktop_area);

	//Assign resolution values to referenced objects
	horizontal = desktop_area.right;
	vertical = desktop_area.bottom;
}