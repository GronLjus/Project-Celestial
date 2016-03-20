#include "stdafx.h"
#include "Celestial2DDrawer.h"
#include "CelestialMath.h"
#include "GUITextBox.h"
#include "GUILayout.h"
#include "GUIImage.h"

using namespace Graphics;
using namespace CelestialMath;
using namespace Resources;

std::wstring s2ws2(const std::string& s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;

}

Celestial2DDrawer::Celestial2DDrawer()
{

	maxFonts = 5;
	numberOfFonts = 0;
	maxBrushes = maxFonts;
	numberOfBrushes = 0;
	fonts = new IDWriteTextFormat*[maxFonts];
	brushes = new ID2D1Brush*[maxBrushes];
	
	writeFactory = nullptr;
	d2DFactory = nullptr;
	rT = nullptr;

	for(int i=0;i<maxFonts;i++)
	{

		fonts[i] = nullptr;
		brushes[i] = nullptr;

	}

	caretBlinkInterVal = 500;
	caretBlinkShow = 500;
	caretBlink = false;
	caretTime = 0;
}

void Celestial2DDrawer::Begin()
{

	rT->BeginDraw();
}

bool Celestial2DDrawer::End()
{

	HRESULT hr = rT->EndDraw();
	return hr == S_OK;

}

HRESULT Celestial2DDrawer::Init(CrossHandlers::TextureResourceObject* target, UINT width, UINT height)
{

	this->width = width;
	this->height = height;
	HRESULT hr = S_OK;


	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2DFactory);

	IDXGISurface* dxgiS;
	hr = target->GetDXT()->GetTexture()->QueryInterface(__uuidof(IDXGISurface), (LPVOID*)&dxgiS);

	FLOAT dpiX;
	FLOAT dpiY;
	d2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY);

	hr = d2DFactory->CreateDxgiSurfaceRenderTarget(dxgiS, &props, &rT);
	dxgiS->Release();

	return hr;

}

HRESULT Celestial2DDrawer::InitFont(int height, UINT width, FontWeight weight, UINT mipLevels, bool italic, LPCWSTR faceName, int &index)
{

	HRESULT hr = S_OK;

	if(numberOfFonts >= maxFonts)
	{

		extendFontArray(5);

	}

	DWRITE_FONT_WEIGHT fw = transFormFW(weight);
	index = numberOfFonts;

	 hr= writeFactory->CreateTextFormat(faceName, nullptr, fw, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, height, L"en-us", &fonts[numberOfFonts]);

	numberOfFonts++;
	return hr;

}

HRESULT Celestial2DDrawer::InitSolidBrush(float r, float g, float b, int &index)
{

	HRESULT hr = S_OK;

	if (numberOfBrushes >= maxBrushes)
	{

		extendBrushArray(5);

	}

	index = numberOfBrushes;
	ID2D1SolidColorBrush* temp;

	hr = rT->CreateSolidColorBrush(
		D2D1::ColorF(r,g,b),
		&temp
		);

	brushes[numberOfBrushes] = temp;
	numberOfBrushes++;
	return hr;

}

ImageResourceObject* Celestial2DDrawer::Load2DImage(unsigned char* values, UINT bPC, UINT channels, UINT width, UINT height)
{

	ID2D1Bitmap* out;
	D2D1_SIZE_U imageSize;
	D2D1_BITMAP_PROPERTIES bitProps;
	imageSize.height = height;
	imageSize.width = width;
	rT->GetDpi(&bitProps.dpiX, &bitProps.dpiY);
	bitProps.pixelFormat = rT->GetPixelFormat();

	rT->CreateBitmap(imageSize, values, bPC*channels*width, &bitProps, &out);
	return new ImageResourceObject(out);

}

void Celestial2DDrawer::SetBorderBrush(GUIObject* object, Vector3 color)
{

	int borderBrush = object->GetBorderBrush();

	if (borderBrush > 0)
	{

		((ID2D1SolidColorBrush*)brushes[borderBrush - 1])->SetColor(D2D1::ColorF(color.x, color.y, color.z));

	}
	else
	{

		InitSolidBrush(color.x, color.y, color.z, borderBrush);
		object->SetBorderBrush(borderBrush+1);

	}
}

void Celestial2DDrawer::SetContentBrush(GUIObject* object, Vector3 color)
{
	
	int contentBrush = object->GetContentBrush();

	if (contentBrush > 0)
	{

		((ID2D1SolidColorBrush*)brushes[contentBrush - 1])->SetColor(D2D1::ColorF(color.x, color.y, color.z));

	}
	else
	{

		InitSolidBrush(color.x, color.y, color.z, contentBrush);
		object->SetContentBrush(contentBrush+1);

	}
}

