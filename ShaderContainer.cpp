#include "stdafx.h"
#include "ShaderContainer.h"

using namespace Graphics;

ShaderContainer::ShaderContainer(ID3D11VertexShader* vertex,ID3D11GeometryShader* geometry,ID3D11PixelShader* pixel)
{

	vertexShader = vertex;
	geometryShader = geometry;
	pixelShader = pixel;

}

ID3D11VertexShader* ShaderContainer::GetVertexShader()
{

	return vertexShader;

}

ID3D11GeometryShader* ShaderContainer::GetGeometryShader()
{

	return geometryShader;

}

ID3D11PixelShader* ShaderContainer::GetPixelShader()
{

	return pixelShader;

}

ShaderContainer::~ShaderContainer()
{

}