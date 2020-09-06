#include "Game.h"

bool Game::Initialize()
{
	tickCount = clock();
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
			//TEST
			std::cout << "a" << std::endl;
			update = true;
		}
	}
}

void Game::UpdateGame()
{
	//计算时间步长
	while (clock() - tickCount < 16);
	currentTick = clock();
	float deltaTime = static_cast<float>((currentTick - tickCount)) / CLOCKS_PER_SEC;
	tickCount = currentTick;



	//检测是否需要更新游戏
	if (!update) { return; }

}

void Game::GenerateOutput() {
	//检测是否需要重新绘制
	if (!update) { return; }

	//输出图像
	system("cls");//清屏

	//TEST
	std::cout << tickCount << std::endl;

	//重置更新
	update = false;
}

