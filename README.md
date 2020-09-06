# CMDGameTemplate
C++控制台游戏基础模板

## Main.cpp

一个游戏程序的生命周期最少包括三个环节，准备运行环境，游戏循环，关闭前收尾工作

在游戏开始前，程序需要做好准备游戏的环境，比如创建窗口，配置程序环境变量，加载存档数据，初始化音频、视频、UI、物理引擎等子系统。
做完准备工作，游戏程序就可以开始运行了，在游戏中，处理玩家输入，显示图像，输出音频，更新游戏场景，更新相机。
最后，当程序关闭时，结束所有的任务，关闭子系统，释放资源


这篇文章主要关注一下游戏循环。从游戏循环框架来说，有三种，可以用一个，也可以混用

* 消息泵，按照一定的事件的顺序，依次进行处理
* 回调驱动，给你一个接口，自己实现，程序进行调用
* 事件驱动，例如C#里面的事件和委托



```C++
#include "Game.h"
int main() {
    Game game = Game();
    if (game.Initialize())
        game.Loop();
    game.Shutdown();
}
```
## Game.h
Game类Public的三个函数上面已经简单介绍过了

Private里面包含三个函数
* 处理案件输入，对于输入，我们可以简单的检测某个按键是否按下，或者按下了某个按键，来更新游戏。这种方式是存在问题的，稍后解释
* 更新游戏，游戏场景里面需要更新的东西很多，我们全部放到这里，按照一定的顺序进行更新
* 输出图像，显示玩家看到的一切


包含四个变量
* 游戏是否结束
* 游戏是否需要更新，因为我们这里只是一个简单的小例子，所以用这个变量来表示整个游戏是否需要更新，实际上我们可能在更多的地方用这样的布尔值来记录是否需要更新
* 从游戏开始到现在的累计的时间，实际上是上一帧的时间；此时此刻的时间。这个要放到后面再说。


```C++
#pragma once
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <stdio.h>
class Game {
public:
    //初始化
    bool Initialize();
    
    //游戏循环
    void Loop();
    
    //游戏结束
    void Shutdown();

private:
    //按键输入
    void ProcessInput();
    
    //更新游戏
    void UpdateGame();
    
    //输出图像
    void GenerateOutput();

    //游戏是否结束
    bool isRunning = true;

    //是否需要更新游戏
    bool update = true;

    //累计毫秒数，
    long tickCount, currentTick;
};
```

## Public
Initialize中初始化了记录时间的变量

Loop中，如果游戏状态为正在运行，那么就不停循环执行三个关键函数

```C++
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
```

## Private
#### 处理输入
这里我们使用_kbhit()函数去检测是否有按键被按下，然后再用_getch()函数去接受被按下的按键的ASCILL值，最后和按键A进行测试，如果被按下，则输出字符串

这样处理输入存在的问题很明显，当有多个按键同时被按下时，只能处理一个。虽然能力有限，但好在简单。
```C++
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
```
#### 更新游戏
```C++
void Game::UpdateGame()
{
    //计算时间步长
    while (clock() - tickCount < 16);
    currentTick = clock();
    float deltaTime = static_cast<float>((currentTick - tickCount)) / CLOCKS_PER_SEC;
    tickCount = currentTick;

    //TEST
    //update = true;

    //检测是否需要更新游戏
    if (!update) { return; }

}
```
#### 输出图像

```C++
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


```



## 相关函数


`clock()`


返回值：程序运行开始的毫秒数


返回值类型：clock_t，实际上clock_t就是long类型

```C++
_ACRTIMP clock_t __cdecl clock(void);

typedef long clock_t;
```
