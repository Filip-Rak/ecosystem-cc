#include "Application/App.hpp"

#include "Application/CLI/CLIOptions.hpp"
#include "Application/CLI/ParseCLI.hpp"

using namespace cc;

auto main( int argc, char** argv ) -> int
{
	const cli::Options options = cli::parse( argc, argv );
	app::App app( options );
	app.run();
}