#include <iostream>
#include <cstdint>
#include <bitset>

void func(uint8_t* ptr, size_t lenghtByte){
    for (auto i=0; i<lenghtByte; i++)
        ptr[i] = ~ptr[i];    //ptr[i] ^= (1 << i);
}

int main()
{
    uint8_t buf[] = {0b10101010, 0b01010101, 0b11001100};
    func(buf, sizeof(buf));
    for (uint8_t b : buf)
        std::cout << std::bitset<8>(b);
    return 0;
}
