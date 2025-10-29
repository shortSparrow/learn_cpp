
// Функції не потрібен extern, функції мають external linkage за замовченням
int add(int x, int y) {
    return  x + y;
}

// Non-const global variables are external by default, so we don’t need to mark them as extern
extern const int a = 1; // const globals can be defined as extern, making them external
const int b = 2; // Немає extern - не можна отримати у main.cpp
int c = 3; // extern за замовченням
extern constexpr int d = 4; // constexpr globals can be defined as extern, making them external (but this is pretty useless)

