#include "Application/App.hpp"

#include "Application/CLI/CLIParams.hpp"
#include "Application/CLI/ParseCLI.hpp"

auto main( int argc, char** argv ) -> int
{
	const cc::cli::Params params = cc::cli::parse( argc, argv );
	cc::eco::App app( params );
	app.run();
}