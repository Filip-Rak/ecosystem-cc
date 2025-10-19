#include "Application/CLI/ParseCLI.hpp"

#include <CLI/CLI.hpp>

#include "Application/CLI/CLIParams.hpp"

namespace cc::cli
{
auto parse( int argc, char** argv ) -> Params
{
	Params params;

	CLI::App app{ "Ecosystem" };
	app.add_flag( "--headless", params.headless, "Run without GUI" );

	try
	{
		app.parse( argc, argv );
	}
	catch ( const CLI::ParseError& error )
	{
		std::exit( app.exit( error ) );
	}

	return params;
}
}  // namespace cc::cli