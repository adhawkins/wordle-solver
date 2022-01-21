#pragma once

#include <string>

#include "WordList.h"

class CWordleSolver
{
public:
	CWordleSolver(const std::string &WordListFile, int Length);

	void Solve() const;

private:
	static constexpr const char *m_cGreen = "\033[32m";
	static constexpr const char *m_cYellow = "\033[33m";
	static constexpr const char *m_cDefault = "\033[0m";

	std::string m_WordListFile;
	int m_Length;

	static CWordList::tMatchTypeVector CalculateMatches(const std::string &Correct, const std::string &Guess);
	static std::string ColourMatches(const std::string &Guess, const CWordList::tMatchTypeVector &Matches);
	static CWordList::tMatchTypeVector ParseMatches(const std::string strMatches);
};