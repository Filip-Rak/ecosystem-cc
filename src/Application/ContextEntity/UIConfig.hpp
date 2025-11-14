#pragma once

#include "Application/Constants/UIConstants.hpp"

namespace cc::app
{
struct SidePanel
{
	float width{ constant::UI.SidePanel.InitialWidth };
};

struct UIConfig
{
	SidePanel sidePanel;
	bool askBeforeContinuing = false;
};
}  // namespace cc::app