#include "stdafx.h"
#include "TextureResourceObject.h"

using namespace CrossHandlers;

TextureResourceObject::TextureResourceObject()
{

	dxt = nullptr;
	next = nullptr;

}

DXTextureResource* TextureResourceObject::GetDXT()
{

	return dxt;

}

void TextureResourceObject::SetDXT(DXTextureResource* dxt)
{

	if(this->dxt != nullptr)
	{

		delete this->dxt;

	}

	this->dxt = dxt;

}
TextureResourceObject* TextureResourceObject::GetNext()
{

	return next;

}

void TextureResourceObject::SetNext(TextureResourceObject* next)
{

	this->next = next;

}

TextureResourceObject::~TextureResourceObject()
{
	
	if(dxt != nullptr)
	{

		delete dxt;

	}

	if(next != nullptr)
	{

		delete next;

	}
}