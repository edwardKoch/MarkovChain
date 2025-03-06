

//-----------------------------------------------------------------------------
// File: MarkovChain.h
// Author: Edward Koch
// Description: Holds the declaration of the MarkovChain Class
//              as created by following Daniel Schiffman's tutorial
// https://thecodingtrain.com/challenges/42-markov-chain-name-generator
//
// Revision History
// Author     Date        Description
//-----------------------------------------------------------------------------
// E. Koch    03/05/25    Initial Creation 
//-----------------------------------------------------------------------------
#include "MarkovChain.h"

#include <iostream>
#include <sstream>
#include <fstream>

// The following is from: 
// https://stackoverflow.com/questions/6942273/how-to-get-a-random-element-from-a-c-container
#include  <random>
#include  <iterator>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}
// End copy pasta

MarkovChain::MarkovChain(uint16_t n)
    : order(n)
{
    if (order < 1)
    {
        order = 1;
    }
}

void MarkovChain::train(std::string fileName)
{
    std::ifstream file(fileName);

    if (file.is_open()) 
    {
        std::cout << "Training on File " << fileName << "..." << std::endl;
        std::string line;
        while (std::getline(file, line)) 
        {
            if (line.length() > 0)
            {
                if (!digestLine(line))
                {
                    break;
                }
            }
        }
        file.close();
        std::cout << "Training Complete" << std::endl;
    }
    else 
    {
        std::cout << "Unable to open file" << std::endl;
    }
}

std::string MarkovChain::predict(std::string question)
{
    splitString(question);

    if (tempStringVec.size() == 0)
    {
        return "Error! Try Asking Again!";
    }

    std::string response = question;

    std::string currGram = " ";
    int count = 0;

    StringVec::iterator itr = tempStringVec.begin();
    StringVec::iterator jtr = itr;
    std::string gram = "";
    std::string next = "";

    // Until I am satisfied with the answer...
    while (response.at(response.length() - 2) != '.' 
           && count++ < 100)
    {
        // Track if a next word has been found
        bool found = false;

        // Split the current response into a vector of words
        splitString(response);

        // Try to find the longest match possible, starting at order
        for (uint16_t n = order; n > 0 && !found; --n)
        {
            // Skip any n greater than the length of the current context
            if (tempStringVec.size() < n)
            {
                continue;
            }

            // Combine the last n words into a gram
            itr = tempStringVec.end() - n;
            gram = "";
            for (jtr = itr; jtr != tempStringVec.end(); ++jtr)
            {
                if (jtr != itr)
                {
                    gram += " ";
                }

                gram += *jtr;
            }

            // Check if this gram is in the nGrams map
            nGramType::iterator gtr = nGrams.find(gram);
            if (gtr != nGrams.end())
            {
                found = true;

                // Get a Random next word from the possibilities
                StringVec* possibilities = &gtr->second;
                next = *select_randomly(possibilities->begin(), possibilities->end());
            }
        }

        // if no matches were found for any length gram, select a radom one
        if (!found)
        {
            nGramType::iterator randomNGram = select_randomly(nGrams.begin(), nGrams.end());
            next = *select_randomly(randomNGram->second.begin(), randomNGram->second.end());
        }
        if (count == 1)
        {
            response = gram + " " + next + " ";
        }
        else
        {
            response += next + " ";
        }
    }

    return response;
}


void MarkovChain::splitString(std::string sentence)
{
    tempStringVec.clear();


    std::istringstream s(sentence);

    tempStringVec.insert(tempStringVec.end(),
                         std::istream_iterator<std::string>(s),
                         std::istream_iterator<std::string>());
}

void MarkovChain::sanitizeTempStringVec()
{
    // All alphabetical characters and the following are considered "valid"
    static const uint8_t numChars = 22;
    static char validChars[numChars] =
    {
        ' ', '!', '"', '\'', '(', ')', ',', '.', '-',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        ':', ';', '?'
    };

    StringVec::iterator itr = tempStringVec.begin();
    for (itr; itr != tempStringVec.end(); /*NoOp*/)
    {
        bool wordValid = true;
        bool charValid = true;

        // Check if word is "valid"
        std::string* word = &(*itr);

        for (int idx = 0; idx < word->length(); ++idx)
        {
            charValid = false;

            char c = word->at(idx);

            if ((c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z'))
            {
                charValid = true;
                continue;
            }

            for (int i = 0; i < numChars; ++i)
            {
                if (c == validChars[i])
                {
                    charValid = true;
                    break;
                }
            }

            if (charValid)
            {
                continue;
            }

            wordValid = false;
        }

        if (wordValid)
        {
            ++itr;
        }
        else
        {
            itr = tempStringVec.erase(itr);
        }
    }

}

bool MarkovChain::digestLine(std::string line)
{
    bool dataAdded = false;
    // Split intput string into vector of words
    splitString(line);

    // Remove words with invalid characters
    sanitizeTempStringVec();

    if (tempStringVec.size() == 0)
    {
        // Do not count invalid lines as no data added lines
        return true;
    }

    // Add nGrams up to order
    StringVec::iterator itr = tempStringVec.begin();
    StringVec::iterator jtr = itr;
    std::string gram = "";

    // For all words in string
    for (itr; itr != tempStringVec.end() - 1; itr++)
    {
        // for all N up to order
        for (uint16_t i = 1; i <= order; ++i)
        {
            // Skip any n greater than the length of the current context
            if (tempStringVec.size() < i)
            {
                continue;
            }

            if (itr < tempStringVec.end() - i)
            {
                // Build a gram of up to i words
                gram = "";
                for (jtr = itr; jtr < itr + i; ++jtr)
                {
                    if (jtr != itr)
                    {
                        gram += " ";
                    }

                    gram += *jtr;
                }

                // Check if Gram exists
                nGramType::iterator gtr = nGrams.find(gram);

                // If Gram Exists
                if (gtr != nGrams.end())
                {
                    // Limit size of Gram vectors
                    if (gtr->second.size() < GRAM_VALUE_LIMIT)
                    {
                        // If there is enough room, get next word
                        gtr->second.push_back(*(itr + i));
                        dataAdded = true;
                    }
                }
                // if Gram does not exist
                else
                {
                    // Limit number of Grams
                    if (nGrams.size() < GRAM_KEY_LIMIT)
                    {
                        // Insert and assign new Gram
                        // Get next word based on current gram
                        nGrams[gram].push_back(*(itr + i));
                        dataAdded = true;
                    }
                }
            }
        }
    }

    return dataAdded;
}
