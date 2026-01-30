#include <iostream>

/**
 * Smart pointers
 * A smart pointer class is a composition class that is designed to manage dynamically allocated memory,
 * and ensure that memory gets deleted when the smart pointer object goes out of scope.
 *
 * Проблема зі звичайними pointers у тому, що якщо ми динамічно алокуємо пам'ять через new
 * то там потрібно буде пам'ятати, щоб деалокувати її. А це може бути важко, особливо коли код змінюється
 * і може додатися якась умова яка призведе до return у функції до моменту деалокації (delete my_pointer).
 *
 * Саме для вирішення цієї проблеми і існують smart pointers. Щоб зрозуміти як вони працюють розглянемо
 * самописний приклад з класом. Ми пам'ятаємо, що у класу є деструктор (destructor) який викличеться автоматично
 * перед знищенням класу, тож якщо туди записати очищення алокованої пам'яті, то нам не треба буде про це пам'ятати
 */


template <typename T>
class Auto_ptr1 {
    T* m_ptr {};
public:
    Auto_ptr1(T* ptr=nullptr) :m_ptr(ptr) {
    }

    ~Auto_ptr1() {
        delete m_ptr;
    }

    // Overload dereference and operator-> so we can use Auto_ptr1 like m_ptr.
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
};


/**
 *
 * У C++ є:
 *  L-values: Об'єкти, що мають ім'я та адресу в пам'яті (наприклад, змінна int x). Вони "живуть" довго.
 *  R-values: Тимчасові значення, які зазвичай зникають після завершення виразу (наприклад, число 5 або результат виразу x + y).
 *
 * R-value reference (&&) — це спеціальний тип посилання, який дозволяє нам "вхопитися" за цей тимчасовий об'єкт.
 *
 * У виразі int a = 10; змінна a - це L-value
 * У виразі int b = a + 5; результат обчислення a + 5 - це R-Value
 *
 * Move Constructor та Move Assignment (Конструктор переміщення)
 * Тепер, коли ми знаємо, що таке r-value (тимчасові об'єкти), ми підходимо до "магії" переміщення.
 * Проблема: Раніше, якщо нам треба було передати великий масив даних із тимчасового об'єкта в постійний, C++ робив повну копію
 *
 * Рішення (Move Semantics): Замість копіювання ми просто "крадемо" вказівник на дані.
 * 1) Ми беремо адресу даних у тимчасового об'єкта.
 * 2) Присвоюємо її нашому новому об'єкту.
 * 3) У тимчасового об'єкта зануляємо вказівник (щоб він при видаленні не знищив наші дані).
 *
 * Важливий інструмент — std::move(): Це функція-перетворювач. Вона не переміщує дані сама, вона просто каже компілятору:
 * "Гей, подивись на цей l-value (постійну змінну) так, ніби це r-value (тимчасовий об'єкт), щоб ми могли його пограбувати через Move Constructor".
 *
 * Вправа-загадка: Уяви, що в тебе є клас String, який тримає всередині дуже довгий текст. Ти написав такий код:
 * String a = "Дуже довгий текст..."; String b = std::move(a);
 * Змінна a стане порожньою (її дані "переїдуть" до b).
 *
 *
 * Smart Pointers та Move Semantics
 * Це фінальна частина пазла. Розумні вказівники (Smart Pointers), як-от std::unique_ptr, створені для того,
 * щоб автоматично видаляти об'єкт з пам'яті, коли він більше не потрібен.
 *
 * Ключова фішка std::unique_ptr: Він унікальний. Це означає, що за правилами C++ його не можна копіювати.
 * Якби ми його скопіювали, то два вказівники вважали б, що вони обидва володіють одним об'єктом, і спробували б
 * видалити його двічі (що призвело б до катастрофи/крашу).
 * Ось тут на допомогу приходить Move Semantics: Хоча ми не можемо скопіювати unique_ptr, ми можемо його перемістити.
 *
 * Ось як це виглядає в коді:
 * std::unique_ptr<int> res1 = std::make_unique<int>(42);
 * // std::unique_ptr<int> res2 = res1; // ПОМИЛКА! Копіювання заборонено.
 * std::unique_ptr<int> res2 = std::move(res1); // ПРАВИЛЬНО. res1 тепер порожній, res2 володіє числом 42.
 *
 *
 * Розглянемо детальніше Auto_ptr5
 * Клас Auto_ptr5 це реалізація std::unique_ptr ("розумного вказівника", який вміє використовувати логіку переміщення.)
 * Auto_ptr5(Auto_ptr5&& a) - тут параметр "a" як раз і є R-Value
 *
 * Auto_ptr5<Item> getWeapon() {
 *  Auto_ptr5<Item> temp(new Item);
 *  return temp; // Ось тут народжується тимчасовий об'єкт!
 * }
 *
 * У головній програмі:
 * Auto_ptr5<Item> mainWeapon = getWeapon();
 * Що тут відбувається?
 *  1) Функція getWeapon закінчує роботу. Змінна temp має "померти".
 *  2) Але нам треба передати цей вказівник у mainWeapon.
 *  3) Замість того, щоб копіювати всі дані (що заборонено), компілятор бачить: "О, результат getWeapon() — це
 *     тимчасовий об'єкт (r-value). Він все одно зараз зникне!"
 *  4) Компілятор викликає твій Move constructor (Auto_ptr5(Auto_ptr5&& a)).
 *  5) Твій конструктор "краде" вказівник у цього вмираючого тимчасового об'єкта.
 */

