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
	//��������
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
	//����ʱ�䲽��
	currentTick = static_cast<int>(clock());
	float deltaTime = (currentTick - tickCount) / 1000;
	tickCount = currentTick;

	//������Ҫ����
	update = true;

	//����Ƿ���Ҫ������Ϸ
	if (!update) { return; }

}

void Game::GenerateOutput() {
	//����Ƿ���Ҫ���»���
	if (!update) { return; }

	//���ͼ��
	system("cls");//����
	std::cout << tickCount ;

	//���ø���
	update = false;
}

