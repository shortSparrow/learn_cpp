#include <iostream>
#include <vector>

// цей тип необхідний для передачі reference одного вектора у інший
using RefVector = std::vector<std::reference_wrapper<std::vector<int>>>;


void printArray(const std::vector<int>& v) {
    for (const int& item: v) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}

void printArrayOfArray(const std::vector<std::vector<int>> v) {
    for (const std::vector<int>& item: v) {
        printArray(item);
    }
}

// перевантажили функцію, щоб працювала для обох випадків (і масив масивів і масив посилань на масиви)
void printArrayOfArray(const RefVector& v) {
    for (const auto& item: v) {
        printArray(item);
    }
}



int main() {
    // // Тут просто створююється незалежна копія
    // std::vector v1 {1,2,3};
    // std::vector v2 {v1, {4,5,6}};
    // printArrayOfArray(v2); // 1 2 3   4 5 6
    // v1[0] = 0;
    // printArrayOfArray(v2); // 1 2 3   4 5 6



    // А тут вже передамо посилання на масив
    std::vector v1 {1,2,3};
    std::vector v2 {4,5,6};

    RefVector v3;
    v3.push_back(std::ref(v1));
    v3.push_back(std::ref(v2));

    printArrayOfArray(v3); // 1 2 3   4 5 6
    v1[0] = 0;
    printArrayOfArray(v3); // 0 2 3   4 5 6
    printArray(v1); // 0 2 3


    return 0;
}