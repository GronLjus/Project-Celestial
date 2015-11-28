#include "stdafx.h"
#include "ShaderContainer.h"

using namespace Graphics;

ShaderContainer::ShaderContainer(ID3D10VertexShader* vertex,ID3D10GeometryShader* geometry,ID3D10PixelShader* pixel)
{

	vertexShader = vertex;
	geometryShader = geometry;
	pixelShader = pixel;

}

ID3D10VertexShader* ShaderContainer::GetVertexShader()
{

	return vertexShader;

}

ID3D10GeometryShader* ShaderContainer::GetGeometryShader()
{

	return geometryShader;

}

ID3D10PixelShader* ShaderContainer::GetPixelShader()
{

	return pixelShader;

}

ShaderContainer::~ShaderContainer()
{

}