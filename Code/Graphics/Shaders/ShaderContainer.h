#pragma once
#include "../GraphicAPI.h"

namespace Graphics
{

	///<summary>This class contains the shaders to use when drawing</summary>
	class ShaderContainer
	{

		public:
			///<param name='vertex'>[in]A pointer to the vertexshader to use</param>
			///<param name='geometry'>[in]A pointer to the geometryshader to use</param>
			///<param name='pixel'>[in]A pointer to the pixelshader to use</param>
			ShaderContainer(ID3D11VertexShader* vertex,ID3D11GeometryShader* geometry,ID3D11PixelShader* pixel);
			///<summary>Gets the vertexshader to use</summary>
			///<returns>A pointer to the internal vertexshader</returns>
			ID3D11VertexShader* GetVertexShader();
			///<summary>Gets the geometryshader to use</summary>
			///<returns>A pointer to the internal geometryshader</returns>
			ID3D11GeometryShader* GetGeometryShader();
			///<summary>Gets the pixelshader to use</summary>
			///<returns>A pointer to the internal pixelshader</returns>
			ID3D11PixelShader* GetPixelShader();
			~ShaderContainer();
			
		private:
			///<summary>A pointer to the vertexshader to use</summary>
			ID3D11VertexShader* vertexShader;
			///<summary>A pointer to the geometryshader to use</summary>
			ID3D11GeometryShader* geometryShader;
			///<summary>A pointer to the pixelshader to use</summary>
			ID3D11PixelShader* pixelShader;
	};
}