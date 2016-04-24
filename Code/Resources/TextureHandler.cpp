#include "../../stdafx.h"
#include "TextureHandler.h"
#include "Loaders/FileLoaders/ImageFileLoader.h"
#include "Loaders/ColourLoader.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace std;

TextureHandler::TextureHandler()
{

	fileMatLoaders = 1;

	matFileLoaders = new IFileTextureLoader*[fileMatLoaders];
	matFileLoaders[0] = new ImageFileLoader();
	matColourLoader = new ColourLoader();

}

void TextureHandler::Init(Graphics::CardHandler* &card)
{

	for (int i = 0; i<fileMatLoaders; i++)
	{

		matFileLoaders[i]->Init(card,nullptr);

	}

	matColourLoader->Init(card);

}

string TextureHandler::getExtension(string file)
{

	string extension = "";
	bool stop = false;

	for (int i = file.length() - 1; !stop && i >= 0; i--)
	{

		stop = file[i] == '.';

		if (!stop)
		{

			extension += file[i];

		}
	}

	string temp = extension;

	for (UINT i = 0; i< extension.length(); i++)
	{

		extension[i] = temp[extension.length() - 1 - i];

	}

	return extension;

}

TextureResourceObject* TextureHandler::LoadTextureFromFile(std::string file)
{

	bool found = false;
	string extension = getExtension(file);
	int i = 0;

	for (i = 0; i<fileMatLoaders && !found; i++)
	{

		const string* extensions = matFileLoaders[i]->Extension();

		for (int k = 0; k<matFileLoaders[i]->GetNrExtensions() && !found; k++)
		{

			found = extensions[k] == extension;

		}
	}

	if (found)
	{

		return matFileLoaders[i-1]->LoadTexture(file);

	}

	return nullptr;

}

TextureResourceObject* TextureHandler::LoadTextureFromColour(int* colours)
{

	return matColourLoader->LoadTextureFromColour(colours);

}

TextureHandler::~TextureHandler()
{

	delete matColourLoader;

	for (int i = 0; i < fileMatLoaders; i++)
	{

		delete matFileLoaders[i];

	}

	delete[] matFileLoaders;

}