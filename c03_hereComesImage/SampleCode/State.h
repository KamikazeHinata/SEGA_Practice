#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image; //ֻ��Ҫ���������ذ�������

//״̬��
class State {
public:
	State(const char* stageData, int size);
	~State();
	void Update(char input);
	void Draw() const;
	bool HasCleared() const;
private:
	enum Object 
	{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	//������ƺ���
	enum ImageID 
	{
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_BLOCK_ON_GOAL,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};
	void SetSize(const char* stageData, int size);
	void DrawCell(int x, int y, ImageID) const;

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Array2D< bool > mGoalFlags;
	Image* mImage; //ͼƬ
};

#endif
