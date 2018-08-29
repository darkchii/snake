#include "pch.h"
#include <iostream>
#include <windows.h>
#include <deque>
#include <set>
#include <time.h>
typedef enum { U, D, L, R } Status;
class Snake {
private:
	std::deque<std::pair<int, int>> snake;
	std::pair<int, int> food;
	Status status;
	int score;

	void SetPosition(short x, short y)
	{
		HANDLE winHandle;
		COORD pos = { x, y };
		winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(winHandle, pos);
	}
	bool IsCollised(std::deque<std::pair<int, int>> h, std::pair<int, int> f)
	{
		return h.front().first == f.first && h.front().second == f.second;
	}
	bool IsCollisSelf(std::deque<std::pair<int, int>> h)
	{
		std::deque<std::pair<int, int>> self = h;
		self.pop_front();
		while (!self.empty())
		{
			if (IsCollised(h, self.front()))
				return true;
			self.pop_front();
		}
		return false;
	}
	void GenerateFood(std::deque<std::pair<int, int>> h, std::pair<int, int> & f)
	{
		srand((unsigned int)time(0));
		int x = rand() % 68 + 1;
		int y = rand() % 28 + 1;
		f.first = x & 1 ? x + 1 : x;
		f.second = y;
		for (std::deque<std::pair<int, int>>::iterator self = h.begin(); self != h.end(); ++self)
			if (self->first == f.first && self->second == f.second)
				GenerateFood(h, f);
		SetPosition(f.first, f.second);
		std::cout << "■";
	}
	void MoveUp(std::deque<std::pair<int, int>>&h, std::pair<int, int> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first, h.front().second - 1));
		if (h.front().second <= 0) h.front().second = 30;
		SetPosition(h.front().first, h.front().second);
		std::cout << "■";
		if (!IsCollised(h, f))
		{
			SetPosition(h.back().first, h.back().second);
			h.pop_back();
			std::cout << " ";
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
	void MoveDown(std::deque<std::pair<int, int>>&h, std::pair<int, int> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first, h.front().second + 1));
		if (h.front().second >= 30) h.front().second = 0;
		SetPosition(h.front().first, h.front().second);
		std::cout << "■";
		if (!IsCollised(h, f))
		{
			SetPosition(h.back().first, h.back().second);
			h.pop_back();
			std::cout << " ";
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
	void MoveLeft(std::deque<std::pair<int, int>>&h, std::pair<int, int> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first - 2, h.front().second));
		if (h.front().first <= 0) h.front().first = 70;
		SetPosition(h.front().first, h.front().second);
		std::cout << "■";
		if (!IsCollised(h, f))
		{
			SetPosition(h.back().first, h.back().second);
			h.pop_back();
			std::cout << " ";
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
	void MoveRight(std::deque<std::pair<int, int>>&h, std::pair<int, int> &f)
	{
		h.push_front(std::pair <int, int>(h.front().first + 2, h.front().second));
		if (h.front().first >= 70) h.front().first = 0;
		SetPosition(h.front().first, h.front().second);
		std::cout << "■";
		if (!IsCollised(h, f))
		{
			SetPosition(h.back().first, h.back().second);
			h.pop_back();
			std::cout << " ";
		}
		else
		{
			score += 25;
			GenerateFood(h, f);
		}
	}
public:
	Snake()
	{
		status = U;
		score = 0;
		for (int i = 0; i <= 4; i++)
		{
			SetPosition(24, 8 + i);
			std::cout << "■";
			snake.push_back(std::pair<int, int>(24, 8 + i));
		}
		GenerateFood(snake, food);
		SetPosition(78, 14);
		std::cout << "得分：";
	}
	void Game()
	{
		while (1)
		{
			Sleep(200);
			if (GetAsyncKeyState(VK_F1) && status != D)
				status = U;
			else if (GetAsyncKeyState(VK_F2) && status != U)
				status = D;
			else if (GetAsyncKeyState(VK_F3) && status != R)
				status = L;
			else if (GetAsyncKeyState(VK_F5) && status != L)
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
				SetPosition(35, 14);
				std::cout << "游戏结束！";
				SetPosition(0, 32);
				return;
			}
			SetPosition(85, 14);
			std::cout << score;
		}
	}
};
int main(int argc, char **argv)
{
	Snake self;
	self.Game();
	return 0;
}