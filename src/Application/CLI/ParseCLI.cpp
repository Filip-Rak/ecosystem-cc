#include "Application/CLI/ParseCLI.hpp"

#include <cstdlib>

#include <CLI/CLI.hpp>

#include "Application/CLI/CLIOptions.hpp"

namespace cc::cli
{
auto parse( int argc, char** argv ) -> Options
{
	Options options;

	CLI::App app{ "Ecosystem" };
	app.add_option( "--preset", options.presetPath, "Simulation preset to run" );
	app.add_flag( "--headless", options.headless, "Run without GUI" );

	try
	{
		app.parse( argc, argv );
	}
	catch ( const CLI::ParseError& error )
	{
		std::exit( app.exit( error ) );
	}

	return options;
}
}  // namespace cc::cli