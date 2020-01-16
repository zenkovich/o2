#include "o2/stdafx.h"
#include "Debug.h"

#include <cstdarg>
#include "o2/Assets/Assets.h"
#include "o2/Assets/BitmapFontAsset.h"
#include "o2/Assets/VectorFontAsset.h"
#include "o2/Render/Render.h"
#include "o2/Render/Text.h"
#include "o2/Render/VectorFont.h"
#include "o2/Render/VectorFontEffects.h"
#include "o2/Utils/Debug/Log/ConsoleLogStream.h"
#include "o2/Utils/Debug/Log/FileLogStream.h"
#include "o2/Utils/Debug/Log/LogStream.h"

#undef DrawText

namespace o2
{
	Debug::Debug()
	{
		FileLogStream* fileLogStream = mnew FileLogStream("", "log.txt");
		mLogStream = mnew ConsoleLogStream("");
		fileLogStream->BindStream(mLogStream);
	}

	Debug::~Debug()
	{
		delete mLogStream->GetParentStream();
		delete mFont;
		delete mText;
	}

	void Debug::InitializeFont()
	{
		mFont = mnew VectorFont(o2Assets.GetBuiltAssetsPath() + "debugFont.ttf");
		mFont->AddEffect<FontStrokeEffect>();
		mText = mnew Text(FontRef(mFont));
	}

	void Debug::Update(float dt)
	{
		mDbgDrawables.ForEach([&](auto drw) { drw->delay -= dt; });
	}

	void Debug::Draw()
	{
		Vector<IDbgDrawable*> freeDrawables;
		for (auto drw : mDbgDrawables)
		{
			drw->Draw();

			if (drw->delay < 0)
				freeDrawables.Add(drw);
		}

		freeDrawables.ForEach([&](auto drw) { mDbgDrawables.Remove(drw); delete drw; });
	}

	void Debug::Log(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->OutStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void Debug::LogStr(const WString& out)
	{
		mInstance->mLogStream->OutStr(out);
	}

	void Debug::LogWarning(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->WarningStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void Debug::LogWarningStr(const WString& out)
	{
		mInstance->mLogStream->WarningStr(out);
	}

	void Debug::LogError(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->ErrorStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void Debug::LogErrorStr(const WString& out)
	{
		mInstance->mLogStream->ErrorStr(out);
	}

	LogStream* Debug::GetLog()
	{
		return mInstance->mLogStream;
	}

	void Debug::DrawRect(const RectF& rect, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgRect(rect, color, delay));
	}

	void Debug::DrawRect(const RectF& rect, const Color4& color)
	{
		mDbgDrawables.Add(mnew DbgRect(rect, color, -1.0f));
	}

	void Debug::DrawRect(const RectF& rect, float delay)
	{
		mDbgDrawables.Add(mnew DbgRect(rect, Color4::White(), delay));
	}

	void Debug::DrawLine(const Vec2F& begin, const Vec2F& end, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgLine(begin, end, color, delay));
	}

	void Debug::DrawLine(const Vec2F& begin, const Vec2F& end, const Color4& color)
	{
		mDbgDrawables.Add(mnew DbgLine(begin, end, color, -1.0f));
	}

	void Debug::DrawLine(const Vec2F& begin, const Vec2F& end, float delay)
	{
		mDbgDrawables.Add(mnew DbgLine(begin, end, Color4::White(), delay));
	}

	void Debug::DrawLine(const Vector<Vec2F>& points, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgPolyLine(points, color, delay));
	}

	void Debug::DrawLine(const Vector<Vec2F>& points, const Color4& color)
	{
		mDbgDrawables.Add(mnew DbgPolyLine(points, color, -1.0f));
	}

	void Debug::DrawLine(const Vector<Vec2F>& points, float delay)
	{
		mDbgDrawables.Add(mnew DbgPolyLine(points, Color4::White(), delay));
	}

