#include <cstddef>

namespace cc::app
{
struct PerformanceLog
{
	std::size_t iteration;
	std::size_t agentCount;
	float frameTime;
	float tickTime;
};
}  // namespace cc::app