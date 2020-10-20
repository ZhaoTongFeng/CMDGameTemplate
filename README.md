# CMDGameTemplate


首先推荐这本游戏开发入门书籍，理论和实践都有，从2D到3D游戏，用的SDL和OpenGL程序库。
```
C++游戏编程：创建3D游戏（Game Programming in C++:Creating 3D Games）Sanjay Madhav
```

我这个项目的意义在于梳理游戏程序的基本结构，然后可以在此基础上，开发一些简单的控制台小游戏，作为入门程序使用。
也有时候可能有一个想法或者逻辑，想快速进行编写和测试，那么不用打开什么游戏引擎，命令台就足够了

这个程序你也可以看成是一个命令台游戏引擎，在Github上，你只需要点击上面的`Use Template`，就可以拷贝到自己的存储库，可以在此基础上编写自己的版本


我自己编写了一些例子，可以作为使用参考，会不定时进行更新
* [俄罗斯方块](https://github.com/ZhaoTongFeng/Tetris-CMD)
* [BackJack/21点](https://github.com/ZhaoTongFeng/BlackJack-CMD)

## Main.cpp

一个游戏程序的生命周期大致来讲包括三个环节
* 准备运行环境
* 游戏循环
* 关闭前收尾工作

在游戏开始前，程序需要做好准备游戏的环境，比如创建窗口，配置程序环境变量，加载存档数据，初始化音频、视频、UI、物理引擎等子系统，以及初始化游戏场景

做完准备工作，游戏程序就进入了循环部分，一次循环可以称为一次迭代，在一次迭代中，程序要处理输入，更新状态，输出数据。

最后，当程序关闭时，停止响应输入，保存数据，卸载子系统，释放资源等等工作。

**
这模板主要就是提供一个游戏应该有的大致框架。**




## Main.h
将游戏抽象成一个Game类，在main函数中调用其启动，运行，关闭的函数。
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
Game类Public的三个函数就是在main函数中调用的三个，作用已经简单介绍过了

Private里面包含三个函数
* 处理按键输入，对于输入，我们可以简单的检测某个按键是否按下，或者按下了某个按键，来更新游戏。
* 更新游戏，游戏场景里面需要更新的东西很多，我们全部放到这里，按照一定的顺序进行更新
* 输出图像，显示玩家看到的一切


四个变量


**游戏程序普遍采用状态机的方式进行更新，比如update变量其实就是两个状态的状态机
在设计游戏程序时，一定要牢记状态机的概念，在最开始学习的时候可能体会不到，后面慢慢就会发现其重要性**

* 游戏是否结束
* 游戏是否需要更新，因为我们是靠控制台清屏并输出字符串的方法来输出，没有必要一直进行刷新
* 从游戏开始到现在的累计的时间，实际上次迭代时刻的时间，这个方便实现计时器功能


```C++
#pragma once
#include <time.h>
#include <conio.h>
#include <iostream>

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

Shutdown函数暂且为空，因为我们没有使用任何动态分配的变量，也没有做数据保存等游戏结束处理操作

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
使用_kbhit()函数去检测是否有按键被按下，然后再用_getch()函数去接收被按下的按键的ASCILL值。
下面的例子作用是，当按下A时，则在屏幕中输出字符串。

处理输入存在的问题很也很明显，当有多个按键同时被按下时，只能处理一个。
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
    //锁定60帧率
    while (clock() - tickCount < 16);

    //计算时间步长
    currentTick = clock();
    float deltaTime = static_cast<float>((currentTick - tickCount)) / CLOCKS_PER_SEC;
    tickCount = currentTick;

    //TEST
    //update = true;

    //检测是否需要更新游戏
    if (!update) { return; }

}
```





从游戏循环框架来说，有三种，可以用一个，也可以混用
* 消息泵，按照一定的事件的顺序，依次进行处理
* 回调驱动，给你一个接口，自己实现，程序进行调用
* 事件驱动，例如C#里面的事件和委托







###### 限制帧率

用上一帧的时刻和当前的时刻进行比较，
如果两帧之间时间间隔小于16毫秒（一秒钟60帧，1000/60=16.666），
就继续获取当前时刻，直到两帧之间时间间隔超过16毫秒，
最终的效果就是将帧率锁定在60帧以内。
但是实际显示帧率还取决于其他因素。

也可以去掉锁定帧数，程序在一秒钟内尽可能的更新

其实对于控制台来说，远远达不到60次刷新/秒的速度，
这里只是想说明，用当前时刻-上一次迭代的时刻就可以得出时间间隔，
控制两次刷新的时间间隔，就相当于控制刷新频率，
改成`while (clock() - tickCount < 1000);`
就相当于每隔1000毫秒刷新一次

###### 时间步长deltaTime


假设一颗子弹，从屏幕左侧水平飞到右侧，我们如何来更新子弹在屏幕中的位置？

上面说了限制帧率，假设现在程序就以60帧/s的速度进行更新，那么可以在每次迭代时，将子弹的位置进行偏移。可以实现移动的效果，但是当我们的程序以120帧/s的速度进行更新时，子弹就会以之前两倍的速度进行移动，这肯定是不能接受的

这个例子就和最初以CPU的时钟周期进行更新的例子类似，为了解决这个问题，提出了时间步长的概念

**整理下这个情景**

* 位置坐标系的X轴原点在屏幕左测中心，1像素代表1米
* pos(位置) = (0,0)
* screenWidth（屏幕宽度） = 1000像素
* v（移速） = 250米/s = 250像素/s
* offset(水平偏移值) = ?
* 每次迭代 pos.x+=offset

现在的问题就是计算每次迭代时的offset

答案：offset = deltaTime*v

```
float deltaTime = static_cast<float>((currentTick - tickCount)) / CLOCKS_PER_SEC;
60：delTime = 16/1000
120：delTime = 8/1000
```
按照理想情况，60帧下，两帧之间的时间间隔为16毫秒左右，120下为8毫秒左右
```
60： offset = 16 / 1000 * 250

120：offset =  8 / 1000 * 250
```
按照这个公式表示出两种情况下的offset = deltaTime*v

```
60： S = 16/1000 * 250 * 60  = 960/1000 * 250

120：S =  8/1000 * 250 * 120 = 960/1000 * 250
```
最后进行验算，两种帧率下，相同时间移动距离一致


这种用步长来进行运算的方法称为帧独立运算，也就是说，数据的变化和帧率没有关系，完全按照各自的时间步长进行计算





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

一般情况下，控制台不可能完全按照之前设定的帧率进行刷新，有时候为了降低刷新率，特地设置的这个update变量

输出的内容也有限，仅仅是将屏幕中的字符串清除再重写

这个过程和图形渲染库的方法是类似的，
不同的是，调用时所处的层级不同。
图像渲染库，从图像到显示器，中间有一个或者多个图形缓冲区，
首先要将图像流入图形适配器（GPU）的缓冲区，GPU再将缓冲区的内容流入显示器
cout输出，从字符串到显示器，同样cout有一个字符串缓冲区
先将字符串放到缓冲区，流入控制台缓冲区，控制台再传给操作系统，
获取字符图片，接着再用上面的方式渲染图片


大多数时候，图形渲染库不需要清除之前的内容，而是直接覆盖之前的内容，

###### 画面撕裂和垂直同步

有时候，循环迭代的次数太多，而显示器的绘制速度更不上，导致上一帧的图像残留，最终图像就会表现为撕裂。
当我们开启垂直同步，之后一般都会建立一个双图形缓冲区，程序将画面绘制到A，显示器读取B，然后再将A变成B，此时如果中间B还没绘制完成就等着，屏幕就一直显示这给画面，这就产生了卡顿现象

简单来说就是

* 不开垂直同步，有可能撕裂
* 开了垂直同步，有可能卡顿

当然这个问题不是三言两语就能解决的，这些问题再控制台程序中也体现不出来


## 相关函数
|  函数名   | 返回类型  |返回值  |库  |
|  :----:  | :----:  | :----:  |
| clock()  | clock_t|程序运行开始的毫秒数 |time.h|
| _kbhit()  | clock_t|非阻塞地响应键盘输入事件 |conio.h|
| _getch()  | clock_t|自动读取键盘输入无需回车 |conio.h|

###### clock原型
```C++
_ACRTIMP clock_t __cdecl clock(void);
typedef long clock_t;
```
实际上clock_t就是long类型


