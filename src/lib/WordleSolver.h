#pragma once

#include <string>

#include "WordList.h"

class CWordleSolver
{
public:
	CWordleSolver(const std::string &WordListFile, int Length, const std::string &InitialGuess = "");
	CWordleSolver(const std::vector<std::string> &Words, const std::string &InitialGuess = "");

	std::string InitialGuess();
	std::string InvalidWord();
	std::string Result(const CWordList::tMatchTypeVector &Matches);
	int GuessNum() const { return m_GuessNum; }
	int NumWords() const { return m_WordList.NumWords(); }

	static CWordList::tMatchTypeVector ParseMatches(const std::string &strMatches);
	static CWordList::tMatchTypeVector CalculateMatches(const std::string &Correct, const std::string &Guess);
	static std::string ColourMatches(const std::string &Guess, const CWordList::tMatchTypeVector &Matches);

private:
	std::string m_InitialGuess;
	CWordList m_WordList;
	std::vector<std::string> m_Guesses;
	int m_GuessNum{1};

	static constexpr const char *const m_cGreen = "\033[32m";
	static constexpr const char *const m_cYellow = "\033[33m";
	static constexpr const char *const m_cDefault = "\033[0m";
};