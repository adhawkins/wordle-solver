#pragma once

#include <vector>
#include <string>

class CWordList;

class CWordList
{
public:
	class CWord
	{
	public:
		CWord(const std::string &Word);

		bool operator==(const CWord &Other) const
		{
			return m_Word == Other.m_Word;
		}
		void CountLetters();

		static std::string::size_type UniqueLetters(const std::string &String);

		std::string m_Word;
		std::string::size_type m_UniqueLetters;
		std::string::size_type m_UniqueVowels;
	};

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
	std::vector<std::string> Words() const;
	std::vector<CWord>::size_type NumWords() const { return m_Words.size(); }

private:
	void LoadWords(const std::string &FileName, int Length);
	void SortWords();

	std::vector<CWord> m_Words;
};
