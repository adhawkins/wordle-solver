#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32

#include <shlwapi.h>

#else

#include <libgen.h>

#endif

#include <getopt.h>
#include <string.h>

#include "WordleSolver.h"

#include "config.h"

std::string BaseName(const std::string &Name)
{
	char *NameCopy = new char[Name.length() + 1];

#ifdef _WIN32

	strcpy_s(NameCopy, Name.length() + 1, Name.c_str());
	PathStripPath(NameCopy);
	std::string Ret = NameCopy;

#else

	strcpy(NameCopy, Name.c_str());
	std::string Ret = basename(NameCopy);

#endif

	delete[] NameCopy;
	return Ret;
}

void ShowUsage(const std::string &ProgName)
{
	std::cerr << BaseName(ProgName) << " usage:" << std::endl
						<< std::endl;

	std::cerr << "\t-h, --help              Display this message" << std::endl;
	std::cerr << "\t-v, --version           Display version information" << std::endl;
	std::cerr << "\t-d, --dictionary        Path to dictionary to use" << std::endl;
	std::cerr << "\t[ -l, --word-length ]   Length of word to guess - defaults to 5" << std::endl;
	std::cerr << "\t[ -i, --initial-guess ] Initial guess" << std::endl;
}

void ShowVersion(const std::string &ProgName)
{
	std::cerr << BaseName(ProgName) << " v" << wordle_solver_version << std::endl;
}

int main(int argc, char **argv)
{
	bool PrintVersion = false;
	bool PrintUsage = false;
	std::string Dictionary;
	int Length = 5;
	std::string InitialGuess;

	struct option LongOptions[] =
			{
					{"help", no_argument, 0, 'h'},
					{"version", no_argument, 0, 'v'},
					{"dictionary", required_argument, 0, 'd'},
					{"word-length", required_argument, 0, 'l'},
					{"initial-guess", required_argument, 0, 'i'},
					{nullptr, no_argument, nullptr, 0},
			};

	while (true)
	{
		auto arg = getopt_long(argc, argv, "hvd:l:i:", LongOptions, 0);
		if (arg == -1)
		{
			break;
		}

		switch (arg)
		{
		case 'h':
			PrintUsage = true;
			break;

		case 'd':
			Dictionary = optarg;
			break;

		case 'l':
			Length = atoi(optarg);
			break;

		case 'i':
			InitialGuess = optarg;
			break;

		case 'v':
			PrintVersion = true;
			break;

		case '?':
			PrintUsage = true;
			break;

		default:
			abort();
		}
	}

	if (PrintVersion)
	{
		ShowVersion(argv[0]);
	}

	if (PrintUsage)
	{
		ShowUsage(argv[0]);
	}

	if (PrintVersion || PrintUsage)
	{
		return 0;
	}

	if (Dictionary.empty())
	{
		std::cerr << "Dictionary must be specified" << std::endl
							<< std::endl;
		ShowUsage(argv[0]);

		return 0;
	}

	CWordleSolver Solver(Dictionary, Length, InitialGuess);
	std::string Guess = Solver.InitialGuess();
	while (true)
	{
		if (Guess.empty())
		{
			std::cout << "Run out of guesses - sorry" << std::endl;
			break;
		}

		std::string strMatches;
		while (strMatches.length() != Length && strMatches != "z")
		{
			std::cout << Solver.NumWords() << " word" << (Solver.NumWords() == 1 ? "" : "s") << " - guess is '" << Guess << "' - Enter matches: ";
			std::getline(std::cin, strMatches);
		}

		if (strMatches.end() != std::find_if_not(strMatches.begin(),
																						 strMatches.end(),
																						 [](unsigned char Char)
																						 {
																							 return Char == 'z';
																						 }))
		{
			auto Matches = CWordleSolver::ParseMatches(strMatches);
			std::cout << CWordleSolver::ColourMatches(Guess, Matches) << std::endl;

			if (Matches.end() == std::find_if_not(Matches.begin(),
																						Matches.end(),
																						[](CWordList::tMatchType MatchType)
																						{
																							return MatchType == CWordList::tMatchType::eRightLocation;
																						}))
			{
				std::cout << "Got it in " << Solver.GuessNum() << "!" << std::endl;
				break;
			}

			Guess = Solver.Result(Matches);
		}
		else
		{
			Guess = Solver.InvalidWord();
		}
	}

	return 0;
}
