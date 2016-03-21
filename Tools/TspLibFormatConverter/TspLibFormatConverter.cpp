#include <iostream>
#include <fstream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <cstdint>
#include <cstring>
#include <cmath>

#include <unordered_map>

std::unordered_map<std::string, uint32_t> optimal_costs
{
	{ "a280" , 2579 },
	{ "ali535" , 202339 },
	{ "att48" , 10628 },
	{ "att532" , 27686 },
	{ "bayg29" , 1610 },
	{ "bays29" , 2020 },
	{ "berlin52" , 7542 },
	{ "bier127" , 118282 },
	{ "brazil58" , 25395 },
	{ "brd14051" , 469385 },
	{ "brg180" , 1950 },
	{ "burma14" , 3323 },
	{ "ch130" , 6110 },
	{ "ch150" , 6528 },
	{ "d198" , 15780 },
	{ "d493" , 35002 },
	{ "d657" , 48912 },
	{ "d1291" , 50801 },
	{ "d1655" , 62128 },
	{ "d2103" , 80450 },
	{ "d15112" , 1573084 },
	{ "d18512" , 645238 },
	{ "dantzig42" , 699 },
	{ "dsj1000" , 18659688 },
	{ "eil51" , 426 },
	{ "eil76" , 538 },
	{ "eil101" , 629 },
	{ "fl417" , 11861 },
	{ "fl1400" , 20127 },
	{ "fl1577" , 22249 },
	{ "fl3795" , 28772 },
	{ "fnl4461" , 182566 },
	{ "fri26" , 937 },
	{ "gil262" , 2378 },
	{ "gr17" , 2085 },
	{ "gr21" , 2707 },
	{ "gr24" , 1272 },
	{ "gr48" , 5046 },
	{ "gr96" , 55209 },
	{ "gr120" , 6942 },
	{ "gr137" , 69853 },
	{ "gr202" , 40160 },
	{ "gr229" , 134602 },
	{ "gr431" , 171414 },
	{ "gr666" , 294358 },
	{ "hk48" , 11461 },
	{ "kroA100" , 21282 },
	{ "kroB100" , 22141 },
	{ "kroC100" , 20749 },
	{ "kroD100" , 21294 },
	{ "kroE100" , 22068 },
	{ "kroA150" , 26524 },
	{ "kroB150" , 26130 },
	{ "kroA200" , 29368 },
	{ "kroB200" , 29437 },
	{ "lin105" , 14379 },
	{ "lin318" , 42029 },
	{ "linhp318" , 41345 },
	{ "nrw1379" , 56638 },
	{ "p654" , 34643 },
	{ "pa561" , 2763 },
	{ "pcb442" , 50778 },
	{ "pcb1173" , 56892 },
	{ "pcb3038" , 137694 },
	{ "pla7397" , 23260728 },
	{ "pla33810" , 66048945 },
	{ "pla85900" , 142382641 },
	{ "pr76" , 108159 },
	{ "pr107" , 44303 },
	{ "pr124" , 59030 },
	{ "pr136" , 96772 },
	{ "pr144" , 58537 },
	{ "pr152" , 73682 },
	{ "pr226" , 80369 },
	{ "pr264" , 49135 },
	{ "pr299" , 48191 },
	{ "pr439" , 107217 },
	{ "pr1002" , 259045 },
	{ "pr2392" , 378032 },
	{ "rat99" , 1211 },
	{ "rat195" , 2323 },
	{ "rat575" , 6773 },
	{ "rat783" , 8806 },
	{ "rd100" , 7910 },
	{ "rd400" , 15281 },
	{ "rl1304" , 252948 },
	{ "rl1323" , 270199 },
	{ "rl1889" , 316536 },
	{ "rl5915" , 565530 },
	{ "rl5934" , 556045 },
	{ "rl11849" , 923288 },
	{ "si175" , 21407 },
	{ "si535" , 48450 },
	{ "si1032" , 92650 },
	{ "st70" , 675 },
	{ "swiss42" , 1273 },
	{ "ts225" , 126643 },
	{ "tsp225" , 3916 },
	{ "u159" , 42080 },
	{ "u574" , 36905 },
	{ "u724" , 41910 },
	{ "u1060" , 224094 },
	{ "u1432" , 152970 },
	{ "u1817" , 57201 },
	{ "u2152" , 64253 },
	{ "u2319" , 234256 },
	{ "ulysses16" , 6859 },
	{ "ulysses22" , 7013 },
	{ "usa13509" , 19982859 },
	{ "vm1084" , 239297 },
	{ "vm1748" , 336556 },
	{ "br17", 39 },
	{ "ft53", 6905 },
	{ "ft70", 38673 },
	{ "ftv33", 1286 },
	{ "ftv35", 1473 },
	{ "ftv38", 1530 },
	{ "ftv44", 1613 },
	{ "ftv47", 1776 },
	{ "ftv55", 1608 },
	{ "ftv64", 1839 },
	{ "ftv70", 1950 },
	{ "ftv90", 1579 },
	{ "ftv100", 1788 },
	{ "ftv110", 1958 },
	{ "ftv120", 2166 },
	{ "ftv130", 2307 },
	{ "ftv140", 2420 },
	{ "ftv150", 2611 },
	{ "ftv160", 2683 },
	{ "ftv170", 2755 },
	{ "kro124", 36230 },
	{ "p43", 5620 },
	{ "rbg323", 1326 },
	{ "rbg358", 1163 },
	{ "rbg403", 2465 },
	{ "rbg443", 2720 },
	{ "ry48p", 14422 },
};

