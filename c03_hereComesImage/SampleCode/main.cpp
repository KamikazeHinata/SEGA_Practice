#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//函数原型
void MainLoop();

//全局变量
State* gState = 0;

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
	//获取输入
	cout << "a:left s:right w:up z:down. command?" << endl; //操作说明
	char input;
	cin >> input;
	//结束判断
	if (input == 'q') 
	{
		delete gState;
		gState = 0;
		Framework::instance().requestEnd();
		return;
	}
	//更新
	gState->Update(input);
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


