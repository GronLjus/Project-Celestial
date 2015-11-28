#include "stdafx.h"
#include "GUITextBoxLoader.h"
#include "GUITextBox.h"

using namespace Resources;
using namespace std;
using namespace CelestialMath;

GUITextBoxLoader::GUITextBoxLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "";

}

void GUITextBoxLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{
}

GUIObject* GUITextBoxLoader::Create(GUISnap hor, GUISnap ver)
{

	GUIObject* newObj = new GUITextBox(true, hor, ver);
	return newObj;

}

GUIObject* GUITextBoxLoader::Load(std::string text)
{

	return nullptr;

}

std::string* GUITextBoxLoader::Extension() const
{
	
	return extensions;

}

int GUITextBoxLoader::GetNrExtensions() const
{

	return extens;

}

GUITextBoxLoader::~GUITextBoxLoader()
{

	delete[] extensions;

}