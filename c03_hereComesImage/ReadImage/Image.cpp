#include "Tool.h"
#include "Image.h"

namespace GameLib
{
	Image::Image(const char* filename)
	{
		// 读取源文件
		char* rawContent = ReadFile(filename);

		// 获取宽、高
		mHeight = GetUnsigned(&rawContent[12]);
		mWidth = GetUnsigned(&rawContent[16]);

		// 读取图片数据到缓存
		mData = new unsigned[mWidth * mHeight]();
		for (int y = 0; y < mHeight; ++y)
		{
			for (int x = 0; x < mWidth; ++x)
			{
				int pixelIdx = y * mWidth + x;
				mData[pixelIdx] = GetUnsigned(&rawContent[128 + 4 * pixelIdx]);
			}
		}
	}

	Image::~Image()
	{
		delete[] mData;
		mData = 0;
	}

	unsigned Image::Height() const
	{
		return mHeight;
	}

	unsigned Image::Width() const
	{
		return mWidth;
	}

	unsigned Image::BlockSize() const
	{
		return 32;  // 每种小图占 32 * 32 个像素
	}

	const unsigned* Image::Data() const
	{
		return mData;
	}

	const unsigned* Image::GetBlockBegCoord(GAME_IMAGE type) const
	{
		return &Data()[type * BlockSize()];
	}
}