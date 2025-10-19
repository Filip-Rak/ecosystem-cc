#pragma once

namespace cc::cli
{
struct Params;
auto parse( int argc, char** argv ) -> Params;
}  // namespace cc::cli