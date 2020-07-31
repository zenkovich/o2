#include "o2Editor/stdafx.h"
#include "CameraActorViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/EnumProperty.h"
#include "o2Editor/Core/Properties/Basic/Vector2FloatProperty.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/ImageSlicesEditorWidget.h"

namespace Editor
{
	void CameraActorViewer::RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		const Type& cameraActorType = TypeOf(CameraActor);

		auto commonFieldsLayout = mnew VerticalLayout();
		commonFieldsLayout->spacing = 5;
		commonFieldsLayout->expandWidth = true;
		commonFieldsLayout->expandHeight = false;
		commonFieldsLayout->fitByChildren = true;
		mSpoiler->AddChild(commonFieldsLayout);

		mTypeProperty = o2EditorProperties.BuildFieldType<EnumProperty>(commonFieldsLayout, cameraActorType, "mType", "",
																		mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mTypeProperty->onChanged += [&](IPropertyField* x) { OnTypeSelected(); };

		mHiddenProperties = o2UI.CreateWidget<VerticalLayout>();
		mHiddenProperties->expandWidth = true;
		mHiddenProperties->expandHeight = false;
		mHiddenProperties->fitByChildren = true;
		mSpoiler->AddChild(mHiddenProperties);

		// Size properties
		mSizePropertySpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mSizePropertySpoiler);

		auto sizeSpace = mnew Widget();
		sizeSpace->layout->minHeight = 5;
		mSizePropertySpoiler->AddChildWidget(sizeSpace);

		mSizeProperty = o2EditorProperties.BuildFieldType<Vec2FProperty>(mSizePropertySpoiler, cameraActorType, "mFixedOrFittedSize", "",
																		 mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);

		mSizeProperty->SetCaption("Size");

		// Units properties
		mUnitsPropertySpoiler = o2UI.CreateWidget<Spoiler>();
		mHiddenProperties->AddChild(mUnitsPropertySpoiler);

		auto unitsSpace = mnew Widget();
		unitsSpace->layout->minHeight = 5;
		mUnitsPropertySpoiler->AddChildWidget(unitsSpace);

		mUnitsProperty = o2EditorProperties.BuildFieldType<EnumProperty>(mUnitsPropertySpoiler, cameraActorType, "mUnits", "",
																		 mPropertiesContext, mOnChildFieldChangeCompleted, onChanged);
	}

	void CameraActorViewer::OnTypeSelected()
	{
		CameraActor::Type type = (CameraActor::Type)(mTypeProperty->GetCommonValue());

		mSizePropertySpoiler->SetExpanded(type == CameraActor::Type::FittedSize || type == CameraActor::Type::FixedSize);
		mUnitsPropertySpoiler->SetExpanded(type == CameraActor::Type::PhysicalCorrect);
	}
}

DECLARE_CLASS_MANUAL(Editor::TObjectPropertiesViewer<o2::CameraActor>);

DECLARE_CLASS(Editor::CameraActorViewer);
