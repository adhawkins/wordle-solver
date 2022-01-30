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

	void Filter(const std::string &Guess, const tMatchTypeVector &Matches);
	std::string GetGuess(const std::vector<std::string> &Guesses) const;

private:
	void LoadWords(const std::string &FileName, int Length);
	void SortWords();

	static std::string::size_type UniqueLetters(const std::string &String);

	std::vector<std::string> m_Words;
};
