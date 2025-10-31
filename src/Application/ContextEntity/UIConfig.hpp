#pragma once

#include <string>

#include "Application/Constants/UIConstants.hpp"

namespace cc::app
{
struct SidePanel
{
	float width{ constant::SidePanel.InitialWidth };
	std::string pauseButtonLabel{ constant::SidePanel.WidgetLabels.PauseButtonPaused };
};

struct UIConfig
{
	SidePanel sidePanel;
};
}  // namespace cc::app