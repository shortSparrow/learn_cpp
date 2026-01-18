#include <functional>
#include <iostream>


int binarySearchRecursive(const int* array, int target, int min, int max) {

    if (min > max) {
        return -1; // not found
    }

    const int median = min+(max-min)/2;
    const int candidate {array[median]};
    if (candidate == target) {
        return median;
    }

    if (target > candidate) {
        min=median+1;
    }

    if (target < candidate) {
        max=median-1;
    }

    return binarySearchRecursive(array, target, min, max);
}

int binarySearchIterable(const int* array, size_t length, int target) {
    int left {0};
    int right {static_cast<int>(length) - 1};


    do {
        int median = left+(right-left)/2;
        int candidate {array[median]};

        if (candidate == target) {
            return median;
        }

        if (candidate > target) {
            right = median-1;
        }

        if (candidate < target) {
            left = median+1;
        }
    } while (right >= left);

    return -1;

}


int main() {


    constexpr int array[]{ 3, 6, 8, 12, 14, 17, 20, 21, 26, 32, 36, 37, 42, 44, 48 };
    std::cout << binarySearchRecursive(array, 49, 0, std::size(array)-1); // -1
    std::cout << binarySearchRecursive(array, -3, 0, std::size(array)-1); // -1
    std::cout << binarySearchRecursive(array, 12, 0, std::size(array)-1); // 3
    std::cout << binarySearchRecursive(array, 48, 0, std::size(array)-1); // 14

    std::cout << '\n';

    int length {std::size(array)};
    std::cout << binarySearchIterable(array, length, 49); // -1
    std::cout << binarySearchIterable(array, length, -3); // -1
    std::cout << binarySearchIterable(array, length, 12); // 3
    std::cout << binarySearchIterable(array, length, 48); // 14



    return 0;
}