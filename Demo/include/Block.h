#ifndef DEMO_BLOCK_H
#define DEMO_BLOCK_H
#include<cstdint>
#include<iostream>
using namespace std;
 
class Block {
public:
    string sPrevHash;
    Block(uint32_t nIndexIn, const string &sDataIn);
    string GetHash();
    void MineBlock(uint32_t nDifficulty); //挖矿，其参数nDifficulty表示指定的难度值

private:
    uint32_t _nIndex; //区块索引值，第几个区块，从0开始计算
    int64_t _nNonce; //区块随机数
    string _sData; //区块描述字符
    string _sHash; //区块Hash值
    time_t _tTime; //区块生成时间
    string _CalculateHash() const; //计算Hash值
 
};

#endif