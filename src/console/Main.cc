#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "WordleSolver.h"

const char *cGreen = "\033[32m";
const char *cYellow = "\033[33m";
const char *cDefault = "\033[0m";

std::string ColourMatches(const std::string &Guess, const CWordList::tMatchTypeVector &Matches)
{
	std::stringstream ColouredMatches;

	for (CWordList::tMatchTypeVector::size_type count = 0; count < Matches.size(); count++)
	{
		std::string ColourString;

		switch (Matches[count])
		{
		case CWordList::tMatchType::eNotPresent:
			break;

		case CWordList::tMatchType::eWrongLocation:
			ColourString = cYellow;
			break;

		case CWordList::tMatchType::eRightLocation:
			ColourString = cGreen;
			break;
		}

		ColouredMatches << ColourString << Guess[count] << cDefault;
	}

	return ColouredMatches.str();
}

CWordList::tMatchTypeVector ParseMatches(const std::string& strMatches)
{
	CWordList::tMatchTypeVector Matches;

	for (const auto& Char: strMatches)
	{
		switch (Char)
		{
			case '0':
				Matches.push_back(CWordList::tMatchType::eNotPresent);
				break;

			case '1':
				Matches.push_back(CWordList::tMatchType::eWrongLocation);
				break;

			case '2':
				Matches.push_back(CWordList::tMatchType::eRightLocation);
				break;
		}
	}

	return Matches;
}
int main(int argc, const char *argv[])
{
	if (argc>=3)
	{
		std::string WordFile = argv[1];
		int Length = atoi(argv[2]);
		std::string InitialGuess;

		if (argc==4)
		{
			InitialGuess=argv[3];
		}

		CWordleSolver Solver(WordFile, Length, InitialGuess);
		std::string Guess = Solver.InitialGuess();
		while (true)
		{
			if (Guess.empty())
			{
				std::cout << "Run out of guesses - sorry" << std::endl;
				break;
			}

			std::string strMatches;
			while (strMatches.length() != Length)
			{
				std::cout << "Guess is '" << Guess << "' - Enter matches: ";
				std::getline(std::cin, strMatches);
			}

			if (strMatches.end() != std::find_if_not(strMatches.begin(),
																							strMatches.end(),
																							[](unsigned char Char)
																							{
																								return Char == 'z';
																							}))
			{
				auto Matches = ParseMatches(strMatches);
				std::cout << ColourMatches(Guess, Matches) << std::endl;

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
	}
	else
	{
		std::cout << "Usage: " << argv[0] << "wordlistfile wordlength [initial guess word]" << std::endl;
	}

	return 0;
}