#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "WordleSolver.h"

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
			while (strMatches.length() != Length && strMatches != "z")
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
	}
	else
	{
		std::cout << "Usage: " << argv[0] << "wordlistfile wordlength [initial guess word]" << std::endl;
	}

	return 0;
}
