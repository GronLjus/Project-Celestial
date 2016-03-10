#include "stdafx.h"
#include "TextContainer.h"

using namespace std;
using namespace CrossHandlers;

TextContainer::TextContainer()
{

	lines = 0;
	text = "";

}

void TextContainer::AddTextLine(std::string text)
{

	
	this->text += "\r\n" + text;
	lines++;

}

void TextContainer::AppendText(string text)
{

	this->text += text;
}

void TextContainer::SetText(string text)
{

	this->text = text;
}

void TextContainer::EraseText(unsigned int by)
{

	text = text.substr(0, text.length() - by);

}

string TextContainer::GetText() const
{


	return text;

}

void TextContainer::Clear()
{

	lines = 0;

}

TextContainer::~TextContainer()
{

}