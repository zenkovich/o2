#include "o2/stdafx.h"

#ifdef PLATFORM_IOS
#import <MetalKit/MetalKit.h>

#include "o2/Render/Texture.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Render/Mac/MetalWrappers.h"

namespace o2
{
    TextureBase::TextureBase():
        mImpl(mnew MTLTextureImpl())
    {}
    
    Texture::~Texture()
    {
        o2Render.OnTextureDestroyed(this);

        if (!mReady)
            return;
        
        delete mImpl;
    }

    void Texture::Create(const Vec2I& size, PixelFormat format /*= Format::Default*/, Usage usage /*= Usage::Default*/)
    {
        if (mReady)
            mImpl->texture = nil;

        mFormat = format;
        mUsage = usage;
        mSize = size;
        
        MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
        textureDescriptor.width = size.x;
        textureDescriptor.height = size.y;
        
        if (usage == Usage::RenderTarget)
            textureDescriptor.usage = MTLTextureUsageRenderTarget|MTLTextureUsageShaderRead;
        
        mImpl->texture = [RenderDevice::device newTextureWithDescriptor:textureDescriptor];
        
        mReady = true;
    }

    void Texture::Create(Bitmap* bitmap)
    {
        if (mReady)
            mImpl->texture = nil;

        mFormat = bitmap->GetFormat();
        mUsage = Usage::Default;
        mSize = bitmap->GetSize();
        
        MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
        textureDescriptor.width = mSize.x;
        textureDescriptor.height = mSize.y;
        
        mImpl->texture = [RenderDevice::device newTextureWithDescriptor:textureDescriptor];
        
        NSUInteger bytesPerRow = 4 * mSize.x;
        MTLRegion region = { { 0, 0, 0 }, { (UInt)mSize.x, (UInt)mSize.y, 1 } };
        [mImpl->texture replaceRegion:region
                          mipmapLevel:0
                            withBytes:bitmap->GetData()
                          bytesPerRow:bytesPerRow];

        mReady = true;
    }

    void Texture::SetData(Bitmap* bitmap)
    {
        NSUInteger bytesPerRow = 4 * mSize.x;
        MTLRegion region = { { 0, 0, 0 }, { (UInt)mSize.x, (UInt)mSize.y, 1 } };
        [mImpl->texture replaceRegion:region
                          mipmapLevel:0
                            withBytes:bitmap->GetData()
                          bytesPerRow:bytesPerRow];
    }

    void Texture::SetSubData(const Vec2I& offset, Bitmap* bitmap)
    {
        NSUInteger bytesPerRow = 4 * bitmap->GetSize().x;
        MTLRegion region = { { (UInt)offset.x, (UInt)offset.y, 0 }, { (UInt)bitmap->GetSize().x, (UInt)bitmap->GetSize().y, 1 } };
        [mImpl->texture replaceRegion:region
                          mipmapLevel:0
                            withBytes:bitmap->GetData()
                          bytesPerRow:bytesPerRow];
    }

    void Texture::Copy(const Texture& from, const RectI& rect)
    {
//        glBindTexture(GL_TEXTURE_2D, from.mHandle);
//
//        GLint texFormat = GL_RGB;
//        if (mFormat == PixelFormat::R8G8B8A8)
//            texFormat = GL_RGBA;
//        else if (mFormat == PixelFormat::R8G8B8)
//            texFormat = GL_RGB;

        //glCopyTexImage2D(mHandle, 0, texFormat, rect.left, rect.top, rect.Width(), rect.Height(), 0);
    }

    Bitmap* Texture::GetData()
    {
        Bitmap* bitmap = mnew Bitmap(mFormat, mSize);
        return bitmap;
    }
    
    void Texture::SetFilter(Filter filter)
    {
    }
    
    Texture::Filter Texture::GetFilter() const
    {
        return Filter::Nearest;
    }
}

#endif //PLATFORM_IOS
