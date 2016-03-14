#include "stdafx.h"
#include "GUIImage.h"

using namespace Resources;

GUIImage::GUIImage(GUISnap h, GUISnap v) : GUIObject()
{

	frames = nullptr;
	drawFrame = nullptr;
	totalFrames = 0;
	currentFrame = 0;
	type = GUIObjects_IMAGE;
	hSnap = h;
	vSnap = v;
	lastTime = 0;

}

GUIImage::GUIImage(ImageResourceObject** frames, unsigned int totalFrames, GUISnap h, GUISnap v) : GUIObject()
{

	drawFrame = nullptr;
	this->frames = frames;
	this->totalFrames = totalFrames;
	currentFrame = 0;
	type = GUIObjects_IMAGE;
	hSnap = h;
	vSnap = v;

}

void GUIImage::Update(CrossHandlers::Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		switch (mess->mess)
		{

		case ObjectMess_STEPFRAME:

			if (mess->timeSent - lastTime >= frames[currentFrame]->GetDelay())
			{

				currentFrame = (currentFrame + 1) % totalFrames;
				lastTime = mess->timeSent;

			}
			break;
		default:
			GUIObject::Update(mess);

		}
	}
}

ImageResourceObject* GUIImage::GetCurrentFrame() const
{

	if (frames != nullptr)
	{

		return frames[currentFrame];

	}

	return nullptr;

}

ImageResourceObject* GUIImage::GetDrawFrame()
{

	return drawFrame;

}

void GUIImage::SetDrawFrame(ImageResourceObject* frame)
{

	drawFrame = frame;

}

GUIImage::~GUIImage()
{

	if (frames != nullptr)
	{

		for (unsigned int i = 0; i < totalFrames; i++)
		{

			delete frames[i];

		}

		delete[] frames;

	}

	if (drawFrame != nullptr)
	{

		delete drawFrame;

	}
}