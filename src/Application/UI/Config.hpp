#pragma once

#include <string>

#include "Application/UI/Constants.h"

namespace cc::app::ui::config
{
struct SidePanel
{
	float width{ constant::SidePanel.InitialWidth };
	std::string pauseButtonLabel{ constant::SidePanel.WidgetLabels.PauseButtonPaused };
};
}  // namespace cc::app::ui::config