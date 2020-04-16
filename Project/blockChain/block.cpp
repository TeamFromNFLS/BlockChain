#include <iostream>
#include <cstring>
#include <ctime>
#include "block.h"
#include "sha256.h"

void Block::init()
{
    time = std::time(0);
    std::string preBlock = "123456789";
    std::string transRoot = "abcdefg";
    uint32_t preHeight = 10;
    height = preHeight++;
    preBlockHash = sha256(preBlock);
    merkleRoot = sha256(transRoot);
    return;
}