#pragma once

#include "Utils/IObject.h"
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

		void RebuildBasicUIManager();

		IOBJECT(BasicUIStyleBuilder);
	};
}
