#ifndef MD5_H
#define MD5_H
#include <string>
using namespace std;
typedef unsigned char byte;
typedef unsigned long ulong;

class MD5
{
public:
    /*clarification of MD5 */
    MD5(){};
    MD5(string str);
    ~MD5(){};

    string md5(); // output the encrypted result

private:
    void Init(const byte *input, size_t length); // initiate an object, put the string into the buffer
    void final();

    /*encrypt functions*/
    void transform(const byte block[64]);
    void encode(const ulong *input, byte *output, size_t length);

    bool finished{false};                                              // check whether the calculation is finished
    ulong count[2] = {0};                                              // number of bits
    ulong state[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}; // four magic constants

    /*static constants*/
    static constexpr size_t BufferSize = 1024;
};

#endif