template<typename T>
class Auto_ptr5 {
    T* m_ptr {};
public:
    Auto_ptr5(T* ptr = nullptr)
        : m_ptr { ptr } {
    }

    ~Auto_ptr5() {
        delete m_ptr;
    }

    // Copy constructor -- no copying allowed!
    Auto_ptr5(const Auto_ptr5& a) = delete;

    // Move constructor
    // Transfer ownership of a.m_ptr to m_ptr
    Auto_ptr5(Auto_ptr5&& a) noexcept
        : m_ptr { a.m_ptr } {
        a.m_ptr = nullptr;
    }

    // Copy assignment -- no copying allowed!
    Auto_ptr5& operator=(const Auto_ptr5& a) = delete;

    // Move assignment
    // Transfer ownership of a.m_ptr to m_ptr
    Auto_ptr5& operator=(Auto_ptr5&& a) noexcept {
        // Self-assignment detection
        if (&a == this)
            return *this;

        // Release any resource we're holding
        delete m_ptr;

        // Transfer ownership of a.m_ptr to m_ptr
        m_ptr = a.m_ptr;
        a.m_ptr = nullptr;

        return *this;
    }

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    bool isNull() const { return m_ptr == nullptr; }
};


class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
    void sayHi() { std::cout << "Hi!\n"; }
};


void someFunction() {
    Auto_ptr1<Resource> ptr(new Resource()); // ptr now owns the Resource

    int x;
    std::cout << "Enter an integer: ";
    std::cin >> x;

    if (x == 0)
        return; // the function returns early

    // do stuff with ptr here
    ptr->sayHi();
}

int main() {

    /**
     * Розглянемо, як працює ця програма. Спочатку ми динамічно створюємо ресурс і передаємо його як параметр до нашого
     * шаблонного класу Auto_ptr1. З цього моменту наша змінна Auto_ptr1 res володіє цим об'єктом ресурсу.
     * Оскільки res оголошена як локальна змінна і має область дії блоку, вона вийде за межі області дії,
     * коли блок закінчиться, і буде знищена (не потрібно турбуватися про те, що ви забудете звільнити її).
     * А оскільки це клас, коли він буде знищений, буде викликаний деструктор Auto_ptr1. Цей деструктор забезпечить,
     * що покажчик ресурсу, який він утримує, буде видалений!
     */

    Auto_ptr1<Resource> res(new Resource()); // Note the allocation of memory here
    /**
     * Виведе у термінал
     * Resource acquired
     * Resource destroyed
     */

    someFunction();
    /**
     * Resource acquired
     * Enter an integer:5
     *
     * Hi!
     * Resource destroyed
     */


    /**
     * Але цей клас має один великий недолік, ми не перевизначили оператор = і конструктор копіювання,
     * а отже компілятор створить дефолті за нас, і тут виникне проблема. У прикладі нижче викличеться
     * конструктор копіювання.
     *
     * Отже, коли ми ініціалізуємо res2 за допомогою res1, обидві змінні Auto_ptr1 вказують на один і той самий ресурс.
     * Коли res2 виходить за межі області дії, він видаляє ресурс, залишаючи res1 з висячим (dangling) покажчиком.
     * Коли res1 намагається видалити свій (вже видалений) ресурс, це призведе до невизначеної поведінки (ймовірно, до збою)!
     *
     *
     * Resource acquired
     * Resource destroyed
     * Resource destroyed
     */
    Auto_ptr1<Resource> res1(new Resource());
    Auto_ptr1<Resource> res2(res1); // Alternatively, don't initialize res2 and then assign res2 = res1;



    //*************************************************
    /**
     * Друга проблема, припустимо у мене є функція яка має повернути Auto_ptr1, як їй це зробити
     *
     * ??? generateResource() {
     *  Resource* r{ new Resource() };
     *  return Auto_ptr1(r);
     * }
     *
     * Повернути як reference ми не можемо, бо коли функція знищиться pointer теж очиститься
     * Повернути як pointer (Resource*), (ми створимо об'єкт десь у heap (купі) і повернемо цього адресу) але тоді ми можемо забути його очистити пізніше
     * Повернути як значення, але це зробить дорогу копію і втрачається сенс smart pointer
     *
     * Правильним варіантом буде Move semantics - ми перемістимо володіння об'єктом до іншого власника, а у поточного видалимо
     *
     */


    /**
     * Auto_ptr5 є більш повною копією std::unique_ptr, тобто це наочна ілюстрація що під капотом у
     * std:unique_ptr
     */




    /**
     * Додаткова інформаційна вижимка по smart pointers
     *
     * Хоча розумні покажчики (smart pointer) можуть пропонувати й інші функції, їхньою визначальною характеристикою є те,
     * що вони керують динамічно виділеними ресурсами, наданими користувачем розумного покажчика (smart pointer),
     * і забезпечують належне очищення динамічно виділеного об'єкта в потрібний час (зазвичай, коли розумний покажчик (smart pointer)
     * виходить за межі області дії).
     */






    return 0;
}