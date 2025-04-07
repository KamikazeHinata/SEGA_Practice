#include "State.h"
#include "Image.h"
#include <iostream>
#include "../../c02_startWithPixel/2DGraphics1/include/GameLib/Base/DebugStream.h"

class State::Object
{
public:
	enum Type
	{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	enum ImageID
	{
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};
	// 默认构造函数
	Object() : mType(OBJ_WALL), mGoalFlag(false), mMoveX(0), mMoveY(0) {};
	// 通过场景数据的字符来初始化
	void Set(char c)
	{
		switch (c)
		{
			case '#': mType = OBJ_WALL; break;
			case ' ': mType = OBJ_SPACE; break;
			case 'o': mType = OBJ_BLOCK; break;
			case 'O': mType = OBJ_BLOCK; mGoalFlag = true; break;
			case '.': mType = OBJ_SPACE; mGoalFlag = true; break;
			case 'p': mType = OBJ_MAN; break;
			case 'P': mType = OBJ_MAN; mGoalFlag = true; break;
		}
	}
	// 绘制：背景、前景
	void DrawBackground(int x, int y, const Image* image) const
	{
		if (mType == OBJ_WALL)
		{
			DrawCell(x, y, IMAGE_ID_WALL, image);
		}
		else
		{
			if (mGoalFlag)
			{
				DrawCell(x, y, IMAGE_ID_GOAL, image);
			}
			else
			{
				DrawCell(x, y, IMAGE_ID_SPACE, image);
			}
		}
	}
	void DrawForeground(int x, int y, const Image* image, int moveCount) const
	{
		// 人、砖块
		ImageID id = IMAGE_ID_SPACE;
		if (mType == OBJ_BLOCK)
			id = IMAGE_ID_BLOCK;
		else if (mType == OBJ_MAN)
			id = IMAGE_ID_PLAYER;

		if (id != IMAGE_ID_SPACE)
		{
			int dx = mMoveX * (32 - moveCount);
			int dy = mMoveY * (32 - moveCount);
			image->Draw(x * 32 - dx, y * 32 - dy, id * 32, 0, 32, 32);
		}
	}
	static void DrawCell(int x, int y, ImageID id, const Image* image)
	{
		image->Draw(32*x, 32*y, id*32, 0, 32, 32);
	}
	// 移动
	void Move(int dx, int dy, Type replaced)
	{
		mMoveX = dx;
		mMoveY = dy;
		mType = replaced;
	}

	Type mType;
	bool mGoalFlag;
	// 从哪个方向来
	int mMoveX;
	int mMoveY;
};

State::State(const char* stageData, int size) : mImage(0), mMoveCount(0)
{
	//尺寸测量
	SetSize(stageData, size);
	//数组分配
	mObjects.SetSize(mWidth, mHeight);
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i)
	{
		Object t;
		bool goalFlag = false;
		switch (stageData[i])
		{
			case '#': case ' ': case 'o': case 'O':
			case '.': case 'p': case 'P':
				mObjects(x, y).Set(stageData[i]);
				++x;
				break;
			case '\n': 
				x = 0; 
				++y; 
				break;
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
	// 先绘制背景（地台）
	for (int y = 0; y < mHeight; ++y) 
	{
		for (int x = 0; x < mWidth; ++x) 
		{
			mObjects(x, y).DrawBackground(x, y, mImage);
		}
	}
	// 后绘制前景（人、墙、砖）
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			mObjects(x, y).DrawForeground(x, y, mImage, mMoveCount);
		}
	}
}

void State::Update(int dx, int dy) 
{
	// 如果移动计数达到32
	if (mMoveCount == 32)
	{
		mMoveCount = 0;
		for (int y = 0; y < mHeight; ++y)
		{
			for (int x = 0; x < mWidth; ++x)
			{
				mObjects(x, y).mMoveX = 0;
				mObjects(x, y).mMoveY = 0;
			}
		}
	}
	// 移动时不更新
	if (mMoveCount > 0)
	{
		++mMoveCount;
		return;
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
			if (o(x, y).mType == Object::OBJ_MAN) 
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
	if (o(tx, ty).mType == Object::OBJ_SPACE)
	{
		o(tx, ty).Move(dx, dy, Object::OBJ_MAN);
		o(x, y).Move(dx, dy, Object::OBJ_SPACE);
		mMoveCount = 1;
	}
	//B.该方向是箱子。如果该方向上的下一个格子为空白或目标，则移动。
	else if (o(tx, ty).mType == Object::OBJ_BLOCK)
	{
		//2检查方格是否在范围内
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) 
		{ 
			//不能按
			return;
		}
		if (o(tx2, ty2).mType == Object::OBJ_SPACE) 
		{
			//推箱子、移动人
			o(tx2, ty2).Move(dx, dy, Object::OBJ_BLOCK);
			o(tx, ty).Move(dx, dy, Object::OBJ_MAN);
			o(x, y).Move(dx, dy, Object::OBJ_SPACE);
			mMoveCount = 1;
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
			if (mObjects(x, y).mType == Object::OBJ_BLOCK)
			{
				if (mObjects(x, y).mGoalFlag == false)
				{
					return false;
				}
			}
		}
	}
	return true;
}
