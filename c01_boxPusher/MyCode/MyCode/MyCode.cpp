#include <iostream>
#include <cstdlib>
#include <conio.h> // _WIN32
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

enum Direction
{
    Up,
    Down,
    Left,
    Right,
    InvalidDirection
};

enum MatrixElement
{
    Player,
    Brick,
    Blank,
};

enum DirectionState
{
    CannotMove,
    Movable,
};

namespace MyGame
{
    string tips = "Hello";

    const int ROW = 5;
    const int COL = 10;

    MatrixElement** matrix = nullptr;
    Direction d = InvalidDirection;
    char c = 0;

    int playerCoorX = 0;
    int playerCoorY = 0;

    void Move(int x, int y, Direction dir)
    {
        if (x < 0 || x >= ROW || y < 0 || y >= COL)
            return;

        int newX = x, newY = y;
        if (dir == Up) newX--;
        else if (dir == Down) newX++;
        else if (dir == Left) newY--;
        else if (dir == Right) newY++;

        if (newX < 0 || newX >= ROW || newY < 0 || newY >= COL)
            return;

        matrix[newX][newY] = matrix[x][y];
        matrix[x][y] = Blank;
    }

    DirectionState IsMovable(int x, int y, Direction dir)
    {
        if (x < 0 || x >= ROW || y < 0 || y >= COL)
            return CannotMove;

        int newX = x, newY = y;
        if (dir == Up) newX--;
        else if (dir == Down) newX++;
        else if (dir == Left) newY--;
        else if (dir == Right) newY++;
        else
            return CannotMove;

        if (matrix[x][y] == Player)
        {
            return IsMovable(newX, newY, dir);
        }
        else if (matrix[x][y] == Brick)
        {
            if (IsMovable(newX, newY, dir))
            {
                Move(x, y, dir);
                return Movable;
            }
            else
            {
                return CannotMove;
            }
        }
        else if (matrix[x][y] == Blank)
        {
            return Movable;
        }
    }

    void Move()
    {
        if (d != InvalidDirection)
        {
            if (d == Up) tips = "Up";
            if (d == Down) tips = "Down";
            if (d == Left) tips = "Left";
            if (d == Right) tips = "Right";
            if (d == InvalidDirection) tips = "Invalid";
            tips += to_string(playerCoorX) + " // " + to_string(playerCoorY) + "//" + to_string(d) + "IsMovable: " + to_string(IsMovable(playerCoorX, playerCoorY, d));

            if (IsMovable(playerCoorX, playerCoorY, d))
            {
                Move(playerCoorX, playerCoorY, d);

                int newX = playerCoorX, newY = playerCoorY;
                if (d == Up) newX--;
                if (d == Down) newX++;
                if (d == Left) newY--;
                if (d == Right) newY++;
                playerCoorX = newX;
                playerCoorY = newY;
            }
        }
    }

    void RandomlyPutBrick(int num)
    {
        // Shuffle {num}-blocks into matrix RANDOMLY
        int tot = ROW * COL;
        int* arr = new int[tot]();
        int tail = tot - 1;
        for (int i = 0; i < tot; ++i)
            arr[i] = i;
        
        // random
        srand(time(nullptr)); 
        for (int i = 0; i < num; ++i)
        {
            int a = 1, b = tail + 1;
            int tar = rand() % (b - a + 1) + a;
            // swap
            int tmp = arr[tar];
            arr[tar] = arr[tail];
            arr[tail] = tmp;
            tail--;
        }

        // put tail index into matrix
        for (int i = tail + 1; i < tot; ++i)
        {
            int row = arr[i] / COL;
            int col = arr[i] % COL;
            matrix[row][col] = Brick;
        }
    }

    bool GetInput()
    {
        if (_kbhit())
        {
            c = _getch();
            if (c == 'a' || c == 'A')
                d = Left;
            else if (c == 's' || c == 'S')
                d = Down;
            else if (c == 'd' || c == 'D')
                d = Right;
            else if (c == 'w' || c == 'W')
                d = Up;
            else
                d = InvalidDirection;
            return true;
        }
        else
        {
            d = InvalidDirection;
            return false;
        }
    }

    void Update()
    {
        if (matrix == nullptr)
        {
            // Init Blank
            matrix = new MatrixElement*[ROW];
            for (int i = 0; i < ROW; ++i)
            {
                matrix[i] = new MatrixElement[COL];
                for (int j = 0; j < COL; ++j)
                {
                    matrix[i][j] = Blank;
                }
            }
            // Put bricks
            RandomlyPutBrick(2);
            // Put Player
            matrix[playerCoorX][playerCoorY] = Player;
        }
        Move();
    }

    void Draw()
    {
        // clear
        system("cls");

        cout << tips << endl;

        for (int i = 0; i <= ROW + 1; ++i)
        {
            for (int j = 0; j <= COL + 1; ++j)
            {
                if (i == 0 || i == ROW + 1 || j == 0 || j == COL + 1)
                {
                    cout << "#";
                }
                else
                {
                    MatrixElement ele = matrix[i - 1][j - 1];
                    if (ele == Player)
                        cout << "P";
                    else if (ele == Blank)
                        cout << ".";
                    else if (ele == Brick)
                        cout << "O";
                    else
                        cout << "X";
                }
            }
            cout << endl;
        }
    }
}

int main()
{
    MyGame::Update();
    MyGame::Draw();

    while (true)
    {
        if (MyGame::GetInput())
        {
            MyGame::Update();
            MyGame::Draw();
        }
    }
    return 0;
}