void Celestial2DDrawer::DrawGUIObject(Resources::GUIObject* object, unsigned int time)
{

	DrawGUIObject(object, Vector2(0, 0), Vector2(width, height), time);

}

void Celestial2DDrawer::DrawGUIObject(Resources::GUIObject* object, Vector2 parentalAbsPos, Vector2 parentalAbsSize, unsigned int time)
{

	if (object == nullptr)
	{

		return;

	}

	if (object->IsVisible())
	{

		Vector2 objSize = Vector2(object->GetScale().x, object->GetScale().y);
		Vector2 topLeft = Vector2(
			object->GetHorizontalSnap() == GUISnap_LEFT ? object->GetPosition().x : object->GetHorizontalSnap() == GUISnap_MIDDLE ? 0.5f - objSize.x*0.5f : objSize.x - object->GetPosition().x,
			object->GetVerticalSnap() == GUISnap_TOP ? object->GetPosition().y : object->GetVerticalSnap() == GUISnap_MIDDLE ? 0.5f - objSize.y*0.5f : objSize.y - object->GetPosition().y);

		Vector2 topLeftAbs = parentalAbsPos + topLeft * parentalAbsSize;
		Vector2 abSize = objSize * parentalAbsSize;

		D2D1_RECT_F layoutRect = D2D1::RectF(
			topLeftAbs.x,
			topLeftAbs.y,
			topLeftAbs.x + abSize.x,
			topLeftAbs.y + abSize.y
			);


		D2D1::Matrix3x2F wM;
		rT->GetTransform(&wM);
		D2D1_LAYER_PARAMETERS layPar = D2D1_LAYER_PARAMETERS();
		layPar.contentBounds = layoutRect;
		layPar.maskTransform = D2D1::Matrix3x2F::Identity();
		rT->PushAxisAlignedClip(
			layoutRect,
			D2D1_ANTIALIAS_MODE_ALIASED
			);

		ID2D1Brush* contentBrush = object->GetContentBrush() > 0 ? brushes[object->GetContentBrush() - 1] : nullptr;
		ID2D1Brush* borderBrush = object->GetBorderBrush() > 0 ? brushes[object->GetBorderBrush() - 1] : nullptr;

		if (borderBrush != nullptr)
		{

			rT->DrawRectangle(layoutRect, borderBrush);

		}

		if (object->GetType() == GUIObjects_TEXTBOX && contentBrush != nullptr)
		{

			GUITextBox* tB = (GUITextBox*)object;
			std::string superString = tB->GetText()->GetText();

			IDWriteTextLayout* boxLayout = nullptr;
			
			DWRITE_TEXT_METRICS metrics;
			writeFactory->CreateTextLayout(s2ws2(superString).c_str(), superString.length(), fonts[0], layoutRect.right - layoutRect.left, layoutRect.bottom - layoutRect.top, &boxLayout);
			boxLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
			boxLayout->GetMetrics(&metrics);
			D2D1_POINT_2F textTop = D2D1_POINT_2F();

			textTop.x = layoutRect.left;
			textTop.y = layoutRect.top - max(0, metrics.height - metrics.layoutHeight);

			rT->DrawTextLayout(textTop, boxLayout, contentBrush);

			if (tB->GetFocus())
			{

				caretTime += time - caretLastTime;
				caretLastTime = time;

				if (caretBlink && caretTime < caretBlinkShow)
				{

					D2D1_POINT_2F caretTop;
					DWRITE_HIT_TEST_METRICS metr;
					boxLayout->HitTestTextPosition(tB->GetCaretPosition(), false, &(caretTop.x), &(caretTop.y), &metr);
					caretTop.x += textTop.x;
					caretTop.y += textTop.y;

					D2D1_POINT_2F caretBottom = caretTop;
					caretBottom.y += metr.height;

					rT->DrawLine(caretTop, caretBottom, contentBrush, 1.5f);

				}
				else if (caretBlink && caretTime >= caretBlinkShow || (!caretBlink && caretTime >= caretBlinkInterVal))
				{

					caretBlink = !caretBlink;
					caretTime = 0;

				}
			}

			boxLayout->Release();

		}
		else if (object->GetType() == GUIObjects_IMAGE)
		{

			GUIImage* image = (GUIImage*)object;

			ImageResourceObject* drawArea = image->GetDrawFrame();

			if (drawArea == nullptr)
			{

				drawArea = new ImageResourceObject();
				drawArea->SetSize(vectorUI2(image->GetCurrentFrame()->GetSize().x, image->GetCurrentFrame()->GetSize().y));
				ID2D1BitmapRenderTarget* renderTarget;
				rT->CreateCompatibleRenderTarget(&renderTarget);

				drawArea->SetRenderImage(renderTarget);
				image->SetDrawFrame(drawArea);
				
			}

			vectorUI2 subPos = image->GetCurrentFrame()->GetPos();
			vectorUI2 subSize = image->GetCurrentFrame()->GetSize();

			D2D1_RECT_F subImageRect = D2D1::RectF(
				subPos.x,
				subPos.y,
				subPos.x + subSize.x,
				subPos.y + subSize.y
				);

			drawArea->GetRenderImage()->BeginDraw();
			drawArea->GetRenderImage()->DrawBitmap(image->GetCurrentFrame()->GetImage(), subImageRect);
			drawArea->GetRenderImage()->EndDraw();

			ID2D1Bitmap* composite;
			drawArea->GetRenderImage()->GetBitmap(&composite);
			rT->DrawBitmap(composite, layoutRect);
			composite->Release();

			Message mess;
			mess.destination = MessageSource_OBJECT;
			mess.mess = ObjectMess_STEPFRAME;
			mess.read = false;
			mess.timeSent = time;
			mess.type = MessageType_OBJECT;
			image->Update(&mess);

		}
		else if (object->GetType() == GUIObjects_LAYOUT)
		{

			GUILayout* layout = (GUILayout*)object;


			if (contentBrush != nullptr)
			{

				rT->FillRectangle(layoutRect, contentBrush);

			}

			layout->ShouldPause();

			for (int i = 0; i < layout->GetChildren(); i++)
			{

				if (layout->GetChild(i) != nullptr)
				{

					DrawGUIObject(layout->GetChild(i), topLeftAbs, abSize, time);

				}
			}
		}

		rT->PopAxisAlignedClip();

	}
}

