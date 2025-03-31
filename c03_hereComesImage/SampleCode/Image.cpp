#include "Image.h"
#include "File.h"

#include "GameLib/Framework.h"
using namespace GameLib;

Image::Image(const char* filename) :
	mWidth(0),
	mHeight(0),
	mData(0) 
{
	File f(filename);
	mHeight = f.GetUnsigned(12);
	mWidth = f.GetUnsigned(16);
	mData = new unsigned[mWidth * mHeight];
	for (int i = 0; i < mWidth * mHeight; ++i) 
	{
		mData[i] = f.GetUnsigned(128 + i * 4);
	}
}

Image::~Image() 
{
	delete[] mData;
	mData = 0;
}

int Image::Width() const 
{
	return mWidth;
}

int Image::Height() const 
{
	return mHeight;
}

void Image::Draw(
	int dstX,
	int dstY,
	int srcX,
	int srcY,
	int width,
	int height) const 
{
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();

	for (int y = 0; y < height; ++y) 
	{
		for (int x = 0; x < width; ++x) 
		{
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
			*dst = mData[(y + srcY) * mWidth + (x + srcX)];
		}
	}
}
