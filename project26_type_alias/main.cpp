#include <iostream>
#include <vector>

int main() {

    /**
     * alias сам не є типом, в кінці Distance заміниться на double,
     * але це може бути зручна річ коли треба розуміти що саме ти зберігатимеш у змінній
     */

    using Distance = double; // define Distance as an alias for type double
    Distance milesToDestination{ 3.4 }; // defines a variable of type double
    std::cout << milesToDestination << '\n'; // prints a double value



    /**
     * Typedefs - це старий спосіб створити alias. Ці два рядки ідентичні
     *     typedef long Miles;
     *     using Miles = long;
     */



    // Alias корисні коли треба написати великий складний тип більш просто
    using VectPairSI = std::vector<std::pair<std::string, int>>;
    VectPairSI pairlist;






    return 0;
}