void Celestial2DDrawer::Clear()
{
	D2D1_COLOR_F col = D2D1_COLOR_F();

	rT->Clear(col);

}

void Celestial2DDrawer::Release()
{

	for(int i=0;i<numberOfFonts;i++)
	{

		fonts[i]->Release();

	}

	for (int i = 0; i<numberOfBrushes; i++)
	{

		brushes[i]->Release();

	}

	if (rT != nullptr)
	{

		rT->Release();

	}

	writeFactory->Release();
	d2DFactory->Release();

}

void Celestial2DDrawer::extendFontArray(int by)
{

	maxFonts += by;
	IDWriteTextFormat** tempFonts = new IDWriteTextFormat*[maxFonts];

	for(int i=0;i<maxFonts-by;i++)
	{

		tempFonts[i] = fonts[i];

	}

	delete[] fonts;
	fonts = tempFonts;

}

void Celestial2DDrawer::extendBrushArray(int by)
{

	maxBrushes += by;
	ID2D1Brush** tempBrushes = new ID2D1Brush*[maxFonts];

	for (int i = 0; i<maxBrushes - by; i++)
	{

		tempBrushes[i] = brushes[i];

	}

	delete[] brushes;
	brushes = tempBrushes;

}

DWRITE_FONT_WEIGHT Celestial2DDrawer::transFormFW(FontWeight weight)
{

	switch(weight)
	{

	case FontWeight_BOLD: return DWRITE_FONT_WEIGHT_BOLD;
	case FontWeight_EXTRABOLD: return DWRITE_FONT_WEIGHT_EXTRA_BOLD;
	case FontWeight_EXTRALIGHT: return DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
	case FontWeight_HEAVY: return DWRITE_FONT_WEIGHT_HEAVY;
	case FontWeight_LIGHT: return DWRITE_FONT_WEIGHT_LIGHT;
	case FontWeight_MEDIUM: return DWRITE_FONT_WEIGHT_MEDIUM;
	case FontWeight_SEMIBOLD: return DWRITE_FONT_WEIGHT_SEMI_BOLD;
	case FontWeight_THIN: return DWRITE_FONT_WEIGHT_THIN;
	default: return DWRITE_FONT_WEIGHT_NORMAL;

	}
}

Celestial2DDrawer::~Celestial2DDrawer()
{
	
	delete[] fonts;
	delete[] brushes;
}