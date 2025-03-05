#include <iostream>

#include "MarkovChain.h"

int main()
{
    MarkovChain chatGPT(6);

    chatGPT.train("The unicorn is a legendary creature that has been described since antiquity as a beast with a large, pointed, spiraling horn projecting from its forehead. The unicorn was depicted in ancient seals of the Indus Valley Civilization and was mentioned by the ancient Greeks in accounts of natural history by various writers, including Ctesias, Strabo, Pliny the Younger, and Aelian.[1] The Bible also describes an animal, the re'em, which some translations have erroneously rendered with the word unicorn.[1] In European folklore, the unicorn is often depicted as a white horse-like or goat-like animal with a long horn and cloven hooves (sometimes a goat's beard). In the Middle Ages and Renaissance, it was commonly described as an extremely wild woodland creature, a symbol of purity and grace, which could only be captured by a virgin. In the encyclopedias its horn was said to have the power to render poisoned water potable and to heal sickness. In medieval and Renaissance times, the tusk of the narwhal was sometimes sold as unicorn horn.");

    //std::cout << chatGPT.predict("The unicorn").c_str() << std::endl;

    std::string question = "";

    while (question != "exit")
    {
        std::getline(std::cin, question);

        std::cout << chatGPT.predict(question).c_str() << std::endl;
    }
}