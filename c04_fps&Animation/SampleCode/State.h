#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image; //只需要声明。不必包含它。

//状态类
class State {
public:
	State(const char* stageData, int size);
	~State();
	void Update(int dx, int dy);
	void Draw() const;
	bool HasCleared() const;
private:
	class Object;
	//网格绘制函数
	void SetSize(const char* stageData, int size);

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Image* mImage; //图片
	int mMoveCount;
};

#endif
