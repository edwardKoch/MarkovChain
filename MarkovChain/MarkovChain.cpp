

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
        std::string line;
        while (std::getline(file, line)) 
        {
            if (line.length() > 0)
            {
                digestLine(line);
            }
        }
        file.close();
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
    while (response.at(response.length() - 1) != '.' 
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

void MarkovChain::digestLine(std::string line)
{
    // Split intput string into vector of words
    splitString(line);

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

                // Get next word based on current gram
                nGrams[gram].push_back(*(itr + i));
            }
        }
    }
}
