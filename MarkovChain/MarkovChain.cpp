

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

}

void MarkovChain::train(std::string trainingText)
{
    uint32_t trainingLength = trainingText.length();

    for (uint32_t i = 0; i < trainingLength - order; ++i)
    {
        std::string gram = trainingText.substr(i, order);

        // Insert or ignore
        nGrams[gram].push_back(std::string(1, trainingText.at(i + order)));
    }

#if DEBUG
    nGramType::iterator itr = nGrams.begin();
    for (itr; itr != nGrams.end(); ++itr)
    {
        std::cout << itr->first << ": ";
        std::vector<std::string>::iterator jtr = itr->second.begin();
        for (jtr; jtr != itr->second.end(); ++jtr)
        {
            std::cout << jtr->c_str() << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif
}

std::string MarkovChain::predict(std::string question)
{
    if (question.length() < order)
    {
        return "Question too short";
    }

    std::string response = question.substr(question.length() - order, order);

    std::string currGram = " ";
    int count = 0;

    while (response.at(response.length() - 1) != ' ' || 
           count++ < order * order)
    {
        currGram = response.substr(response.length() - order, order);

        std::vector<std::string>* possibilities = &nGrams[currGram];

        std::string next = "";
        if (possibilities->size() > 0)
        {
            next = *select_randomly(possibilities->begin(), possibilities->end());
        }
        else
        {
            next = *select_randomly(nGrams.begin()->first.begin(), nGrams.begin()->first.end());
        }

        response += next;
    }

    return response;
}


