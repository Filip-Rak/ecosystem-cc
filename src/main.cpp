#include "Application/App.hpp"

#include "Application/CLI/CLIOptions.hpp"
#include "Application/CLI/ParseCLI.hpp"

auto main( int argc, char** argv ) -> int
{
	const cc::cli::Options options = cc::cli::parse( argc, argv );
	cc::eco::App app( options );
	app.run();
}