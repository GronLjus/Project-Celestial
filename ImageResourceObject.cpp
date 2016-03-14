#include "stdafx.h"
#include "ImageResourceObject.h"

using namespace CrossHandlers;
using namespace CelestialMath;

ImageResourceObject::ImageResourceObject()
{

	image = nullptr;
	renderImage = nullptr;
	size = vectorUI2(0, 0);
	pos = vectorUI2(0, 0);
	delay = 0;

}

ImageResourceObject::ImageResourceObject(ID2D1Bitmap* image)
{

	renderImage = nullptr;
	this->image = image;
	size = vectorUI2(image->GetSize().width, image->GetSize().height);
	delay = 0;

}

unsigned int ImageResourceObject::GetDelay() const
{

	return delay;

}

void ImageResourceObject::SetDelay(unsigned int delay)
{

	this->delay = delay;

}

ID2D1Bitmap* ImageResourceObject::GetImage() const
{

	return image;

}

void ImageResourceObject::SetImage(ID2D1Bitmap* image)
{

	this->image = image;

}

ID2D1BitmapRenderTarget* ImageResourceObject::GetRenderImage() const
{

	return renderImage;

}

void ImageResourceObject::SetRenderImage(ID2D1BitmapRenderTarget* image)
{

	this->renderImage = image;

}

void ImageResourceObject::SetSize(vectorUI2 size)
{

	this->size = size;

}

void ImageResourceObject::SetPos(vectorUI2 pos)
{

	this->pos = pos;

}

vectorUI2 ImageResourceObject::GetSize() const
{

	return size;

}

vectorUI2 ImageResourceObject::GetPos() const
{

	return pos;

}

ImageResourceObject::~ImageResourceObject()
{

	if (image != nullptr)
	{

		image->Release();
		image = nullptr;

	}

	if (renderImage != nullptr)
	{

		renderImage->Release();
		renderImage = nullptr;

	}
}