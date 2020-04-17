#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <iostream>
#include <cstring>
#include <ctime>

class Block
{
public:
    void init();
    Block() : nonce(1)
    {
        init();
    }
    Block(int _nonce) : nonce(_nonce)
    {
        init();
    }
    Block(const Block &p) : preBlockHash(p.preBlockHash),
                            merkleRoot(p.merkleRoot),
                            time(p.time),
                            difficultyTarget(p.difficultyTarget),
                            height(p.height),
                            nonce(p.nonce){};
    void SetDifficultyTarget(uint32_t difficulty)
    {
        difficultyTarget = difficulty;
    }
    void SetNonce(uint32_t _nonce)
    {
        nonce = _nonce;
    }
    std::string GetHash();
    void show() const
    {
        std::string now = ctime(&time);
        std::cout << height << std::endl
                  << preBlockHash << std::endl
                  << merkleRoot << std::endl
                  << now << nonce << std::endl;
    }
    Block *preBlock = nullptr;

private:
    uint32_t version = 0x00;
    std::string preBlockHash;
    std::string merkleRoot;
    time_t time;
    uint32_t difficultyTarget;
    uint32_t height;
    uint32_t nonce;
};

#endif //BLOCK_H