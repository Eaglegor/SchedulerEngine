#include <boost/program_options.hpp>

int main(int ac, char** argv)
{
	boost::program_options::options_description desc;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(ac, argv, desc), vm);

	return 0;
}