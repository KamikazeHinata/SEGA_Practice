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
			unsigned* src = &mData[(y + srcY) * mWidth + (x + srcX)];
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
			
			// alpha blend
			unsigned srcA = (*src & 0xff000000) >> 24;

			unsigned srcR = *src & 0x00ff0000;
			unsigned srcG = *src & 0x0000ff00;
			unsigned srcB = *src & 0x000000ff;

			unsigned dstR = *dst & 0x00ff0000;
			unsigned dstG = *dst & 0x0000ff00;
			unsigned dstB = *dst & 0x000000ff;

			unsigned r = (srcR - dstR) * srcA / 255 + dstR;
			unsigned g = (srcG - dstG) * srcA / 255 + dstG;
			unsigned b = (srcB - dstB) * srcA / 255 + dstB;

			*dst = (r & 0xff0000) | (g & 0x00ff00) | b;
		}
	}
}
