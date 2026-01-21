#include <iostream>

int g_counter{ 0 };

void eatStack() {
    std::cout << ++g_counter << ' ';

    // We use a conditional here to avoid compiler warnings about infinite recursion
    if (g_counter > 0)
        eatStack(); // note that eatStack() calls itself

    // Needed to prevent compiler from doing tail-call optimization
    std::cout << "hi";
}

/**
 * Невеликий офтоп. А як змінні зберігаються у С++ при компіляції?
 *
 * Під час компіляції компілятор розмістить усі звичайні змінні в пам'яті як зміщення від деякої базової
 * адреси (яка буде надана під час виконання). Отже, компілятор скаже, що x знаходиться за адресою (базова адреса + 4),
 * y — за адресою (базова адреса + 8) тощо. Потім, де б компілятор не побачив x у нашому вихідному коді, він генерує
 * скомпільований код, який замість цього використовує (базова адреса + 4). Таким чином, компілятор не повинен турбуватися про те,
 де саме в пам'яті має знаходитися x, йому просто потрібно переконатися, що всі використовувані змінні достатньо відокремлені,
 щоб не перекриватися, а фактичні кінцеві адреси обчислюються під час виконання. Наприклад, коли викликається функція,
 що містить змінні, базова адреса присвоюється — скажімо, за адресою пам'яті 136. І оскільки компілятор розмістив x за адресою
 (базова адреса + 4), виконуваний код отримає доступ до адреси пам'яті (136 + 4), тобто адреси пам'яті 140.
 *
 */
int main() {
    /**
     *
     * The call stack segment
     *
     * The call stack segment holds the memory used for the call stack.
     * When the application starts, the main() function is pushed on the call stack by
     * the operating system. Then the program begins executing.
     *
     * Тобто наша main function на початку кладеться у стек і оскільки у стеку діє правило First in last out то
     * main закінчить роботу останньою. Далі коли система бачить наступну функцію то вона кладе її у стек, і виконує,
     * коли виконання закінчилося то система повертається на місце з якого почала виконувати функцію і йде далі.
     *
     * The stack has advantages and disadvantages:
     *  - Allocating memory on the stack is comparatively fast.
     *  - Memory allocated on the stack stays in scope as long as it is on the stack. It is destroyed when it is popped off the stack.
     *  - All memory allocated on the stack is known at compile time. Consequently, this memory can be accessed directly through a variable.
     *  - Because the stack is relatively small, it is generally not a good idea to do anything that eats up lots of stack space.
     *    This includes allocating or copying large arrays or other memory-intensive structures.
    */

    /**
     * This program tries to allocate a huge (likely 40MB) array on the stack.
     * Because the stack is not large enough to handle this array, the array allocation
     * overflows into portions of memory the program is not allowed to use.
     *
     * Якщо ми дійсно хочемо мати C-style array з 10000000 який займатиме близько 40МБ то
     * можна алокувати пам'ять не у stack, а у heap за допомогою new, тоді помилка не буде.
     * Бо обмеження по пам'яті є лише у stack (на різних системах по різному, але по кілька мега байт)
     */
    // int stack[10000000];
    // std::cout << "hi" << stack[0]; // we'll use stack[0] here so the compiler won't optimize the array away
    // Process finished with exit code -1073741571 (0xC00000FD)

    //************************************************************

    /**
     * Тут теж буде переповнення, але що саме «з’їдає» пам'ять у цьому прикладі?
     * Навіть якщо ти не створюєш великих масивів, кожен виклик функції eatStack() створює
     * так званий Stack Frame (кадр стека). Це маленька ділянка пам'яті, яка резервується спеціально
     * для цього конкретного виклику.
     *
     * Ось що туди потрапляє кожного разу, коли функція викликає саму себе:
     *      Адреса повернення: Комп'ютер має запам'ятати, куди йому «повернутися» (в який рядок коду),
     *                         коли функція закінчить свою роботу.
     *      Параметри функції: Хоча у eatStack() немає аргументів, якби вони були (наприклад, int x),
     *                         вони б копіювалися в кожен новий кадр.
     *      Локальні змінні: Будь-яка змінна всередині { } функції жила б у цьому кадрі.
     *      Службова інформація: Регістри процесора, які треба зберегти, щоб не втратити стан попередньої функції.
     *
     * Останнє що виводить до помилки 43202
     *
     * Відповідь Gemini:
     * При кожному запуску результат дещо різний, причина цьому ASLR (Address Space Layout Randomization)
     * Це технологія безпеки, яку використовують майже всі сучасні ОС (Windows, Linux, macOS).
     * Вона рандомізує адреси пам'яті, де розміщуються стек, купа та код програми при кожному її запуск
     *
     * Навіщо це треба?
     * Щоб хакерам було важче вгадати, де саме в пам'яті лежать важливі дані, і здійснити атаку
     * (наприклад, той самий exploit через переповнення буфера).
     *
     * Як це впливає?
     * Через це початкова адреса, з якої починає "рости" стек, щоразу трохи інша, і це може впливати на те,
     * скільки саме кадрів туди влізе до зіткнення з "кордоном".
     *
     */
    eatStack();
    return 0;
}
