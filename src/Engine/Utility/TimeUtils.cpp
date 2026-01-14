#include "Engine/Utility/TimeUtils.hpp"

namespace cc
{
auto getTimeStamp() -> clock::time_point
{
	return clock::now();
}

auto getTimeStampDiff( clock::time_point start ) -> float
{
	return getTimeStampDiff( start, getTimeStamp() );
}

auto getTimeStampDiff( clock::time_point start, clock::time_point end ) -> float
{
	const auto time                                  = end - start;
	const std::chrono::duration< float > timeSeconds = time;
	return timeSeconds.count();
}
}  // namespace cc