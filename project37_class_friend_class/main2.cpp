#include <iostream>

class Storage2; // forward declaration for class Storage

class Display2 {
private:
    bool m_displayIntFirst {};

public:
    Display2(bool displayIntFirst)
        : m_displayIntFirst { displayIntFirst }
    {
    }

    void displayStorage(const Storage2& storage); // forward declaration for Storage needed for reference here
};

// full definition of Storage class
class Storage2 {
private:
    int m_nValue {};
    double m_dValue {};
public:
    Storage2(int nValue, double dValue)
        : m_nValue { nValue }, m_dValue { dValue }
    {
    }

    // Make the Display::displayStorage member function a friend of the Storage class
    // Requires seeing the full definition of class Display (as displayStorage is a member)
    friend void Display2::displayStorage(const Storage2& storage);
};

// Now we can define Display::displayStorage
// Requires seeing the full definition of class Storage (as we access Storage members)
void Display2::displayStorage(const Storage2& storage) {
    if (m_displayIntFirst)
        std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
    else // display double first
        std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
}

int main2() {
    Storage2 storage { 5, 6.7 };
    Display2 display { false };
    display.displayStorage(storage);

    return 0;
}

