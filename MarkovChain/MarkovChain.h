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

class MarkovChain
{
public:
    MarkovChain(uint16_t n);

    void train(std::string fileName);

    std::string predict(std::string question);

private:
    static const uint32_t GRAM_KEY_LIMIT = 100000;
    static const uint32_t GRAM_VALUE_LIMIT = 500;

    uint16_t order;

    typedef std::vector<std::string> StringVec;

    typedef std::map<std::string, StringVec> nGramType;
    nGramType nGrams;

    StringVec tempStringVec;

    void splitString(std::string sentence);

    void sanitizeTempStringVec();

    bool digestLine(std::string line);
};

#endif
