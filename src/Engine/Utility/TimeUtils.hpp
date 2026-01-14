#include <chrono>

namespace cc
{
using clock = std::chrono::steady_clock;
auto getTimeStamp() -> clock::time_point;
auto getTimeStampDiff( clock::time_point start ) -> float;
auto getTimeStampDiff( clock::time_point start, clock::time_point end ) -> float;
}  // namespace cc