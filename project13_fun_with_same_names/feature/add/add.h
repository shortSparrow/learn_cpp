#ifndef TEST_ADD_H
#define TEST_ADD_H

/**
 * Для того щоб не було помилки треба додати namespace.
 * Інакше буде конфлікт з двома int add(int,int) бо одна оголошена
 * у add/add.cpp, а друга add_2/add.cpp.
 *
 */

namespace A {
    int add(int x, int y);
}

#endif //TEST_ADD_H