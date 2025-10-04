
#ifndef PROJECT12_HEADERS_ADD_H
#define PROJECT12_HEADERS_ADD_H

int add(int x, int y);

#endif //PROJECT12_HEADERS_ADD_H


/**
 * #ifndef PROJECT12_HEADERS_ADD_H - це Header guards
 *
 * Уявимо що "add.h" має окрім add(int, int) ще один forward declaration,
 * припустимо ми тут робимо #include "sub.h", а sub.h містить sub(int, int)
 *
 * Тепер main.cpp робить #include "add.h" і "sub.h". І тут виникає проблема бо main.cpp
 * буде мати два sub(int, int), один з sub.h, а другий з add.h (і хоча тут він теж береться з
 * sub.h, але лінкеру на це все одно). Отож якщо main.cpp має два sub(int, int) він видасть помилку.
 *
 * Щоб цього уникати використовують Header guards. Іхня суть проста, якщо цей хедер вже було
 * вставлено через #include, то ifndef не спрацює і не задублює forward declaration.
 *
 * Header guards повинен містити кожен .h файл для захисту
 *
 *
 */