#pragma once

namespace cc::cli
{
struct Options;
auto parse( int argc, char** argv ) -> Options;
}  // namespace cc::cli