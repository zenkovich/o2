#pragma once

#ifndef TEXTURE_SOURCE_H
#define TEXTURE_SOURCE_H

#include "o2/Render/TextureRef.h"
#include "o2/Utils/Math/Rect.h"

namespace o2
{
    struct TextureSource 
    {
        Ref<Texture> texture;
        RectF      sourceRect;
    };
}

#endif
