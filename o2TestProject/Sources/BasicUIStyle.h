#pragma once

#include "Utils/Basic/IObject.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	class BasicUIStyleBuilder: public IObject
	{
	public:
		void RebuildButtonStyle();
		void RebuildCloseButtonStyle();
		void RebuildArrowButtonStyle();
		void RebuildHorProgressBarStyle();
		void RebuildVerProgressBarStyle();
		void RebuildHorScrollBarStyle();
		void RebuildVerScrollBarStyle();
		void RebuildCheckboxStyle();
		void RebuildCheckboxWithoutCaptionStyle();
		void RebuildScrollAreaStyle();
		void RebuildScrollAreaStraightBarsStyle();
		void RebuildLabelStyle();
		void RebuildEditBoxStyle();
		void RebuildCustomListStyle();
		void RebuildLongListStyle();
		void RebuildListStyle();
		void RebuildCustomDropDownStyle();
		void RebuildDropDownStyle();
		void RebuildWindowStyle();
		void RebuildContextMenuStyle();
		void RebuildTreeStyle();
		void RebuildMenuPanelStyle();
		void RebuildBacklessDropdown();
		void RebuildBacklessEditbox();
		void RebuildSinglelineEditbox();
		void RebuildBacklessScrollarea();
		void RebuildExpandButton();
		void RebuildSpoiler();

		void RebuildBasicUIManager();

		IOBJECT(BasicUIStyleBuilder);
	};
}

CLASS_BASES_META(o2::BasicUIStyleBuilder)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::BasicUIStyleBuilder)
{
}
END_META;
CLASS_METHODS_META(o2::BasicUIStyleBuilder)
{

	PUBLIC_FUNCTION(void, RebuildButtonStyle);
	PUBLIC_FUNCTION(void, RebuildCloseButtonStyle);
	PUBLIC_FUNCTION(void, RebuildArrowButtonStyle);
	PUBLIC_FUNCTION(void, RebuildHorProgressBarStyle);
	PUBLIC_FUNCTION(void, RebuildVerProgressBarStyle);
	PUBLIC_FUNCTION(void, RebuildHorScrollBarStyle);
	PUBLIC_FUNCTION(void, RebuildVerScrollBarStyle);
	PUBLIC_FUNCTION(void, RebuildCheckboxStyle);
	PUBLIC_FUNCTION(void, RebuildCheckboxWithoutCaptionStyle);
	PUBLIC_FUNCTION(void, RebuildScrollAreaStyle);
	PUBLIC_FUNCTION(void, RebuildScrollAreaStraightBarsStyle);
	PUBLIC_FUNCTION(void, RebuildLabelStyle);
	PUBLIC_FUNCTION(void, RebuildEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildCustomListStyle);
	PUBLIC_FUNCTION(void, RebuildLongListStyle);
	PUBLIC_FUNCTION(void, RebuildListStyle);
	PUBLIC_FUNCTION(void, RebuildCustomDropDownStyle);
	PUBLIC_FUNCTION(void, RebuildDropDownStyle);
	PUBLIC_FUNCTION(void, RebuildWindowStyle);
	PUBLIC_FUNCTION(void, RebuildContextMenuStyle);
	PUBLIC_FUNCTION(void, RebuildTreeStyle);
	PUBLIC_FUNCTION(void, RebuildMenuPanelStyle);
	PUBLIC_FUNCTION(void, RebuildBacklessDropdown);
	PUBLIC_FUNCTION(void, RebuildBacklessEditbox);
	PUBLIC_FUNCTION(void, RebuildSinglelineEditbox);
	PUBLIC_FUNCTION(void, RebuildBacklessScrollarea);
	PUBLIC_FUNCTION(void, RebuildExpandButton);
	PUBLIC_FUNCTION(void, RebuildSpoiler);
	PUBLIC_FUNCTION(void, RebuildBasicUIManager);
}
END_META;
