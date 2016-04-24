#include "../../../../stdafx.h"
#include "ImageFileLoader.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace std;

unsigned char* ImageFileLoader::getImage(std::string file, unsigned int &width, unsigned int &height) const
{

	int len;
	int slength = (int)file.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, file.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, file.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	IWICBitmapDecoder* imDecoder;
	IWICBitmapFrameDecode* frameDecoder;
	IWICBitmap* imageBitmap;
	IWICFormatConverter* imConverter;

	imFac->CreateFormatConverter(&imConverter);
	imFac->CreateDecoderFromFilename(r.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &imDecoder);
	imDecoder->GetFrame(0, &frameDecoder);
	frameDecoder->GetSize(&width, &height);
	imFac->CreateBitmapFromSource(frameDecoder, WICBitmapCacheOnDemand, &imageBitmap);
	imConverter->Initialize(imageBitmap, GUID_WICPixelFormat32bppPRGBA,WICBitmapDitherTypeNone, nullptr, 0.0f,WICBitmapPaletteTypeMedianCut);

	UINT imageStride = width * 4;
	UINT imageSize = height*imageStride;
	byte* imageBuffer = new byte[imageSize];
	imConverter->CopyPixels(nullptr, imageStride, imageSize, imageBuffer);
	imConverter->Release();
	imageBitmap->Release();
	frameDecoder->Release();
	imDecoder->Release();
	return imageBuffer;

}

MeshObject::Material** ImageFileLoader::LoadMaterial(std::string file, int &size)
{

	size = 1;
	UINT height = 0;
	UINT width = 0;
	unsigned char* imageBuffer = getImage(file, width, height);

	MeshObject::Material** retVal = new MeshObject::Material*[size];
	retVal[0] = new MeshObject::Material(file);

	retVal[0]->SetDiffuse(card->LoadTexture(imageBuffer,8,4,"rgba", width, height));
	retVal[0]->SetAmbient(card->LoadTexture(imageBuffer, 8, 4, "rgba", width, height));

	delete[] imageBuffer;
	return retVal;

}

TextureResourceObject* ImageFileLoader::LoadTexture(std::string file)
{

	UINT height = 0;
	UINT width = 0;
	unsigned char* imageBuffer = getImage(file, width, height);

	TextureResourceObject* retVal = card->LoadTexture(imageBuffer, 8, 4, "rgba", width, height);

	delete[] imageBuffer;
	return retVal;

}

ImageResourceObject** ImageFileLoader::LoadBitMaps(std::string file, unsigned int &size)
{

	int len;
	int slength = (int)file.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, file.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, file.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	IWICBitmapDecoder* imDecoder;
	IWICBitmapFrameDecode* frameDecoder;
	UINT height = 0;
	UINT width = 0;

	imFac->CreateDecoderFromFilename(r.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &imDecoder);

	imDecoder->GetFrameCount(&size);
	ImageResourceObject** retVal = new ImageResourceObject*[size];

	for (unsigned int i = 0; i < size; i++)
	{

		IWICFormatConverter* imConverter;
		imFac->CreateFormatConverter(&imConverter);
		imDecoder->GetFrame(i, &frameDecoder);
		frameDecoder->GetSize(&width, &height);

		imConverter->Initialize(frameDecoder, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);

		UINT imageStride = width * 4;
		UINT imageSize = height*imageStride;
		unsigned char* imageBuffer = new unsigned char[imageSize];
		imConverter->CopyPixels(nullptr, imageStride, imageSize, imageBuffer);
		retVal[i] = card->Load2DImage(imageBuffer, 1, 4, width, height);

		vectorUI2 pos;
		IWICMetadataQueryReader* metaDataReader = nullptr;
		frameDecoder->GetMetadataQueryReader(&metaDataReader);
		PROPVARIANT propValue;
		PropVariantInit(&propValue);
		metaDataReader->GetMetadataByName(L"/imgdesc/Left", &propValue);
		pos.x = propValue.uiVal;
		PropVariantClear(&propValue);
		metaDataReader->GetMetadataByName(L"/imgdesc/Top", &propValue);
		pos.y = propValue.uiVal;

		PropVariantClear(&propValue);
		metaDataReader->GetMetadataByName(L"/grctlext/Delay", &propValue);
		retVal[i]->SetDelay(propValue.uiVal * 10);
		PropVariantClear(&propValue);

		retVal[i]->SetPos(pos);
		delete[] imageBuffer;
		 
		metaDataReader->Release();
		frameDecoder->Release();
		imConverter->Release();
		
	}

	imDecoder->Release();
	return retVal;


}

void ImageFileLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	imFac = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory, (LPVOID*)&imFac);

	this->card = card;
	extens = 6;
	extensions = new string[extens];
	extensions[0] = "png";
	extensions[1] = "bmp";
	extensions[2] = "gif";
	extensions[3] = "jpg";
	extensions[4] = "jpeg";
	extensions[5] = "tiff";

}

std::string* ImageFileLoader::Extension() const
{

	return extensions;

}

int ImageFileLoader::GetNrExtensions() const
{

	return extens;

}

ImageFileLoader::~ImageFileLoader()
{

	imFac->Release();
	delete[] extensions;

}