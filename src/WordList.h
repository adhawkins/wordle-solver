#pragma once

#include <vector>
#include <string>

class CWordList;

class CWordList
{
public:
	enum class tMatchType
	{
		eNotPresent,
		eWrongLocation,
		eRightLocation
	};

	typedef std::vector<tMatchType> tMatchTypeVector;

	CWordList(const std::string &FileName, int Length);
	CWordList(const std::vector<std::string> &Words);

	CWordList Filter(const std::string &Guess, const tMatchTypeVector &Matches) const;
	std::string GetGuess(const std::vector<std::string> &Guesses) const;

private:
	void LoadWords(const std::string &FileName, int Length);

	std::vector<std::string> m_Words;
};
