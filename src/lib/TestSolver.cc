#include <iostream>

#include "WordleSolver.h"

int main(int argc, const char *argv[])
{
	if (argc == 5)
	{
		std::string WordFile = argv[1];
		int Length = atoi(argv[2]);
		std::string InitialGuess = argv[3];
		std::string CorrectWord = argv[4];

		CWordleSolver Solver(WordFile, Length, InitialGuess);
		std::string Guess;
		CWordList::tMatchTypeVector Matches;

		while (true)
		{
			if (Guess.empty())
			{
				Guess = Solver.InitialGuess();
			}
			else
			{
				Guess = Solver.Result(Matches);
			}

			if (Guess.empty())
			{
				break;
			}

			Matches = CWordleSolver::CalculateMatches(CorrectWord, Guess);
			std::cout << CWordleSolver::ColourMatches(Guess, Matches) << std::endl;

			if (Guess == CorrectWord)
			{
				break;
			}
		};

		return Guess == CorrectWord ? 0 : 1;
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " wordlistfile wordlength initial-guess correct-word" << std::endl;
	}

	return 0;
}
