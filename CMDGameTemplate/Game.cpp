#include "Game.h"

bool Game::Initialize()
{
	tickCount = static_cast<int>(clock());

	return true;
}

void Game::Loop()
{
	while (isRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{

}

void Game::ProcessInput()
{
	//处理输入
	if (_kbhit()){
		update = false;
		int key = _getch();
		if (key == 'a') {
			std::cout << "a" << std::endl;
		}
	}
}

void Game::UpdateGame()
{
	//计算时间步长
	currentTick = static_cast<int>(clock());
	float deltaTime = (currentTick - tickCount) / 1000;
	tickCount = currentTick;

	//假设需要更新
	update = true;

	//检测是否需要更新游戏
	if (!update) { return; }

}

void Game::GenerateOutput() {
	//检测是否需要重新绘制
	if (!update) { return; }

	//输出图像
	system("cls");//清屏
	std::cout << tickCount ;

	//重置更新
	update = false;
}