	void Debug::DrawText(const Vec2F& position, const String& text, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgText(position, text, mFont, color, delay));
	}

	void Debug::DrawText(const Vec2F& position, const String& text, const Color4& color)
	{
		mDbgDrawables.Add(mnew DbgText(position, text, mText, color));
	}

	void Debug::DrawText(const Vec2F& position, const String& text, float delay)
	{
		mDbgDrawables.Add(mnew DbgText(position, text, mFont, Color4::White(), delay));
	}

	void Debug::DrawArrow(const Vec2F& begin, const Vec2F& end, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgArrow(begin, end, color, delay));
	}

	void Debug::DrawArrow(const Vec2F& begin, const Vec2F& end, const Color4& color /*= Color4::White()*/)
	{
		mDbgDrawables.Add(mnew DbgArrow(begin, end, color, -1.0f));
	}

	void Debug::DrawArrow(const Vec2F& begin, const Vec2F& end, float delay)
	{
		mDbgDrawables.Add(mnew DbgArrow(begin, end, Color4::White(), delay));
	}

	void Debug::DrawRay(const Vec2F& begin, const Vec2F& dir, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgLine(begin, begin + dir, color, delay));
	}

	void Debug::DrawRay(const Vec2F& begin, const Vec2F& dir, const Color4& color)
	{
		mDbgDrawables.Add(mnew DbgLine(begin, begin + dir, color, -1.0f));
	}

	void Debug::DrawRay(const Vec2F& begin, const Vec2F& dir, float delay)
	{
		mDbgDrawables.Add(mnew DbgLine(begin, begin + dir, Color4::White(), delay));
	}

	void Debug::DrawCircle(const Vec2F& origin, float radius, const Color4& color, float delay)
	{
		mDbgDrawables.Add(mnew DbgCircle(origin, radius, color, delay));
	}

	void Debug::DrawCircle(const Vec2F& origin, float radius, const Color4& color)
	{
		mDbgDrawables.Add(mnew DbgCircle(origin, radius, color, -1.0f));
	}

	void Debug::DrawCircle(const Vec2F& origin, float radius, float delay)
	{
		mDbgDrawables.Add(mnew DbgCircle(origin, radius, Color4::White(), delay));
	}

	Debug::IDbgDrawable::IDbgDrawable():
		delay(-1.0f)
	{}

	Debug::IDbgDrawable::IDbgDrawable(const Color4& color, float delay):
		color(color), delay(delay)
	{}

	Debug::IDbgDrawable::~IDbgDrawable()
	{}

	Debug::DbgLine::DbgLine()
	{}

	Debug::DbgLine::DbgLine(const Vec2F& begin, const Vec2F& end, const Color4& color, float delay /*= -1.0f*/):
		begin(begin), end(end), IDbgDrawable(color, delay)
	{}

	void Debug::DbgLine::Draw()
	{
		o2Render.DrawAALine(begin, end, color);
	}

	Debug::DbgCircle::DbgCircle():
		radius(0)
	{}

	Debug::DbgCircle::DbgCircle(const Vec2F& origin, float radius, const Color4& color, float delay /*= -1.0f*/):
		origin(origin), radius(radius), IDbgDrawable(color, delay)
	{}

	void Debug::DbgCircle::Draw()
	{
		o2Render.DrawAACircle(origin, radius, color);
	}

	Debug::DbgRect::DbgRect()
	{}

	Debug::DbgRect::DbgRect(const RectF& rect, const Color4& color, float delay /*= -1.0f*/):
		rect(rect), IDbgDrawable(color, delay)
	{}

	void Debug::DbgRect::Draw()
	{
		o2Render.DrawAARectFrame(rect, color);
	}

	Debug::DbgPolyLine::DbgPolyLine()
	{}

	Debug::DbgPolyLine::DbgPolyLine(const Vector<Vec2F>& points, const Color4& color, float delay /*= -1.0f*/):
		points(points), IDbgDrawable(color, delay)
	{}

	void Debug::DbgPolyLine::Draw()
	{
		o2Render.DrawAALine(points, color);
	}

	Debug::DbgText::DbgText():
		textDrawable(nullptr)
	{}

	Debug::DbgText::DbgText(const Vec2F& position, const String& text, Text* textDrawable, const Color4& color):
		position(position), text(text), textDrawable(textDrawable), ownTextDrawable(false), IDbgDrawable(color, -1.0f)
	{}

	Debug::DbgText::DbgText(const Vec2F& position, const String& text, VectorFont* font, const Color4& color,
							float delay /*= -1.0f*/):
		position(position), text(text), ownTextDrawable(true), IDbgDrawable(color, delay)
	{
		textDrawable = mnew Text(FontRef(font));
	}

	Debug::DbgText::~DbgText()
	{
		if (ownTextDrawable)
			delete textDrawable;
	}

	void Debug::DbgText::Draw()
	{
		if (textDrawable)
		{
			textDrawable->SetPosition(position);
			textDrawable->SetText(text);
			textDrawable->SetColor(color);
			textDrawable->Draw();
		}
	}

	Debug::DbgArrow::DbgArrow()
	{ }

	Debug::DbgArrow::DbgArrow(const Vec2F& begin, const Vec2F& end, const Color4& color, float delay /*= -1.0f*/) :
		begin(begin), end(end), IDbgDrawable(color, delay)
	{ }

	void Debug::DbgArrow::Draw()
	{
		o2Render.DrawAAArrow(begin, end, color);
	}

}
