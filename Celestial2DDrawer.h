#include "I2DDrawer.h"
#include <dwrite.h>
#include <d2d1.h>

namespace Graphics
{

	///<summary>This class draws 2d objects to a texture</summary>
	class Celestial2DDrawer : public I2DDrawer
	{
		public:
			Celestial2DDrawer();
			virtual void Begin();
			virtual bool End();
			virtual HRESULT Init(CrossHandlers::TextureResourceObject* target, UINT width, UINT height);
			virtual HRESULT InitFont(int height,UINT width,FontWeight weight,UINT mipLevels,bool italic,LPCWSTR faceName,int &index);
			virtual HRESULT InitSolidBrush(float r, float g, float b, int &index);
			virtual bool DrawTextToTarget(std::wstring, float x, float y, int font, int brush);
			virtual void DrawGUIObject(Resources::GUIObject* object, CelestialMath::Vector2 parentalAbsPos, CelestialMath::Vector2 parentalAbsSize);
			virtual void DrawGUIObject(Resources::GUIObject* object);
			virtual void Clear();
			virtual void Release();
			~Celestial2DDrawer();

		private:
			///<summary>Extends the internal array containing the fonts</summary>
			///<param name='by'>[in]The amount to increase the array by</param>
			void extendFontArray(int by);
			///<summary>Extends the internal array containing the brushes</summary>
			///<param name='by'>[in]The amount to increase the array by</param>
			void extendBrushArray(int by);
			///<summary>Transforms the FontWeight into DX10FW</summary>
			///<param name='weight'>[in]The FontWeight to transform</param>
			///<returns>A value representing the FW-value</returns>
			DWRITE_FONT_WEIGHT transFormFW(FontWeight weight);

			///<summary>A array containing pointers to all fonts used</summary>
			IDWriteTextFormat** fonts;
			///<summary>The number of usable fonts in the array</summary>
			int numberOfFonts;
			///<summary>The total number of fonts the array can contain</summary>
			int maxFonts;

			///<summary>The factory to use when creating text</summary>
			IDWriteFactory* writeFactory;
			///<summary>The factory to use when building d2-objects</summary>
			ID2D1Factory* d2DFactory;
			///<summary>The rendertarget to use when drawing</summary>
			ID2D1RenderTarget* rT;

			///<summary>An array containing the brushes to use when drawing text</summary>
			ID2D1Brush** brushes;
			///<summary>The number of usable fonts in the array</summary>
			int numberOfBrushes;
			///<summary>The total number of fonts the array can contain</summary>
			int maxBrushes;

			///<summary>The width of the rendertarget</summary>
			UINT width;
			///<summary>The height of the rendertarget</summary>
			UINT height;
	};
}