int main(int ac, const char** av)
{
	namespace po = boost::program_options;
	po::options_description desc("Options");

	desc.add_options()
		("help", "Display help message")
		("input", po::value<std::string>(), "TspLib XML file name to convert to binary format")
		("output", po::value<std::string>(), "TspLib binary output file name")
		("optimum", po::value<uint32_t>(), "Expected optimal total distance")
		("problem", po::value<std::string>(), "Known name of the problem (may be used to automatically set optimal value)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(ac, av, desc), vm);
	po::notify(vm);

	if(vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 1;
	}

	std::string input_filename;
	std::string output_filename;
	uint32_t optimum = 0;

	if(vm.count("input"))
	{
		input_filename = vm["input"].as<std::string>();
		std::cout << "Input file: " << input_filename << std::endl;
	}
	else
	{
		std::cout << "Error: Input filename not set" << std::endl;
		std::cout << desc << std::endl;
		return 1;
	}
	
	if (vm.count("output"))
	{
		output_filename = vm["output"].as<std::string>();
		std::cout << "Output file: " << output_filename << std::endl;
	}
	else
	{
		std::cout << "Error: Output filename not set" << std::endl;
		std::cout << desc << std::endl;
		return 1;
	}
	
	if (vm.count("optimum"))
	{
		optimum = vm["optimum"].as<uint32_t>();
	}
	else if(vm.count("problem"))
	{
		std::cout << "Problem: " << vm["problem"].as<std::string>() << std::endl;
		auto iter = optimal_costs.find(vm["problem"].as<std::string>());
		if(iter == optimal_costs.end())
		{
			std::cout << "Unknown problem name passed" << std::endl;
		}
		else
		{
			optimum = iter->second;
		}
	}

	std::ifstream infile;
	infile.open(input_filename);

	std::ofstream outfile;
	outfile.open(output_filename, std::ios_base::binary);

	using boost::property_tree::ptree;
	using boost::property_tree::read_xml;

	ptree tree;

	std::cout << "Parsing xml file" << std::endl;

	read_xml(infile, tree);

	ptree root = tree.get_child("travellingSalesmanProblemInstance");

	std::string name = root.get<std::string>("name");
	std::string source = root.get<std::string>("source");
	std::string description = root.get<std::string>("description");
	uint32_t ignoredDigits = root.get<uint32_t>("ignoredDigits");

	const ptree& graph = root.get_child("graph");

	uint32_t count = graph.size();
	outfile.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));
	outfile.write(reinterpret_cast<const char*>(&optimum), sizeof(uint32_t));
	outfile.write(reinterpret_cast<const char*>(&ignoredDigits), sizeof(uint32_t));

	uint32_t *distances = new uint32_t[count * count];
	memset(distances, 0, sizeof(uint32_t) * count * count);

	std::cout << "Graph nodes: " << count << std::endl;

	std::cout << "Parsing matrix" << std::endl;

	size_t from_index = 0;
	for (const auto &viter : graph)
	{
		if (from_index % 10 == 0) std::cout << from_index << "/" << count << std::endl;
		for (const auto &eiter : viter.second)
		{
			float distance = eiter.second.get_child("<xmlattr>").get<float>("cost");
			size_t to_index = eiter.second.get_value<size_t>();
			uint32_t normalized_distance = static_cast<uint32_t>(distance * pow(10, ignoredDigits));
			distances[from_index * count + to_index] = normalized_distance;
		}
		++from_index;
	}

	std::cout << "Writing matrix" << std::endl;

	outfile.write(reinterpret_cast<const char*>(distances), sizeof(uint32_t) * count * count);

	outfile.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));

	infile.close();
	outfile.close();

	return 0;
}