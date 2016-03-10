#include "stdafx.h"
#include "GUILayoutLoader.h"
#include "GUILayout.h"

using namespace Resources;
using namespace std;
using namespace CelestialMath;

GUILayoutLoader::GUILayoutLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "";

}

void GUILayoutLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{
}

GUIObject* GUILayoutLoader::Create(GUISnap hor, GUISnap ver)
{

	GUIObject* newObj = new GUILayout(hor,ver);
	return newObj;

}

GUIObject* GUILayoutLoader::Load(std::string text)
{

	return nullptr;

}

std::string* GUILayoutLoader::Extension() const
{

	return extensions;

}

int GUILayoutLoader::GetNrExtensions() const
{

	return extens;

}

GUILayoutLoader::~GUILayoutLoader()
{

	delete[] extensions;

}