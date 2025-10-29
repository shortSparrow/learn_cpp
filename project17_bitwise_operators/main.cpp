#include <bitset>
#include <cstdint>
#include <iostream>


/**
 * Bitwise operators (бітові оператори)
 * left shift	<<	x << n	the bits from x are shifted left by n positions, new bits are 0.
 * right shift	>>	x >> n	the bits from x are shifted right by n positions, new bits are 0.
 * bitwise NOT	~	~x	each bit from x is flipped.
 * bitwise AND	&	x & y	each bit is set when both corresponding bits in x and y are 1.
 * bitwise OR	|	x | y	each bit is set when either corresponding bit in x and y is 1.
 * bitwise XOR	^	x ^ y	each bit is set when the corresponding bits in x and y are different.
 *
 */

std::bitset<4> rotl(const std::bitset<4> bits) {
    const std::bitset<1> last_bit {bits[bits.size() - 1]};

    std::bitset<4> val_shifted {bits << 1};
    val_shifted.set(0, last_bit.test(0));
    return  val_shifted;
}

int main() {
    // LEFT SHIFT (Відбувається зсув з ліва на право на вказану кількість знаків)
    std::cout << "-----------LEFT SHIFT-----------" << "\n";
    std::bitset<4> x {0b0001};
    std::cout << (x << 1) << "\n"; // 0010
    std::cout << (x << 2) << "\n"; // 0100
    std::cout << (x << 3) << "\n"; // 1000
    std::cout << (x << 4) << "\n"; // 0000

    // RIGHT SHIFT (Відбувається зсув з права на ліво на вказану кількість знаків)
    std::cout << "-----------RIGHT SHIFT-----------" << "\n";
    std::bitset<4> y {0b1000};
    std::cout << (y >> 1) << "\n"; // 0100
    std::cout << (y >> 2) << "\n"; // 0010
    std::cout << (y >> 3) << "\n"; // 0001
    std::cout << (y >> 4) << "\n"; // 0000

    // Bitwise NOT (перетворює 0 на 1, і навпаки 1 на 0)
    std::cout << "-----------Bitwise NOT-----------" << "\n";
    std::bitset<1> a {0b0};
    std::cout << ~a << "\n"; // 1
    std::cout << ~~a << "\n"; // 0

    std::bitset<4> b {0b0010};
    std::cout << ~b << "\n"; // 1101
    std::cout << ~~b << "\n"; // 0010

    // Bitwise OR
    /**
     *  Bitwise OR (при застосуванні на двох бітових значеннях поверне нове, яке є результатом побітового порівняння бітів двох операндів)
     *      101
     *  OR
     *      010
     *      ---
     *      111
     * Якщо один з двох операндів 1 то результат буде 1, якщо обидва 0 тоді 0
     */
    std::cout << "-----------Bitwise OR-----------" << "\n";
    std::bitset<4> c {0b0011};
    std::bitset<4> d {0b1001};
    std::cout << (c | d) << "\n"; // 1011

    std::bitset<4> a1 {0b0111};
    std::bitset<4> a2 {0b0011};
    std::bitset<4> a3 {0b0001};
    std::cout << (a1 | a2 | a3) << "\n"; // 0111


    // Bitwise AND (Дуже схоже на роботу OR, але тільки логіка як з логічним and)
    std::cout << "-----------Bitwise AND-----------" << "\n";
    std::bitset<4> e {0b0101};
    std::bitset<4> f {0b0110};
    std::cout << (e & f) << "\n"; // 0100

    std::bitset<4> b1 {0b0111};
    std::bitset<4> b2 {0b0011};
    std::bitset<4> b3 {0b0001};
    std::cout << (b1 & b2 & b3) << "\n"; // 0001


    // Bitwise XOR (exclusive or - виключне або)
    // Коли лише один біт 1 тоді ставиться 1, якщо ж обидва біти однакові 1 або 0, то ставиться 0
    std::cout << "-----------Bitwise XOR-----------" << "\n";
    std::bitset<4> g {0b0110};
    std::bitset<4> h {0b0011};
    std::cout << (g ^ h) << "\n"; // 0101

    std::bitset<4> c1 {0b0111};
    std::bitset<4> c2 {0b0011};
    std::bitset<4> c3 {0b0001};
    std::cout << (c1 ^ c2 ^ c3) << "\n"; // 0101


    // Bitwise assignment operators (зробили операцію і просвоїли результат змінній)
    /**
     * left shift	<<	x <<= n	the bits in x are shifted left by n positions, new bits are 0.
     * right shift	>>	x >>= n	the bits in x are shifted right by n positions, new bits are 0.
     * bitwise AND	&	x &= y	each bit is set when both corresponding bits in x and y are 1.
     * bitwise OR	|	x |= y	each bit is set when either corresponding bit in x and y is 1.
     * bitwise XOR	^	x ^= y	each bit is set when the corresponding bits in x and y are different.
     *
     */
    std::cout << "-----------Bitwise assignment operators-----------" << "\n";
    std::bitset<4> bits { 0b0100 };
    bits >>= 1;
    std::cout << bits << '\n';



    /**
     * ЗАДАЧА
     * Потрібно зробити функцію rotl, яка буде робити left shiht, але зациклено. Наприклад
     * Left shift: 0b1001 << 1 -> 0b0010
     * rotl: rotl(0b1001) -> 0b0011
     *
     * rotl робить зсув тільки на 1
     *
     */
    std::cout << "-----------TASK-----------" << "\n";

    std::cout << rotl(0b0001) << "\n"; // 0010
    std::cout << rotl(0b1001) << "\n"; // 0011





    return 0;
}