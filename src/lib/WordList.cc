#include "WordList.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include <assert.h>

CWordList::CWordList(const std::string &FileName, int Length)
{
	LoadWords(FileName, Length);
	SortWords();
}

CWordList::CWordList(const std::vector<std::string> &Words)
: m_Words(Words)
{
	SortWords();
}

void CWordList::LoadWords(const std::string &FileName, int Length)
{
	std::ifstream File(FileName);

	if (File)
	{
		std::string Line;
		while (std::getline(File, Line))
		{
			Line.erase(std::remove_if(Line.begin(),
																Line.end(),
																[](unsigned char x) {
																	return (x=='\n' || x=='\r');
																}), Line.end());

			if (Line.length()==Length)
			{
				m_Words.push_back(Line);
			}
		}
	}
}

class CStringMatch
{
public:
	CStringMatch(const std::string &Guess, const std::string &InvalidLetters, const CWordList::tMatchTypeVector &Matches)
	: m_Guess(Guess),
		m_InvalidLetters(InvalidLetters),
		m_Matches(Matches)
	{
	}

	bool operator()(const std::string &Item) const
	{
		std::vector<std::string> DebugWords;

//		DebugWords.push_back("toast");

		bool Debug = (DebugWords.end() != std::find(DebugWords.begin(), DebugWords.end(), Item));
		if (Debug)
		{
			std::cout << "Debugging for '" << Item << "'" << std::endl;
		}

		//Check word doesn't contain letters we know aren't in it

		if (std::string::npos != Item.find_first_of(m_InvalidLetters))
		{
			if (Debug)
			{
				std::cout << "Remove: " << __LINE__ << std::endl;
			}

			return true;
		}

		for (CWordList::tMatchTypeVector::size_type count = 0; count < m_Matches.size(); count++)
		{
			switch (m_Matches[count])
			{
			case CWordList::tMatchType::eNotPresent:
				if (m_Guess[count] == Item[count])
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				break;

			case CWordList::tMatchType::eRightLocation:
				if (Item[count] != m_Guess[count])
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				break;

			case CWordList::tMatchType::eWrongLocation:
				if (m_Guess[count] == Item[count])
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				if (std::string::npos == Item.find(m_Guess[count]))
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				break;
			}
		}

		return false;
	}

private:
	std::string m_Guess;
	CWordList::tMatchTypeVector m_Matches;
	std::string m_InvalidLetters;
};

CWordList CWordList::Filter(const std::string &Guess, const tMatchTypeVector &Matches) const
{
	std::string ValidLetters;

	for (std::string::size_type count = 0; count < Guess.length(); count++)
	{
		switch (Matches[count])
		{
			case tMatchType::eRightLocation:
			case tMatchType::eWrongLocation:
				ValidLetters += Guess[count];
				break;
		}
	}

	std::string InvalidLetters;

	for (std::string::size_type count = 0; count < Guess.length(); count++)
	{
		switch (Matches[count])
		{
			case tMatchType::eNotPresent:
				if (std::string::npos == ValidLetters.find(Guess[count]))
				{
					InvalidLetters += Guess[count];
				}
		}
	}

	CStringMatch StringMatch(Guess, InvalidLetters, Matches);

	std::vector<std::string> Ret(m_Words);
	auto LastElement = std::remove_if(Ret.begin(), Ret.end(), StringMatch);
	Ret.erase(LastElement, Ret.end());

	//std::cout << "Gone from " << m_Words.size() << " to " << Ret.size() << std::endl;

	return CWordList(Ret);
}

std::string CWordList::GetGuess(const std::vector<std::string> &Guesses) const
{
	if (!m_Words.empty())
	{
		if (Guesses.empty())
		{
			switch (m_Words[0].length())
			{
				case 5:
					return "arise";

				case 9:
					return "beautiful";

				default:
					break;
			}
		}

		for (const auto &Word : m_Words)
		{
			if (Guesses.end() == std::find(Guesses.begin(), Guesses.end(), Word))
			{
				return Word;
			}
		}
	}

	return "";
}

std::string::size_type CWordList::UniqueLetters(const std::string &String)
{
	return std::unordered_set<char>(std::begin(String), std::end(String)).size();
}

void CWordList::SortWords()
{
	std::sort(m_Words.begin(), m_Words.end(), [](const std::string &a, const std::string &b)
						{ return UniqueLetters(a) > UniqueLetters(b); });
}