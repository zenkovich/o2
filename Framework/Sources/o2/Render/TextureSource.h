﻿#pragma once

#include "o2/Render/TextureRef.h"
#include "o2/Utils/Math/Rect.h"

namespace o2
{
	struct TextureSource 
	{
		TextureRef texture;
		RectF      sourceRect;
	};
}