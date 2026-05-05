#include <iostream>


/**
 * А що буде якщо наша програма викличе exception і не буде catch який його зловить?
 * Тоді викликається std::terminate() і додаток припиняє роботу.
 * У деяких випадках після цього стек можу бути або не бути розгорнутим (unwound). Коли він не розгорнутий,
 * то всі змінні зберігаються і ніякої очистки не буде, а коли розгорнутий то всі змінні очищаються.
 * Насправді доволі корисно може бути, щоб не було очистки, адже тоді ми втратимо інформацію про неочікувану помилку.
 *
 * Якщо стається неочікувана помилка, то система все одно покаже якесь сповіщення, у консолі це може бути текст, а
 * на операційній системі діалогове вікно, що система отримала неочікувану помилку.
 *
 */

struct GameSession {
    // Game session data here
};

void runGame(GameSession&) {
    throw 1;
}

void saveGame(GameSession&) {
    // Save user's game here
}

/**
 * Цей клас немає конструктора тому ніколи не буде створений, і він потрібний лише щоб мати змогу додати якийсь
 * catch коли ми у debug моді прибираємо catch(...) то маємо додати якийсь інший, щоб код лишався консистентним,
 * тобто у нас все одно буде catch, але він ніколи не буде викликаний. Взагалі рішення сумнівне, я його додав
 * лише щоб показатися як можна додавати #ifdef для вбудовування коду під різні моди (debug/release)
 */
class DummyException  {
    DummyException() = delete;
};

int main() {
    // Якщо ми не знаємо який у нас буде тип помилки, або хочемо мати один обробник на все то можна використати оператор ...
    // Хорошою практикою буде обгорнути точку входу (main) у try...catch разом із catch-all handler  (...) для обробки неочікуваних помилок
    try {
        throw 5; // throw an int exception
    } catch (double x) {
        std::cout << "We caught an exception of type double: " << x << '\n';
    } catch (...) { // catch-all handler - відловить всі типи помилок
        std::cout << "We caught an exception of an undetermined type\n";
    }







    /**
     * У дебагу під час розробки нам може заважати catch-all, тому його можна відключити для моду debug.
     * Бо якщо не відключити то треба обов'язково ставити на цей catch break point інакше ми просто проґавимо помилку,
     * адже коли станеться неочікувана помилка то вона потрапить у catch-all і все, піде кудись у лог абощо, і ми її можемо
     * пропустити. Саме тому одним з рішень може буде відключити цей catch-all на дебаг моді
     */

    GameSession session {};

    try {
        runGame(session);
    }
#ifdef NDEBUG // if we're in release node
    catch(...) { // compile in the catch-all handler
        std::cerr << "Abnormal termination\n";
    }
#else // in debug mode, compile in a catch that will never be hit (for syntactic reasons)
    catch(DummyException) {}
#endif

}