#include "stdafx.h"
#include "GUIImage.h"

using namespace Resources;

GUIImage::GUIImage(GUISnap h, GUISnap v) : GUIObject()
{

	setSaveState(false);
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

	setSaveState(false);
	drawFrame = nullptr;
	this->frames = frames;
	this->totalFrames = totalFrames;
	currentFrame = 0;
	type = GUIObjects_IMAGE;
	hSnap = h;
	vSnap = v;

	if (totalFrames > 0)
	{

		CelestialMath::vectorUI2 imageSize = frames[0]->GetSize();
		float sizeX = (float)imageSize.x;
		float sizeY = (float)imageSize.y;
		unsigned char tempBuff[]{0,0,0,0,
		0,0,0,0};
		memcpy(tempBuff, &sizeX, 4);
		memcpy(&tempBuff[4], &sizeY, 4);
		Message mess;
		mess.destination = MessageSource_OBJECT;
		mess.type = MessageType_OBJECT;
		mess.mess = ObjectMess_SIZE;
		mess.SetParams(tempBuff, 0, 8);
		Update(&mess);

	}
}

unsigned char* GUIImage::Update(CrossHandlers::Message* mess)
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
			return GUIObject::Update(mess);

		}
	}

	return nullptr;

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

	if (frame->GetRenderImage() != nullptr)
	{

		killMessages[1] = new Message();
		unsigned char tempBuff[]{ drawFrame->GetImageId() >> 0, drawFrame->GetImageId() >> 8, 
			drawFrame->GetImageId() >> 16, drawFrame->GetImageId() >> 24 };
		killMessages[1]->SetParams(tempBuff, 0, 4);
		killMessages[1]->destination = MessageSource_GRAPHICS;
		killMessages[1]->type = MessageType_GRAPHICS;
		killMessages[1]->mess = GraphicMess_RESETIMAGE;
		killMessages[1]->read = false;

	}

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