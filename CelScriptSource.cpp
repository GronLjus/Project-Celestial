#include "stdafx.h"
#include "CelScriptSource.h"
using namespace Resources;

CelScriptSource::CelScriptSource(int totalLines, std::string** lines)
{

	this->totalLines = totalLines;
	this->lines = lines;

}

std::string CelScriptSource::GetLine(int line)
{

	if (line < totalLines)
	{

		return lines[line][lineValue];

	}
	
	return "";

}

std::string CelScriptSource::GetFile(int line)
{

	if (line < totalLines)
	{

		return lines[line][lineSource];

	}

	return "";

}

int CelScriptSource::GetLines()
{

	return totalLines;

}

int CelScriptSource::GetId() const
{

	return id;

}

void CelScriptSource::SetId(int id)
{

	this->id = id;

}

CelScriptSource::~CelScriptSource()
{
	
	for (unsigned int i = 0; i < totalLines; i++)
	{

		delete[] lines[i];

	}

	delete[] lines;

}