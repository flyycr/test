#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//全局操作数组
int arr[4][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
//全局窗口句柄
HWND g_hwnd;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR *szAppName = TEXT("2048");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("无法注册窗口类"), TEXT("注册错误"), MB_ICONERROR);
		return -1;
	}
	hwnd = CreateWindow(szAppName, TEXT("我的2048游戏"), WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
		NULL, NULL, hInstance, NULL);
	g_hwnd = hwnd;
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void newNum()
{
	int num = 0;//一个在0至9之间的书
	int xPos = 0, yPos = 0;
	srand(time(0));
	do {
		xPos = rand() % 4;
		yPos = rand() % 4;
	} while (arr[xPos][yPos] != 0);
	num = rand() % 10;
	if (num < 7)
		arr[xPos][yPos] = 2;
	else
		arr[xPos][yPos] = 4;
}

//画数组函数
void DrawCanvas(HWND hwnd)
{
	HBRUSH hBrush;
	HDC hdc;
	RECT rt;
	TCHAR szBuffer[10];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (arr[i][j] != 0)
			{
				LOGFONT logfont;
				hdc = GetDC(hwnd);
				//设置数字的字体大小
				ZeroMemory(&logfont, sizeof(LOGFONT));
				logfont.lfCharSet = GB2312_CHARSET;
				logfont.lfHeight = -50; //设置字体的大小
				HFONT hFont = CreateFontIndirect(&logfont);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(200, 200, 0));
				SetBkMode(hdc, TRANSPARENT);
				SelectObject(hdc, hFont);
				//创建画刷
				hBrush = CreateSolidBrush(RGB((arr[i][j] * 10) % 256, (arr[i][j] * 40) % 256, 0));
				SetRect(&rt, j * 100, i * 100, j * 100 + 100, i * 100 + 100);
				FillRect(hdc, &rt, hBrush);
				TextOut(hdc, j * 100, i * 100 + 25, szBuffer, wsprintf(szBuffer, TEXT("%d"), arr[i][j]));
				ReleaseDC(hwnd, hdc);
				DeleteObject(hBrush);
			}
			else
			{
				SetRect(&rt, j * 100, i * 100, j * 100 + 100, i * 100 + 100);
				hdc = GetDC(hwnd);
				hBrush = CreateSolidBrush(RGB(200, 250, 0));
				FillRect(hdc, &rt, hBrush);
				ReleaseDC(hwnd, hdc);
				DeleteObject(hBrush);
			}
		}
	}
}
//判断数组是否满了
bool isFull()
{
	bool full = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (arr[i][j] == 0)
			{
				full = false;
			}
		}
	}
	return full;
}
//处理数组函数
void changeArr(int direct)
{
	bool xiaoyici = false;
	bool chendi = false;
	switch (direct)
	{
		//上键 done
	case 0:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1先沉底
				for (int b = 0; b < 4; b++)
				{
					for (int m = 0; m < 4; ++m)
					{
						for (int k = m; k < 3; ++k)
						{

							if (arr[k][i] == 0 && arr[k + 1][i] != 0)
							{
								chendi = true;
								arr[k][i] = arr[k + 1][i];
								arr[k + 1][i] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = a; j < 4; ++j)
				{


					if (arr[j][i] != 0 && arr[j][i] == arr[j + 1][i])
					{
						xiaoyici = true;
						arr[j][i] <<= 1;
						arr[j + 1][i] = 0;
						j++;
						a++;

					}
					else
					{
						continue;
					}
				}

			}


		}//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}

	}
	break;
	//下键
	case 1:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1先沉底
				for (int b = 3; b >= 0; b--)
				{
					for (int m = 3; m >= 0; --m)
					{
						for (int k = m; k > 0; --k)
						{

							if (arr[k][i] == 0 && arr[k - 1][i] != 0)
							{
								chendi = true;
								arr[k][i] = arr[k - 1][i];
								arr[k - 1][i] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = 3 - a; j >= 0; --j)
				{
					if (arr[j][i] != 0 && arr[j][i] == arr[j - 1][i])
					{
						xiaoyici = true;
						arr[j][i] += arr[j][i];
						arr[j - 1][i] = 0;
						a++;
						--j;
					}
					else
					{
						continue;
					}
				}

			}
		}
		//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}
	}
	break;
	//左键done
	case 2:
	{

		//开始数组操作
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1先沉底
				for (int b = 0; b < 4; b++)
				{
					for (int m = 0; m < 4; ++m)
					{
						for (int k = m; k < 3; ++k)
						{

							if (arr[i][k] == 0 && arr[i][k + 1] != 0)
							{
								chendi = true;
								arr[i][k] = arr[i][k + 1];
								arr[i][k + 1] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = a; j < 4; ++j)
				{


					if (arr[i][j] != 0 && arr[i][j] == arr[i][j + 1])
					{
						xiaoyici = true;
						arr[i][j] <<= 1;
						arr[i][j + 1] = 0;
						j++;
						a++;

					}
					else
					{
						continue;
					}
				}

			}


		}//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}
	}
	break;
	//右键
	case 3:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1先沉底
				for (int b = 3; b >= 0; b--)
				{
					for (int m = 3; m >= 0; --m)
					{
						for (int k = m; k > 0; --k)
						{

							if (arr[i][k] == 0 && arr[i][k - 1] != 0)
							{
								chendi = true;
								arr[i][k] = arr[i][k - 1];
								arr[i][k - 1] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = 3 - a; j >= 0; --j)
				{
					if (arr[i][j] != 0 && arr[i][j] == arr[i][j - 1])
					{
						xiaoyici = true;
						arr[i][j] += arr[i][j];
						arr[i][j - 1] = 0;
						a++;
						--j;
					}
					else
					{
						continue;
					}
				}

			}
		}
		//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}
	}
	break;
	default:
		return;
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		newNum();
		//newNum();
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			//如果上键被按下
		case VK_UP:
		{

			changeArr(0);
			DrawCanvas(hwnd);


		}
		break;
		//如果下键被按下
		case VK_DOWN:
		{
			changeArr(1);
			DrawCanvas(hwnd);
		}
		break;
		//如果左键被按下
		case VK_LEFT:
		{
			changeArr(2);
			DrawCanvas(hwnd);
		}
		break;
		//如果右键被按下
		case VK_RIGHT:
		{
			changeArr(3);
			DrawCanvas(hwnd);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//全局操作数组
int arr[4][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
//全局窗口句柄
HWND g_hwnd;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR *szAppName = TEXT("2048");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("无法注册窗口类"), TEXT("注册错误"), MB_ICONERROR);
		return -1;
	}
	hwnd = CreateWindow(szAppName, TEXT("我的2048游戏"), WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
		NULL, NULL, hInstance, NULL);
	g_hwnd = hwnd;
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void newNum()
{
	int num = 0;//一个在0至9之间的书
	int xPos = 0, yPos = 0;
	srand(time(0));
	do {
		xPos = rand() % 4;
		yPos = rand() % 4;
	} while (arr[xPos][yPos] != 0);
	num = rand() % 10;
	if (num < 7)
		arr[xPos][yPos] = 2;
	else
		arr[xPos][yPos] = 4;
}

//画数组函数
void DrawCanvas(HWND hwnd)
{
	HBRUSH hBrush;
	HDC hdc;
	RECT rt;
	TCHAR szBuffer[10];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (arr[i][j] != 0)
			{
				LOGFONT logfont;
				hdc = GetDC(hwnd);
				//设置数字的字体大小
				ZeroMemory(&logfont, sizeof(LOGFONT));
				logfont.lfCharSet = GB2312_CHARSET;
				logfont.lfHeight = -50; //设置字体的大小
				HFONT hFont = CreateFontIndirect(&logfont);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(200, 200, 0));
				SetBkMode(hdc, TRANSPARENT);
				SelectObject(hdc, hFont);
				//创建画刷
				hBrush = CreateSolidBrush(RGB((arr[i][j] * 10) % 256, (arr[i][j] * 40) % 256, 0));
				SetRect(&rt, j * 100, i * 100, j * 100 + 100, i * 100 + 100);
				FillRect(hdc, &rt, hBrush);
				TextOut(hdc, j * 100, i * 100 + 25, szBuffer, wsprintf(szBuffer, TEXT("%d"), arr[i][j]));
				ReleaseDC(hwnd, hdc);
				DeleteObject(hBrush);
			}
			else
			{
				SetRect(&rt, j * 100, i * 100, j * 100 + 100, i * 100 + 100);
				hdc = GetDC(hwnd);
				hBrush = CreateSolidBrush(RGB(200, 250, 0));
				FillRect(hdc, &rt, hBrush);
				ReleaseDC(hwnd, hdc);
				DeleteObject(hBrush);
			}
		}
	}
}
//判断数组是否满了
bool isFull()
{
	bool full = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (arr[i][j] == 0)
			{
				full = false;
			}
		}
	}
	return full;
}
//处理数组函数
void changeArr(int direct)
{
	bool xiaoyici = false;
	bool chendi = false;
	switch (direct)
	{
		//上键 done
	case 0:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1先沉底
				for (int b = 0; b < 4; b++)
				{
					for (int m = 0; m < 4; ++m)
					{
						for (int k = m; k < 3; ++k)
						{

							if (arr[k][i] == 0 && arr[k + 1][i] != 0)
							{
								chendi = true;
								arr[k][i] = arr[k + 1][i];
								arr[k + 1][i] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = a; j < 4; ++j)
				{


					if (arr[j][i] != 0 && arr[j][i] == arr[j + 1][i])
					{
						xiaoyici = true;
						arr[j][i] <<= 1;
						arr[j + 1][i] = 0;
						j++;
						a++;

					}
					else
					{
						continue;
					}
				}

			}


		}//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}

	}
	break;
	//下键
	case 1:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1先沉底
				for (int b = 3; b >= 0; b--)
				{
					for (int m = 3; m >= 0; --m)
					{
						for (int k = m; k > 0; --k)
						{

							if (arr[k][i] == 0 && arr[k - 1][i] != 0)
							{
								chendi = true;
								arr[k][i] = arr[k - 1][i];
								arr[k - 1][i] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = 3 - a; j >= 0; --j)
				{
					if (arr[j][i] != 0 && arr[j][i] == arr[j - 1][i])
					{
						xiaoyici = true;
						arr[j][i] += arr[j][i];
						arr[j - 1][i] = 0;
						a++;
						--j;
					}
					else
					{
						continue;
					}
				}

			}
		}
		//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}
	}
	break;
	//左键done
	case 2:
	{

		//开始数组操作
		for (int i = 0; i < 4; i++)
		{

			for (int a = 0; a < 4; a++) {

				//1先沉底
				for (int b = 0; b < 4; b++)
				{
					for (int m = 0; m < 4; ++m)
					{
						for (int k = m; k < 3; ++k)
						{

							if (arr[i][k] == 0 && arr[i][k + 1] != 0)
							{
								chendi = true;
								arr[i][k] = arr[i][k + 1];
								arr[i][k + 1] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = a; j < 4; ++j)
				{


					if (arr[i][j] != 0 && arr[i][j] == arr[i][j + 1])
					{
						xiaoyici = true;
						arr[i][j] <<= 1;
						arr[i][j + 1] = 0;
						j++;
						a++;

					}
					else
					{
						continue;
					}
				}

			}


		}//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}
	}
	break;
	//右键
	case 3:
	{
		//开始数组操作
		for (int i = 0; i < 4; i++)
		{
			for (int a = 0; a < 4; a++)
			{

				//1先沉底
				for (int b = 3; b >= 0; b--)
				{
					for (int m = 3; m >= 0; --m)
					{
						for (int k = m; k > 0; --k)
						{

							if (arr[i][k] == 0 && arr[i][k - 1] != 0)
							{
								chendi = true;
								arr[i][k] = arr[i][k - 1];
								arr[i][k - 1] = 0;
							}
						}
					}
				}
				//2两两比较

				for (int j = 3 - a; j >= 0; --j)
				{
					if (arr[i][j] != 0 && arr[i][j] == arr[i][j - 1])
					{
						xiaoyici = true;
						arr[i][j] += arr[i][j];
						arr[i][j - 1] = 0;
						a++;
						--j;
					}
					else
					{
						continue;
					}
				}

			}
		}
		//结束数组操作
		//判断是否死亡
		if (!xiaoyici && isFull())
		{
			MessageBox(g_hwnd, TEXT("你挂掉了"), TEXT("糟糕"), MB_OK);
			SendMessage(g_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			//随机生成一个新的2或4放入数组
			if (xiaoyici || chendi) { newNum(); }
		}
	}
	break;
	default:
		return;
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		newNum();
		//newNum();
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			//如果上键被按下
		case VK_UP:
		{

			changeArr(0);
			DrawCanvas(hwnd);


		}
		break;
		//如果下键被按下
		case VK_DOWN:
		{
			changeArr(1);
			DrawCanvas(hwnd);
		}
		break;
		//如果左键被按下
		case VK_LEFT:
		{
			changeArr(2);
			DrawCanvas(hwnd);
		}
		break;
		//如果右键被按下
		case VK_RIGHT:
		{
			changeArr(3);
			DrawCanvas(hwnd);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
