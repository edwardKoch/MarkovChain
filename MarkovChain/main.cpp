#include <iostream>

#include "MarkovChain.h"

int main()
{
    MarkovChain chatGPT(3);

    chatGPT.train("Unicorns.txt");

    //std::cout << chatGPT.predict("The unicorn").c_str() << std::endl;

    std::string question = "";

    while (question != "exit")
    {
        std::cout << "->";
        std::getline(std::cin, question);

        std::cout << chatGPT.predict(question).c_str() << "\n" << std::endl;
    }
}