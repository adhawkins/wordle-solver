#include "WordList.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include <assert.h>

CWordList::CWord::CWord(const std::string &Word)
		: m_Word(Word)
{
	CountLetters();
}

void CWordList::CWord::CountLetters()
{
	m_UniqueLetters = UniqueLetters(m_Word);

	auto Vowels(m_Word);
	Vowels.erase(std::remove_if(Vowels.begin(),
															Vowels.end(),
															[](unsigned char x)
															{
																return (x != 'a' && x != 'e' && x != 'i' && x != 'o' && x != 'u');
															}),
							 Vowels.end());
	m_UniqueVowels = UniqueLetters(Vowels);
}

std::string::size_type CWordList::CWord::UniqueLetters(const std::string &String)
{
	return std::unordered_set<char>(std::begin(String), std::end(String)).size();
}

CWordList::CWordList(const std::string &FileName, int Length)
{
	LoadWords(FileName, Length);
	SortWords();
}

CWordList::CWordList(const std::vector<std::string> &Words)
{
	for (const auto &Word : Words)
	{
		m_Words.push_back(CWord(Word));
	}

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
																[](unsigned char x)
																{
																	return (x == '\n' || x == '\r');
																}),
								 Line.end());

			if (Line.length() == Length)
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

	bool operator()(const CWordList::CWord &Item) const
	{
		std::vector<std::string> DebugWords;
		auto Word = Item.m_Word;

		//		DebugWords.push_back("toast");

		bool Debug = (DebugWords.end() != std::find(DebugWords.begin(), DebugWords.end(), Word));
		if (Debug)
		{
			std::cout << "Debugging for '" << Word << "'" << std::endl;
		}

		// Check word doesn't contain letters we know aren't in it

		if (std::string::npos != Word.find_first_of(m_InvalidLetters))
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
				if (m_Guess[count] == Word[count])
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				break;

			case CWordList::tMatchType::eRightLocation:
				if (Word[count] != m_Guess[count])
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				break;

			case CWordList::tMatchType::eWrongLocation:
				if (m_Guess[count] == Word[count])
				{
					if (Debug)
					{
						std::cout << "Remove: " << __LINE__ << std::endl;
					}

					return true;
				}

				if (std::string::npos == Word.find(m_Guess[count]))
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

void CWordList::Filter(const std::string &Guess, const tMatchTypeVector &Matches)
{
	std::string ValidLetters;

	for (const auto &Match : Matches)

		for (std::string::size_type count = 0; count < Guess.length(); count++)
		{
			switch (Matches[count])
			{
			case tMatchType::eRightLocation:
			case tMatchType::eWrongLocation:
				ValidLetters += Guess[count];
				break;

			case tMatchType::eNotPresent:
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

		case tMatchType::eRightLocation:
		case tMatchType::eWrongLocation:
			break;
		}
	}

	CStringMatch StringMatch(Guess, InvalidLetters, Matches);

	auto InitialSize = m_Words.size();

	auto LastElement = std::remove_if(m_Words.begin(), m_Words.end(), StringMatch);
	m_Words.erase(LastElement, m_Words.end());
}

std::string CWordList::GetGuess(const std::vector<std::string> &Guesses) const
{
	if (!m_Words.empty())
	{
		if (Guesses.empty())
		{
			switch (m_Words[0].m_Word.length())
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
			if (Guesses.end() == std::find(Guesses.begin(), Guesses.end(), Word.m_Word))
			{
				return Word.m_Word;
			}
		}
	}

	return "";
}

void CWordList::SortWords()
{
	std::sort(m_Words.begin(), m_Words.end(), [](const CWord &a, const CWord &b)
						{
							if (a.m_UniqueVowels != b.m_UniqueVowels)
							{
								return a.m_UniqueVowels > b.m_UniqueVowels;
							}
							else
							{
								return a.m_UniqueLetters > b.m_UniqueLetters;
							} });
}

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(wordlist)
{
	class_<CWordList>("CWordList")
			.constructor<const std::string &, int>()
			.constructor<const std::vector<std::string> &>()
			.function("Filter", &CWordList::Filter)
			.function("GetGuess", &CWordList::GetGuess)
			.function("Words", &CWordList::Words);

	register_vector<std::string>("StringVector");
	register_vector<CWordList::tMatchType>("MatchTypeVector");

	enum_<CWordList::tMatchType>("tMatchType")
			.value("eNotPresent", CWordList::tMatchType::eNotPresent)
			.value("eWrongLocation", CWordList::tMatchType::eWrongLocation)
			.value("eRightLocation", CWordList::tMatchType::eRightLocation);
}

#endif
