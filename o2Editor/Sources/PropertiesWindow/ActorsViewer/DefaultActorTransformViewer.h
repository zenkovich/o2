#pragma once

#include "PropertiesWindow/ActorsViewer/IActorTransformViewer.h"

namespace Editor
{
	class Vec2FProperty;
	class FloatProperty;

	// -------------------------------------
	// Default editor actor transform viewer
	// -------------------------------------
	class DefaultActorTransformViewer: public IActorTransformViewer
	{
	public:
		// Default constructor. Initializes data widget
		DefaultActorTransformViewer();

		// Virtual destructor
		~DefaultActorTransformViewer();

		// Sets target actors
		void SetTargetActors(const Vector<Actor*>& actors);

		// Updates properties values
		void Refresh();

		IOBJECT(DefaultActorTransformViewer);

	protected:
		Vector<Actor*> mTargetActors;
		Vec2FProperty* mPositionProperty = nullptr;
		Vec2FProperty* mPivotProperty = nullptr;
		Vec2FProperty* mScaleProperty = nullptr;
		Vec2FProperty* mSizeProperty = nullptr;
		FloatProperty* mRotationProperty = nullptr;
		FloatProperty* mDepthProperty = nullptr;

	protected:
		void Update();
	};
}