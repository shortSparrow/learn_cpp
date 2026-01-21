#include <iostream>

/**
 * Аргументи функції main
 *
 * argc — це цілочисельний параметр, що містить кількість аргументів, переданих програмі
 * (тобто argc = кількість аргументів). argc завжди буде не менше 1, оскільки першим аргументом завжди
 * є назва самої програми. Кожен аргумент командного рядка, який надає користувач, збільшує argc на 1.
 *
 *
 * Command line arguments are always passed as strings, even if the value provided is numeric in nature
 *
 */
int main(int argc, char* argv[]) {

    std::cout << argc << '\n'; // 1
    std::cout << argv[0] << '\n'; // C:\Users\formo\OneDrive\Desktop\c++\trash\cmake-build-debug\project45_comand_line_arguments.exe
    std::cout << argv[1] << '\n';

    std::cin.get(); // Додано, щоб термінал не закривався миттєво у release mode (чекає на ввід даних від користувача)


    /**
     * Я зробив release build і він знаходиться за адресою
     * C:\Users\formo\OneDrive\Desktop\c++\project45_comand_line_arguments\cmake-build-release\project45_comand_line_arguments.exe
     *
     * Тож якщо я відкрию термінал і у директорії C:\Users\formo\OneDrive\Desktop\c++\project45_comand_line_arguments\cmake-build-release
     * і запущу project45_comand_line_arguments (на windows просто напишу в терміналі project45_comand_line_arguments.exe) то побачу терміналі:
     *      1
     *      project45_comand_line_arguments.exe
     *
     * Якщо ж я напишу у терміналі project45_comand_line_arguments.exe "Hello world", то побачу:
     *      2
     *      project45_comand_line_arguments.exe
     *      Hello world
     *
     * Якщо ж я напишу у терміналі project45_comand_line_arguments.exe Hello world, то побачу:
     *      3
     *      project45_comand_line_arguments.exe
     *      Hello
     *
     *
     */

    return 0;
}