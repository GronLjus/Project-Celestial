#include "stdafx.h"
#include "KubLingSource.h"
using namespace Resources;

KubLingSource::KubLingSource(int totalLines, std::string** lines)
{

	this->totalLines = totalLines;
	this->lines = lines;

}

std::string KubLingSource::GetLine(int line)
{

	if (line < totalLines)
	{

		return lines[line][lineValue];

	}
	
	return "";

}

std::string KubLingSource::GetFile(int line)
{

	if (line < totalLines)
	{

		return lines[line][lineSource];

	}

	return "";

}

int KubLingSource::GetLines()
{

	return totalLines;

}

int KubLingSource::GetId() const
{

	return id;

}

void KubLingSource::SetId(int id)
{

	this->id = id;

}

KubLingSource::~KubLingSource()
{
	
	for (unsigned int i = 0; i < totalLines; i++)
	{

		delete[] lines[i];

	}

	delete[] lines;

}