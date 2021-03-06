#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <deque>
#include <set>
#include <time.h>
class Snake {
	typedef enum { U, D, L, R } Status;

	std::deque<std::pair<short, short>> snake;
	std::pair<short, short> food;
	Status status;
	int score;

	void SetPosition(COORD coord)
	{
		HANDLE winHandle;
		COORD pos = coord;
		winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(winHandle, pos);
	}
	void Print(COORD pos, std::string food)
	{
		SetPosition(pos);
		std::cout << food;
	}
	bool IsCollised(std::pair<short, short> h, std::pair<short, short> f)
	{
		return h.first == f.first && h.second == f.second;
	}
	bool IsCollisSelf(std::deque<std::pair<short, short>> h)
	{
		std::deque<std::pair<short, short>> self = h;
		self.pop_front();
		while (!self.empty())
		{
			if (IsCollised(h.front(), self.front()))
				return true;
			self.pop_front();
		}
		return false;
	}
	void GenerateFood(std::deque<std::pair<short, short>> h, std::pair<short, short> & f)
	{
		srand((unsigned int)time(0));
		int x = rand() % 68 + 1;
		int y = rand() % 28 + 1;
		f.first = x & 1 ? x + 1 : x;
		f.second = y;
		for (std::deque<std::pair<short, short>>::iterator self = h.begin(); self != h.end(); ++self)
			if (IsCollised(*self, f))
				GenerateFood(h, f);
		Print(COORD{ f.first, f.second }, "■");
	}
	void MoveUp(std::deque<std::pair<short, short>>&h, std::pair<short, short> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first, h.front().second - 1));
		if (h.front().second <= 0) h.front().second = 30;
		Print({ h.front().first, h.front().second }, "■");
		if (!IsCollised(h.front(), f))
		{
			Print(COORD{ h.back().first, h.back().second }, " ");
			h.pop_back();
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
	void MoveDown(std::deque<std::pair<short, short>>&h, std::pair<short, short> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first, h.front().second + 1));
		if (h.front().second >= 30) h.front().second = 0;
		Print({ h.front().first, h.front().second }, "■");
		if (!IsCollised(h.front(), f))
		{
			Print(COORD{ h.back().first, h.back().second }, " ");
			h.pop_back();
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
	void MoveLeft(std::deque<std::pair<short, short>>&h, std::pair<short, short> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first - 2, h.front().second));
		if (h.front().first <= 0) h.front().first = 70;
		Print({ h.front().first, h.front().second }, "■");
		if (!IsCollised(h.front(), f))
		{
			Print(COORD{ h.back().first, h.back().second }, " ");
			h.pop_back();
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
	void MoveRight(std::deque<std::pair<short, short>>&h, std::pair<short, short> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first + 2, h.front().second));
		if (h.front().first >= 70) h.front().first = 0;
		Print({ h.front().first, h.front().second }, "■");
		if (!IsCollised(h.front(), f))
		{
			Print(COORD{ h.back().first, h.back().second }, " ");
			h.pop_back();
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
public:
	Snake(): snake(), food(), status(U), score(0)
	{
		for (short i = 0; i <= 4; i++)
		{
			Print(COORD{ 24, 8 + i }, "■");
			snake.push_back(std::pair<short, short>(24, 8 + i));
		}
		GenerateFood(snake, food);
		Print(COORD{ 78, 14 }, "得分：");
	}
	void Game()
	{
		while (1)
		{
			Sleep(200);
			if (GetAsyncKeyState('W') && status != D)
				status = U;
			else if (GetAsyncKeyState('S') && status != U)
				status = D;
			else if (GetAsyncKeyState('A') && status != R)
				status = L;
			else if (GetAsyncKeyState('D') && status != L)
				status = R;
			else if (status == U)
				MoveUp(snake, food);
			else if (status == D)
				MoveDown(snake, food);
			else if (status == L)
				MoveLeft(snake, food);
			else if (status == R)
				MoveRight(snake, food);
			if (IsCollisSelf(snake))
			{
				system("cls");
				Print(COORD{ 35, 14 }, "游戏结束！");
				Print(COORD{ 0, 32 }, "");
				return;
			}
			Print(COORD{ 85, 14 }, std::to_string(score));
		}
	}
};
int main(int argc, char **argv)
{
	Snake self;
	self.Game();
	system("pause");
	return 0;
}