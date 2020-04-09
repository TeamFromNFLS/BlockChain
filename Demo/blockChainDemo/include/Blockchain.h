#ifndef DEMO_BLOCKCHAIN_H
#define DEMO_BLOCKCHAIN_H
#include"Block.h"
#include<vector>

class Blockchain {
public:
    Blockchain();
    void AddBlock(Block bNew);
private:
    uint32_t _nDifficulty;
    vector<Block> _vChain;
    Block _GetLastBlock() const; //获取最新的区块，由const关键字，表示输出的内容不可更改
};

#endif