#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
using namespace std;
#define		DETA_X				1
#define		DETA_Y				1
#define		EDGE_THICKNESS		1

bool isGameOver;
int isFlash = 0;
const int mWidth = 20;
const int mHeight = 20;
int headX, headY, fruitX, fruitY, mScore;
int tailX[100], tailY[100];
int nTail = 1;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void Initial()
{
    SetConsoleTitleW(L"Console_贪吃蛇");
    COORD dSize = { 80, 25 };
    SetConsoleScreenBufferSize(h, dSize); //设置窗口缓冲区大小
    CONSOLE_CURSOR_INFO _cursor = { 1, false }; //设置光标大小，隐藏光标
    SetConsoleCursorInfo(h, &_cursor);

    isGameOver = false;
    isFlash = false;
    dir = STOP;
    headX = mWidth / 2;
    headY = mHeight / 2;
    srand((unsigned)time(NULL));
    fruitX = rand() % (mWidth - 2) + 1;
    fruitY = rand() % (mHeight - 2) + 1;
    mScore = 0;
}
//光标位置
void setPos(int X, int Y)
{
    COORD pos;
    pos.X = X + DETA_X;
    pos.Y = Y + DETA_Y;
    SetConsoleCursorPosition(h, pos);
}
void DrawMap()
{
    system("cls");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int textColor = 0x06;
    SetConsoleTextAttribute(h, textColor);

    setPos(-1, -1);//绘制顶上的墙
    for (int i = 0; i < mWidth + 2; i++)
        cout << "#";

    for (int i = 0; i < mHeight; i++)
    {
        setPos(-1, i);//绘制左右的墙
        for (int j = 0; j < mWidth + 2; j++)
        {
            if (j == 0)
                cout << "#";
            else if (j == mWidth + 1)
                cout << "#";
            else
                cout << " ";
        }
        cout << endl;
    }
    setPos(-1, mHeight);//绘制下方的墙
    for (int i = 0; i < mWidth + 2; i++)
        cout << "#";
    cout << endl;
    cout << "游戏得分:" << mScore << endl;
}
void eraseSnake()
{
    for (int i = 0; i < nTail; i++)
    {
        setPos(tailX[i], tailY[i]);
        cout << " ";
    }
}
void DrawLocally()
{
    if (isFlash)
    {
        setPos(fruitX, fruitY);
        SetConsoleTextAttribute(h, 0x04);
        cout << "F";

        isFlash++;
        if (isFlash % 5 == 0)
        {
            isFlash = 0;
        }

    }
    else
    {
        setPos(fruitX, fruitY);
        SetConsoleTextAttribute(h, 0x04);
        cout << " ";
        isFlash = 1;
    }

    for (int i = 0; i < nTail; i++)
    {
        setPos(tailX[i], tailY[i]);
        if (i == 0)
        {
            SetConsoleTextAttribute(h, 0x09);
            cout << "O";
        }
        else
        {
            SetConsoleTextAttribute(h, 0x0a);
            cout << "o";
        }
    }

    setPos(0, mHeight + 1);
    SetConsoleTextAttribute(h, 0x06);
    cout << "游戏得分" << mScore;
}
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            isGameOver = true;
            break;
        default:
            break;
        }
    }
}
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;

    switch (dir)
    {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    default:
        break;
    }
    //if (headX > mWidth || headX < 0 || headY > mHeight || headY < 0)
    //	isGameOver = true;
    if (headX == fruitX && headY == fruitY)
    {
        mScore += 10;
        fruitX = rand() % (mWidth - 2) + 1;
        fruitY = rand() % (mHeight - 2) + 1;
        nTail++;
    }
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    for (int i = 1; i < nTail; i++)
        if (tailX[i] == headX && tailY[i] == headY)
            isGameOver = true;

    if (headX >= mWidth) headX = 0;	else if (headX < 0)	headX = mWidth - 1;
    if (headY >= mHeight) headY = 0; else if (headY < 0) headY = mHeight - 1;
}
int main()
{
    // 设置控制台为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    Initial();
    DrawMap();
    while (!isGameOver)
    {
        //Draw();
        Input();
        eraseSnake();
        Logic();
        DrawLocally();

        Sleep(100);
    }
    system("pause");
    return 0;
}