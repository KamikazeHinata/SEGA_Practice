#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//函数原型
void MainLoop();

//全局变量
State* gState = 0;
bool gPrevInputW = false;
bool gPrevInputA = false;
bool gPrevInputS = false;
bool gPrevInputD = false;

//用户封装函数。内容被抛出给mainLoop（）
namespace GameLib 
{
	void Framework::update() 
	{
		MainLoop();
	}
}

void MainLoop() 
{
	//×按钮被按下了吗？
	if (Framework::instance().isEndRequested()) 
	{
		if (gState) 
		{
			delete gState;
			gState = 0;
		}
		return;
	}
	//初始化第一帧。绘制第一个状态并完成。
	if (!gState) 
	{
		File file("stageData.txt");
		if (!(file.Data())) 
		{ 
			//没有数据！
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State(file.Data(), file.Size());
		//第一绘制
		gState->Draw();
		return; //结束
	}
	bool cleared = false;
	//主循环
	//清除检测
	if (gState->HasCleared()) 
	{
		cleared = true;
	}
	//结束判断
	Framework f = Framework::instance();
	if (f.isKeyOn('q'))
	{
		delete gState;
		gState = 0;
		Framework::instance().requestEnd();
		return;
	}
	//更新
	bool inputW = f.isKeyOn('w');
	bool inputA = f.isKeyOn('a');
	bool inputS = f.isKeyOn('s');
	bool inputD = f.isKeyOn('d');
	int dx = 0, dy = 0;
	if (!gPrevInputW && inputW) dy = -1;
	else if (!gPrevInputA && inputA) dx = -1;
	else if (!gPrevInputS && inputS) dy = 1;
	else if (!gPrevInputD && inputD) dx = 1;
	gPrevInputW = inputW;
	gPrevInputA = inputA;
	gPrevInputS = inputS;
	gPrevInputD = inputD;
	cout << "xD" << endl;
	gState->Update(dx, dy);
	//绘制
	gState->Draw();

	if (cleared) 
	{
		//庆祝消息
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
}


