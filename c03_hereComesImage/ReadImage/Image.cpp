#include "Tool.h"
#include "Image.h"

namespace GameLib
{
	Image::Image(const char* filename)
	{
		// ��ȡԴ�ļ�
		char* rawContent = ReadFile(filename);

		// ��ȡ����
		mHeight = GetUnsigned(&rawContent[12]);
		mWidth = GetUnsigned(&rawContent[16]);

		// ��ȡͼƬ���ݵ�����
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
		return 32;  // ÿ��Сͼռ 32 * 32 ������
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