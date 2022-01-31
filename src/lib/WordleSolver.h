#pragma once

#include <string>

#include "WordList.h"

class CWordleSolver
{
public:
	CWordleSolver(const std::string &WordListFile, int Length, const std::string &InitialGuess = "");

	std::string InitialGuess();
	std::string InvalidWord();
	std::string Result(const CWordList::tMatchTypeVector &Matches);
	int GuessNum() const { return m_GuessNum; }

	static CWordList::tMatchTypeVector ParseMatches(const std::string &strMatches);

private:
	std::string m_InitialGuess;
	CWordList m_WordList;
	std::vector<std::string> m_Guesses;
	int m_GuessNum{1};

	static CWordList::tMatchTypeVector CalculateMatches(const std::string &Correct, const std::string &Guess);
};