#include "State.h"
#include "Image.h"

State::State(const char* stageData, int size) : mImage(0) 
{
	//�ߴ����
	SetSize(stageData, size);
	//�������
	mObjects.SetSize(mWidth, mHeight);
	mGoalFlags.SetSize(mWidth, mHeight);
	//��д��ʼֵ
	for (int y = 0; y < mHeight; ++y) 
	{
		for (int x = 0; x < mWidth; ++x) 
		{
			mObjects(x, y) = OBJ_WALL; //����Ĳ�����ǽ
			mGoalFlags(x, y) = false; //����Ŀ��
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
			case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���д���
			default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) 
		{ 
			//���if�жϻ�����޷�ʶ����ַ�
			mObjects(x, y) = t; //д��
			mGoalFlags(x, y) = goalFlag; //Ŀ����Ϣ
			++x;
		}
	}
	//ͼƬ����
	mImage = new Image("nimotsuKunImage.dds");
}

State::~State() 
{
	delete mImage;
	mImage = 0;
}

void State::SetSize(const char* stageData, int size) 
{
	mWidth = mHeight = 0; //��ʼ��
	//��ǰλ��
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
			//���ֵ����
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
			if (goalFlag) 
			{
				switch (o) 
				{
					case OBJ_SPACE: id = IMAGE_ID_GOAL; break;
					case OBJ_WALL: id = IMAGE_ID_WALL; break;
					case OBJ_BLOCK: id = IMAGE_ID_BLOCK_ON_GOAL; break;
					case OBJ_MAN: id = IMAGE_ID_PLAYER; break;
				}
			}
			else 
			{
				switch (o) 
				{
					case OBJ_SPACE: id = IMAGE_ID_SPACE; break;
					case OBJ_WALL: id = IMAGE_ID_WALL; break;
					case OBJ_BLOCK: id = IMAGE_ID_BLOCK; break;
					case OBJ_MAN: id = IMAGE_ID_PLAYER; break;
				}
			}
			DrawCell(x, y, id);
		}
	}
}

void State::DrawCell(int x, int y, ImageID id) const 
{
	mImage->Draw(x * 32, y * 32, id * 32, 0, 32, 32);
}


void State::Update(char input) 
{
	//�ƶ���ֱ任
	int dx = 0;
	int dy = 0;
	switch (input) 
	{
		case 'a': dx = -1; break; //����
		case 's': dx = 1; break; //��
		case 'w': dy = -1; break; //�ϡ�Y����Ϊ��
		case 'z': dy = 1; break; //�¡�
	}
	//ʹ�ü�̵ı�������
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//����������
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
	//�ƶ�
	//�˶�������
	int tx = x + dx;
	int ty = y + dy;
	//���������С���ꡣ
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) 
	{
		return;
	}
	//A.�÷������ǿհ׻����յ㡣��С���ƶ�
	if (o(tx, ty) == OBJ_SPACE) 
	{
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
	}
	//B.�÷��������ӡ�����÷����ϵ���һ������Ϊ�հ׻�Ŀ�꣬���ƶ���
	else if (o(tx, ty) == OBJ_BLOCK) 
	{
		//2��鷽���Ƿ��ڷ�Χ��
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) 
		{ 
			//���ܰ�
			return;
		}
		if (o(tx2, ty2) == OBJ_SPACE) 
		{
			//�����ӡ��ƶ���
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}

//����ÿ�������һ��goalFlagΪ��
//��δ���
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
