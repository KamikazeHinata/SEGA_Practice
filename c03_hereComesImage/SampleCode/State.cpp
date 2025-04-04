#include "State.h"
#include "Image.h"

State::State(const char* stageData, int size) : mImage(0) 
{
	//尺寸测量
	SetSize(stageData, size);
	//数组分配
	mObjects.SetSize(mWidth, mHeight);
	mGoalFlags.SetSize(mWidth, mHeight);
	//填写初始值
	for (int y = 0; y < mHeight; ++y) 
	{
		for (int x = 0; x < mWidth; ++x) 
		{
			mObjects(x, y) = OBJ_WALL; //多余的部分是墙
			mGoalFlags(x, y) = false; //不是目标
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) 
	{
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) 
		{
			case '#': t = OBJ_WALL; break;
			case ' ': t = OBJ_SPACE; break;
			case 'o': t = OBJ_BLOCK; break;
			case 'O': t = OBJ_BLOCK; goalFlag = true; break;
			case '.': t = OBJ_SPACE; goalFlag = true; break;
			case 'p': t = OBJ_MAN; break;
			case 'P': t = OBJ_MAN; goalFlag = true; break;
			case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //换行处理
			default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) 
		{ 
			//这个if判断会忽略无法识别的字符
			mObjects(x, y) = t; //写入
			mGoalFlags(x, y) = goalFlag; //目标信息
			++x;
		}
	}
	//图片载入
	mImage = new Image("nimotsuKunImage2.dds");
}

State::~State() 
{
	delete mImage;
	mImage = 0;
}

void State::SetSize(const char* stageData, int size) 
{
	mWidth = mHeight = 0; //初始化
	//当前位置
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) 
	{
		switch (stageData[i]) 
		{
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			//最大值更新
			mWidth = (mWidth > x) ? mWidth : x;
			mHeight = (mHeight > y) ? mHeight : y;
			x = 0;
			break;
		}
	}
}

void State::Draw() const 
{
	for (int y = 0; y < mHeight; ++y) 
	{
		for (int x = 0; x < mWidth; ++x) 
		{
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);
			ImageID id = IMAGE_ID_SPACE;
			// 先绘制背景（地台）
			if (o != OBJ_WALL)
				DrawCell(x, y, goalFlag ? IMAGE_ID_GOAL : IMAGE_ID_SPACE);
			// 后绘制前景（人、墙、砖）
			switch (o)
			{
				case OBJ_WALL:	DrawCell(x, y, IMAGE_ID_WALL); break;
				case OBJ_BLOCK:	DrawCell(x, y, IMAGE_ID_BLOCK); break;
				case OBJ_MAN:	DrawCell(x, y, IMAGE_ID_PLAYER); break;
				default:		break;
			}
		}
	}
}

void State::DrawCell(int x, int y, ImageID id) const 
{
	mImage->Draw(x * 32, y * 32, id * 32, 0, 32, 32);
}


void State::Update(char input) 
{
	//移动差分变换
	int dx = 0;
	int dy = 0;
	switch (input) 
	{
		case 'a': dx = -1; break; //向左
		case 'd': dx = 1; break; //右
		case 'w': dy = -1; break; //上。Y朝下为正
		case 's': dy = 1; break; //下。
	}
	//使用简短的变量名。
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//查找人坐标
	int x = -1;
	int y = -1;
	bool found = false;
	for (y = 0; y < mHeight; ++y) 
	{
		for (x = 0; x < mWidth; ++x) 
		{
			if (o(x, y) == OBJ_MAN) 
			{
				found = true;
				break;
			}
		}
		if (found) 
		{
			break;
		}
	}
	//移动
	//运动后坐标
	int tx = x + dx;
	int ty = y + dy;
	//检查最大和最小坐标。
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) 
	{
		return;
	}
	//A.该方向上是空白或者终点。则小人移动
	if (o(tx, ty) == OBJ_SPACE) 
	{
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
	}
	//B.该方向是箱子。如果该方向上的下一个格子为空白或目标，则移动。
	else if (o(tx, ty) == OBJ_BLOCK) 
	{
		//2检查方格是否在范围内
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) 
		{ 
			//不能按
			return;
		}
		if (o(tx2, ty2) == OBJ_SPACE) 
		{
			//推箱子、移动人
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}

//如果该块至少有一个goalFlag为假
//尚未清除
bool State::HasCleared() const 
{
	for (int y = 0; y < mHeight; ++y) 
	{
		for (int x = 0; x < mWidth; ++x) 
		{
			if (mObjects(x, y) == OBJ_BLOCK) 
			{
				if (mGoalFlags(x, y) == false) 
				{
					return false;
				}
			}
		}
	}
	return true;
}
