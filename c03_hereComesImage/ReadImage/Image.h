#pragma once
#include "GameLib/Framework.h"

namespace GameLib
{
	enum GAME_IMAGE
	{
		GAME_IMAGE_PLAYER,
		GAME_IMAGE_WALL,
		GAME_IMAGE_BLOCK,
		GAME_IMAGE_BLOCK_ON_GOAL,
		GAME_IMAGE_GOAL,
		GAME_IMAGE_PLAIN,
	};

	class Image
	{
	public:
		Image(const char*);
		~Image();
		unsigned Height() const;
		unsigned Width() const;
		unsigned BlockSize() const;
		const unsigned* Data() const;
		const unsigned* GetBlockBegCoord(GAME_IMAGE) const;

	private:
		unsigned mWidth;
		unsigned mHeight;
		unsigned* mData;
	};
}