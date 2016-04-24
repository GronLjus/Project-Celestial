#include "../../../../stdafx.h"
#include "GUIImageLoader.h"
#include "../../Objects/GUIObjects/GUIImage.h"

using namespace Resources;
using namespace std;
using namespace CelestialMath;
using namespace CrossHandlers;

GUIImageLoader::GUIImageLoader(ResourceLoader* rLoader)
{

	resourceLoader = rLoader;
	extens = 1;
	extensions = new string[extens];
	extensions[0] = "";

}

void GUIImageLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{
}

GUIObject* GUIImageLoader::Create(GUISnap hor, GUISnap ver, std::string file)
{

	GUIObject* newObj = nullptr;

	if (file != "")
	{

		unsigned int frames = 0;
		ImageResourceObject** images = resourceLoader->LoadBitMapesFromFile(file, frames);
		newObj = new GUIImage(images, frames, hor, ver);

	}
	else
	{

		newObj = new GUIImage(hor, ver);

	}

	return newObj;

}

GUIObject* GUIImageLoader::Load(std::string text)
{

	return nullptr;

}

std::string* GUIImageLoader::Extension() const
{

	return extensions;

}

int GUIImageLoader::GetNrExtensions() const
{

	return extens;

}

GUIImageLoader::~GUIImageLoader()
{

	delete[] extensions;

}