#pragma once
#include <d2d1.h>
#include "../../CelestialMath/CelestialMath.h"

namespace CrossHandlers
{

	class ImageResourceObject
	{

		private:
			ID2D1Bitmap* image;
			ID2D1BitmapRenderTarget* renderImage;

			CelestialMath::vectorUI2 size;
			CelestialMath::vectorUI2 pos;
			unsigned int delay;

		public:
			ImageResourceObject();
			ImageResourceObject(ID2D1Bitmap* image);

			unsigned int GetDelay() const;
			void SetDelay(unsigned int delay);

			ID2D1Bitmap* GetImage() const;
			void SetImage(ID2D1Bitmap* image);


			ID2D1BitmapRenderTarget* GetRenderImage() const;
			void SetRenderImage(ID2D1BitmapRenderTarget* image);

			void SetSize(CelestialMath::vectorUI2 size);
			void SetPos(CelestialMath::vectorUI2 pos);

			CelestialMath::vectorUI2 GetSize() const;
			CelestialMath::vectorUI2 GetPos() const;

			~ImageResourceObject();

	};
}