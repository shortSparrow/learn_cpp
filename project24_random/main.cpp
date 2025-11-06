#include <iostream>

#include "base/base.h"
#include "random_by_time/random_by_time.h"
#include "random_by_random_device/random_by_random_device.h"
#include "global_random/global_random.h"

/**
 * Основна ідея того, як працюють псевдо випадкові генератори чисел PRNG представлена
 * у функції base(). Воно бере випадкове число (seed) і робить з ним різні математичні
 * маніпуляції, так щоб воно набуло нових значень. Можливі різні розподіли випадковості,
 * але нас цікавить лише один - uniform distribution (нормальний розподіл). Це розподіл,
 * коли кожен елемент має однакову випадковість, наприклад підкидання монети, при 1000 спроб
 * у нас буде розподіл приблизно 50 на 50.
 *
 * Звідки взяти випадкове число (seed)?
 * У нас є два варіанти, як його отримати:
 *      1. Поточна дата (вона постійно змінюється). Хоч це і не зовсім випадкове число, однак при
 *         кожному запуску воно буде різне, і це те, чого ми хочемо.
 *      2. std::random_device() Воно просить операційну систему надати унікальне число, те як воно це робить
 *         залежить від конкретної системи
 *
 * По правильному ми маємо один раз надати seed і потім на його основі генерувати випадкові числа.
 * Погана ідея ре-ініціалізувати state (давати новий seed) бо це може зменшити випадковість.
 *
 *
 * ВАЖЛИВЕ ЗАУВАЖЕННЯ ДО ПОТОЧНОГО КОДУ у random_by_time та random_by_random_device
 * У них використовується PRNG std::mt19937, який ініціалізується зп допомогою 624-ьох integers,
 * натомість ми його ініціалізуємо лише 1 integer, тож бібліотека намагається сама дозаповнити
 * решту 623 числа, але вона не може творити дива, тож як результат можемо отримати не зовсім
 * випадкові числа, тому краще так не робити. Наприклад якщо seed буде лише одним 32-бітним числом,
 * то std::mt19937 ніколи не поверне число 42 як перший output.
 *
 * В ідеалі seed sequence має бути має мати стільки ж біт, скільки і state обраного PRNG, але у деяких
 * PRNG state надто великий. Тут на допомогу приходить std::seed_seq
 * Ми передаємо йому стільки seed, скільки у нас є, і він автоматично доставить решту, тобто у нашому випадку,
 * якщо у нас є 1 seed і ми обрали std::mt19937, то він доставить 623 seeds. Це не додасть випадковості, але
 * я так розумію, що це кращі ніж 0 і 1 які може доставити PRNG.
 * Приклад
 * 	std::random_device rd{};
 *
 * 	// get 8 integers of random numbers from std::random_device for our seed
 * 	std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
 *
 * 	std::mt19937 mt{ ss }; // initialize our Mersenne Twister with the std::seed_seq
 *
 * Одразу виникає питання, чому б не запустити std::random_device 624 рази?
 * Відповідь: Бо це може бути повільно і також це може виснажити список чисел з std::random_device.
 * Звісно можна використати інші випадкові числа, щоб збільшити кількість seeds, наприклад взяти
 * поточний час, або номер thread або який id користувача, тощо.
 *
 *
 *
 * Коли PRNG отримує seed низької якості (або недостатню кількість seeds), початкові результати
 * PRNG можуть бути низької якості. З цієї причини деякі PRNG виграють від «розігріву», тобто техніки,
 * при якій перші N результатів, згенерованих PRNG, відкидаються.
 * Це дозволяє змішати внутрішній стан PRNG таким чином, що наступні результати будуть вищої якості.
 * Зазвичай відкидається від декількох сотень до декількох тисяч початкових результатів.
 * Чим довший період PRNG, тим більше початкових результатів слід відкинути.
 *
 *
 *
 *
 * Можливості рандомізації в C++ доступні через заголовок <random> стандартної бібліотеки.
 * У бібліотеці random доступно 6 сімейств генераторів випадкових чисел (PNG) (станом на C++20):
 *
 *  Type name	    Family                              Period	    State size*	    Performance	    Quality	    Should I use this?
 *  minstd_rand	    Linear congruential generator	    2^31	    4 bytes	        Bad	            Awful	    No
 *  minstd_rand0
 *
 *  mt19937	        Mersenne twister	                2^19937	    2500 bytes	    Decent	        Decent	    Probably (see next section)
 *  mt19937_64
 *
 *  ranlux24        Subtract and carry	                10^171	    96 bytes	    Awful	        Good	    No
 *  ranlux48
 *
 *  knuth_b	        Shuffled linear congruential    	2^31	    1028 bytes	    Awful	        Bad	        No
 *                  generator
 *
 *  default_random_engine Any of above (implementation defined)	Varies	Varies	    ?	            ?	        No^2
 *
 *  rand()	        Linear congruential generator	    2^31	    4 bytes	        Bad	            Awful	    No^no
 *
 *
 *
 *
 *
 * Додатково почитати
 * https://gist.github.com/imneme/540829265469e673d045
 * https://www.pcg-random.org/posts/ease-of-use-without-loss-of-power.html
 *
 *
 */
int main() {
    std::cout << "---------------------BASE-------------------" << "\n";
    base();

    std::cout << "---------------------RANDOM BY TIME-------------------" << "\n";
    random_by_time();

    std::cout << "---------------------RANDOM BY RANDOM DEVICE-------------------" << "\n";
    random_by_random_device();

    std::cout << "---------------------GLOBAL RANDOM-------------------" << "\n";
    // Це глобальний генератор на весь проєкт, який ініціалізований лише один раз
    std::cout << Random::get(1,6) << "\n"; // returns int between 1 and 6
    std::cout << Random::get(1u,6u) << "\n"; // returns unsigned int between 1 and 6

    return 0;
}