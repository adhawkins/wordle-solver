#include "WordleSolver.h"

#include <algorithm>
#include <iostream>
#include <sstream>

CWordleSolver::CWordleSolver(const std::string &WordListFile, int Length, const std::string& InitialGuess)
: m_WordListFile(WordListFile),
	m_Length(Length),
	m_InitialGuess(InitialGuess)
{
}

void CWordleSolver::Solve() const
{
	std::vector<std::string> Guesses;
	std::string Guess(m_InitialGuess);
	int GuessNum = 1;

	CWordList Words(m_WordListFile, m_Length);

	while (true)
	{
		if (1 != GuessNum || m_InitialGuess.empty())
		{
			Guess = Words.GetGuess(Guesses);
		}

		if (Guess.empty())
		{
			std::cout << "Run out of guesses - sorry" << std::endl;
			break;
		}

		std::string strMatches;
		while (strMatches.length() != m_Length)
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
				std::cout << "Got it in " << GuessNum << "!" << std::endl;
				break;
			}

			Words = Words.Filter(Guess, Matches);
			GuessNum++;
		}

		Guesses.push_back(Guess);
	}
}

CWordList::tMatchTypeVector CWordleSolver::CalculateMatches(const std::string &Correct, const std::string &Guess)
{
	CWordList::tMatchTypeVector Matches(Correct.length());
	std::string SecondGuess(Guess);

	//First, go through looking for letters in correct place

	for (std::string::size_type count = 0; count < Correct.length(); count++)
	{
		if (Guess[count] == Correct[count])
		{
			Matches[count] = CWordList::tMatchType::eRightLocation;
			SecondGuess[count] = ' ';
		}
	}

	for (std::string::size_type count = 0; count < Correct.length(); count++)
	{
		if (std::string::npos != Correct.find(SecondGuess[count]))
		{
			Matches[count] = CWordList::tMatchType::eWrongLocation;
		}
	}

	return Matches;
}

std::string CWordleSolver::ColourMatches(const std::string &Guess, const CWordList::tMatchTypeVector &Matches)
{
	std::stringstream ColouredMatches;

	for (CWordList::tMatchTypeVector::size_type count = 0; count < Matches.size(); count++)
	{
		switch (Matches[count])
		{
		case CWordList::tMatchType::eNotPresent:
			ColouredMatches << Guess[count];
			break;

		case CWordList::tMatchType::eWrongLocation:
			ColouredMatches << m_cYellow << Guess[count] << m_cDefault;
			break;

		case CWordList::tMatchType::eRightLocation:
			ColouredMatches << m_cGreen << Guess[count] << m_cDefault;
			break;
		}
	}

	return ColouredMatches.str();
}

CWordList::tMatchTypeVector CWordleSolver::ParseMatches(const std::string strMatches)
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
		std::string InitialGuess;

		if (argc==4)
		{
			InitialGuess=argv[3];
		}

		CWordleSolver Solver(argv[1], atoi(argv[2]), InitialGuess);
		Solver.Solve();
	}
	else
	{
		std::cout << "Usage: " << argv[0] << "wordlistfile wordlength [initial guess word]" << std::endl;
	}

	return 0;
}