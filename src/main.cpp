#include "Application/App.hpp"

#include <print>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/CLI/ParseCLI.hpp"

using namespace cc;

auto main( int argc, char** argv ) -> int
{
	const cli::Options options = cli::parse( argc, argv );

	app::App app( options );
	if ( auto error = app.init( options ); error )
	{
		std::println( "Failed to initialize the app:\n{}", *error );
		return -1;
	}

	app.run();
}