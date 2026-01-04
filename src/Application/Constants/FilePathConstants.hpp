#pragma once

#include <filesystem>

namespace cc::app::constant
{
struct FilePaths
{
	const std::filesystem::path temperaturePath = "temperature.png";
	const std::filesystem::path elevationPath   = "elevation.png";
	const std::filesystem::path humidityPath    = "humidity.png";
} const filePaths;
}  // namespace cc::app::constant