#include "stdafx.h"
#include "TextContainer.h"

using namespace std;
using namespace CrossHandlers;

TextContainer::TextContainer(int maxLines)
{

	this->maxLines = maxLines;
	textLines = new string[this->maxLines];
	lines = 0;

}

void TextContainer::AddText(std::string text)
{

	if (lines >= maxLines)
	{

		maxLines += 20;
		string* temp = new string[maxLines];

		for (int i = 0; i < lines; i++)
		{

			temp[i] = textLines[i];

		}

		string* toDelete = textLines;
		textLines = temp;
		delete[] toDelete;

	}

	textLines[lines] = text;
	lines++;

}

void TextContainer::AppendText(string text)
{

	if (lines > 0)
	{

		textLines[lines - 1] +=  text;

	}
}

void TextContainer::EraseText(unsigned int by)
{

	if (lines > 0)
	{

		textLines[lines - 1] = textLines[lines-1].substr(0,textLines[lines-1].length()-by);

	}
}

string TextContainer::GetText(int line)
{

	string retVal = "";

	if (line < lines)
	{

		retVal = textLines[line];
		
	}

	return retVal;

}

void TextContainer::Clear()
{

	lines = 0;

}

TextContainer::~TextContainer()
{

	delete[] textLines;

}