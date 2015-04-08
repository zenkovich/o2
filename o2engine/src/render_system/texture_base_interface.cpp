#include "texture_base_interface.h"

#include "render_system.h"

OPEN_O2_NAMESPACE

ITexture::ITexture():
mFormat(TextureFormat::Default), mUsage(TextureUsage::Default)
{
}

ITexture::~ITexture()
{
}

const String& ITexture::GetFileName() const
{
	return mFileName;
}

TextureFormat ITexture::GetFormat() const
{
	return mFormat;
}

TextureUsage ITexture::GetUsage() const
{
	return mUsage;
}

Vec2F ITexture::GetSize() const
{
	return mSize;
}

void TextureBaseInterface::OnZeroRefCount()
{
	AppRender()->RemoveTexture((Texture*)this);
}

TextureBaseInterface::TextureBaseInterface():
ITexture()
{
}

TextureBaseInterface::~TextureBaseInterface()
{
}

CLOSE_O2_NAMESPACE