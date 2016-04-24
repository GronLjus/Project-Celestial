#include "../../stdafx.h"
#include "TextContainer.h"

using namespace std;
using namespace CrossHandlers;

TextContainer::TextContainer()
{

	lines = 0;
	text = "";
	caretPos = 0;
	caretLine = 0; 
	caretChar = 0;

	maxLines = 10;
	chars = new unsigned int[maxLines];

	for (unsigned int i = 0; i < maxLines; i++)
	{

		chars[i] = 0;

	}
}

void TextContainer::AddTextLine(std::string text)
{

	if (this->text.length() == 0)
	{

		this->text = text;
		caretPos += text.size();
		caretChar = 0;
		chars[caretLine] += text.length();
		return;

	}

	this->text.insert(caretPos, "\n" + text);

	caretPos += text.size() + 1;
	chars[caretLine]++;

	if (caretLine == lines)
	{

		lines++;

		if (lines >= maxLines)
		{

			maxLines += 10;
			unsigned int* newChars = new unsigned int[maxLines];

			for (unsigned int i = 1; i <= 10; i++)
			{

				newChars[maxLines - i] = 0;

			}

			memcpy(newChars, chars, lines*sizeof(unsigned int));
			delete[] chars;
			chars = newChars;

		}
	}

	caretLine++;
	caretChar = text.length();
	chars[caretLine] += text.length();

}

void TextContainer::AppendText(string text)
{

	this->text.insert(caretPos, text);

	caretPos += text.size();
	caretChar += text.size();
	chars[caretLine] += text.length();

}

void TextContainer::AppendText(char text)
{

	this->text.insert(caretPos, &text, 1);

	caretPos++;
	caretChar++;
	chars[caretLine]++;

}

void TextContainer::SetText(string text)
{

	this->text = text;
	lines = 0;
	caretLine = 0;

	chars[caretLine] = text.length();
	caretChar = text.length();
	caretPos = text.length();


}

void TextContainer::EraseText(unsigned int by)
{

	string removedText = text.substr(caretPos - by, by);
	unsigned int remove = by;

	while (chars[caretLine] < remove)
	{

		remove -= chars[caretLine];
		chars[caretLine] = 0;
		caretLine--;
		lines--;
		caretChar = chars[caretLine];

	}

	text = text.substr(0, text.length() - by);
	caretPos -= by;
	caretChar -= by;
	chars[caretLine] -= by;

}

unsigned int TextContainer::GetCaretPos() const
{

	return caretPos;

}

void TextContainer::MoveCaret(int rows, int columns)
{

	if (rows != 0)
	{

		if (rows > 0 && caretLine < lines)
		{

			caretPos += chars[caretLine] - caretChar;

			for (int i = 1; i < columns-1 && caretLine+i < lines; i++)
			{

				caretPos += chars[caretLine + i];
				caretLine++;

			}

			caretLine++;
			caretChar = chars[caretLine] >= caretChar ? caretChar : caretLine == lines ? chars[caretLine] :chars[caretLine] - 1;
			caretPos += caretChar;

		}
		else if (rows < 0 && caretLine > 0)
		{

			caretPos -= caretChar;

			for (int i = 1; i < abs(rows) - 1 && caretLine - i > 0; i++)
			{

				caretPos -= chars[caretLine - i];
				caretLine--;

			}

			caretLine--;
			caretChar = chars[caretLine] >= caretChar ? caretChar : chars[caretLine]-1;
			caretPos -= chars[caretLine] - caretChar;

		}

	}

	if (columns != 0)
	{

		if (columns > 0)
		{

			caretPos = caretPos + columns > text.length() ? text.length() : caretPos + columns;
			unsigned int toAdd = columns;

			while (caretChar + toAdd > chars[caretLine] && caretLine < lines)
			{

				toAdd -= chars[caretLine] - caretChar;
				caretLine++;
				caretChar = 0;

			}

			caretChar += caretChar + toAdd < chars[caretLine] ? toAdd : chars[caretLine] - caretChar;

		}
		else
		{

			caretPos = caretPos + columns < 0 ? 0 : caretPos + columns;
			int toRemove = abs(columns);

			while (caretChar < toRemove && caretLine > 0)
			{

				toRemove -= caretChar;
				caretLine--;
				caretChar = chars[caretLine];

			}

			caretChar -= caretChar > toRemove ? toRemove : caretChar;

		}
	}
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

	delete[] chars;

}