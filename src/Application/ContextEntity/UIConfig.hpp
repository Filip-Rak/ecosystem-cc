#pragma once

#include <string>

#include "Application/Constants/UIConstants.hpp"

namespace cc::app
{
struct SidePanel
{
	float width{ constant::UI.SidePanel.InitialWidth };
	std::string pauseButtonLabel{ constant::UI.SidePanel.WidgetLabels.PauseButtonPaused };
};

struct UIConfig
{
	SidePanel sidePanel;
};
}  // namespace cc::app