#include "stdafx.h"
#include "ColourLoader.h"

using namespace std;
using namespace Resources;
using namespace CrossHandlers;

MeshObject::Material* ColourLoader::LoadMaterial(int r, int g, int b, string name, float df, float af)
{

	MeshObject::Material* retVal = new MeshObject::Material(name);
	retVal->SetAmbient(card->CreateTexture((UCHAR)(r*df), (UCHAR)(g*df), (UCHAR)(b*df)));
	retVal->SetDiffuse(card->CreateTexture((UCHAR)(r*af), (UCHAR)(g*af), (UCHAR)(b*af)));
	return retVal;

}

TextureResourceObject* ColourLoader::LoadTextureFromColour(int* colours)
{

	return(card->CreateTexture(colours[0], colours[1], colours[2]));

}

void ColourLoader::Init(Graphics::CardHandler* &card)
{

	this->card = card;

}

ColourLoader::~ColourLoader()
{

}