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
#ifndef MARKOV_CHAIN_H
#define MARKOV_CHAIN_H

#include <map>
#include <vector>
#include <string>
#include  <random>

class MarkovChain
{
public:
    MarkovChain(uint16_t n);

    void train(std::string fileName);

    std::string predict(std::string question);

private:
    static const uint32_t GRAM_KEY_LIMIT = UINT32_MAX;// 100000;
    static const uint32_t GRAM_VALUE_LIMIT = UINT32_MAX;// 500;

    uint16_t order;

    typedef std::map<std::string, uint32_t> StrFreqMap;
    struct NextWord
    {
        // Frequency of next words
        StrFreqMap gramFreq;

        // Total of all frequencies in gramFreq
        // used for weighted random selection
        uint64_t sumOfWeights;
    };

    typedef std::map<std::string, NextWord> nGramType;
    nGramType nGrams;

    typedef std::vector<std::string> StringVec;
    StringVec tempStringVec;

    void splitString(std::string sentence);

    void sanitizeTempStringVec();

    bool digestLine(std::string line);
};

#endif
