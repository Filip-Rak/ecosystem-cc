#pragma once

#include "Application/Constants/UIConstants.hpp"

namespace cc::app
{
struct UIConfig
{
	struct SidePanel
	{
		float width{ constant::UI.SidePanel.InitialWidth };
	} sidePanel;

	bool askBeforeContinuing = false;
};
}  // namespace cc::app