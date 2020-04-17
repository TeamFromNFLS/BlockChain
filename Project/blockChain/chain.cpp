#include <iostream>
#include <cstdlib>
#include "chain.h"
#include "block.h"

void Chain::print()
{
    while (end != nullptr)
    {
        end->show();
        end = end->preBlock;
    }
}

void Chain::SetDifficulty(uint32_t difficultyTarget)
{
    difficulty = difficultyTarget;
}