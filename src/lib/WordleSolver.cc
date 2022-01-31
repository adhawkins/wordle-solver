#include "WordleSolver.h"

#include <algorithm>
#include <iostream>
#include <sstream>

CWordleSolver::CWordleSolver(const std::string &WordListFile, int Length, const std::string& InitialGuess)
:	m_InitialGuess(InitialGuess),
	m_WordList(WordListFile, Length)
{
}

std::string CWordleSolver::InitialGuess()
{
	if (!m_InitialGuess.empty())
	{
		m_Guesses.push_back(m_InitialGuess);
	}
	else
	{
		m_Guesses.push_back(m_WordList.GetGuess(m_Guesses));
	}

	return m_Guesses.back();
}

std::string CWordleSolver::InvalidWord()
{
	m_Guesses.push_back(m_WordList.GetGuess(m_Guesses));
	return m_Guesses.back();
}

std::string CWordleSolver::Result(const CWordList::tMatchTypeVector& Matches)
{
	m_WordList.Filter(m_Guesses.back(), Matches);
	m_GuessNum++;
	m_Guesses.push_back(m_WordList.GetGuess(m_Guesses));
	return m_Guesses.back();
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

CWordList::tMatchTypeVector CWordleSolver::ParseMatches(const std::string &strMatches)
{
	CWordList::tMatchTypeVector Matches;

	for (const auto &Char : strMatches)
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

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(wordlesolver)
{
	class_<CWordleSolver>("CWordleSolver")
			.constructor<const std::string &, int, const std::string &>()
			.function("InitialGuess", &CWordleSolver::InitialGuess)
			.function("InvalidWord", &CWordleSolver::InvalidWord)
			.function("Result", &CWordleSolver::Result)
			.function("GuessNum", &CWordleSolver::GuessNum)
			.class_function("ParseMatches", &CWordleSolver::ParseMatches);
}

#endif
