#include <iostream>

/**
 * Diamond problem
 * Це відома проблема, коли у нас є multiple inheritance, а саме коли ми успадковуємося від двох класів,
 * які мають спільного батька.
 *
 */

class PoweredDevice {
public:
    PoweredDevice(int power) {
		  std::cout << "PoweredDevice: " << power << '\n';
    }
};

class Scanner: public PoweredDevice {
public:
    Scanner(int scanner, int power)  : PoweredDevice{ power } {
		  std::cout << "Scanner: " << scanner << '\n';
    }
};

class Printer: public PoweredDevice {
public:
    Printer(int printer, int power) : PoweredDevice{ power } {
		  std::cout << "Printer: " << printer << '\n';
    }
};

class Copier: public Scanner, public Printer {
public:
    Copier(int scanner, int printer, int power) :
    Scanner{ scanner, power },
    Printer{ printer, power } {
      std::cout << "Copier \n";
    }
};


// ------------------------------------------------------------------------------------------------------------------------
/**
 * Той самий код, що і вверху, але із додаванням virtual base class.
 *
 * virtual base classes завжди створюються раніше за non-virtual, тобто це гарантує, що всі базові (child) класи
 * будуть створені раніше ніж батьківські (parent) класи.
 *
 * В нашому випадку саме CopierV створить PoweredDeviceV, бо він найбільш Derived клас серед усіх. А ScannerV
 * та PrinterV просто використають раніше створений екземпляр PoweredDeviceV.
 *
 */

class PoweredDeviceV {
public:
    PoweredDeviceV(int power) {
		  std::cout << "PoweredDevice: " << power << '\n';
    }
};

// note: PoweredDevice is now a virtual base class
class ScannerV: virtual public PoweredDeviceV {
public:
    ScannerV(int scanner, int power)
        : PoweredDeviceV{ power } // this line is required to create Scanner objects, but ignored in this case
    {
		  std::cout << "Scanner: " << scanner << '\n';
    }
};

// note: PoweredDevice is now a virtual base class
class PrinterV: virtual public PoweredDeviceV {
public:
    PrinterV(int printer, int power)
        : PoweredDeviceV{ power } // this line is required to create Printer objects, but ignored in this case
    {
		  std::cout << "Printer: " << printer << '\n';
    }
};

class CopierV: public ScannerV, public PrinterV {
public:
    CopierV(int scanner, int printer, int power)
        : PoweredDeviceV{ power }, // PoweredDevice is constructed here
        ScannerV{ scanner, power },
        PrinterV{ printer, power } {
    }
};

int main() {
    Copier copier { 1, 2, 3 };
    /**
     * Здавалося б, що у нас має викликатися 4 (чотири) конструктора PoweredDevice, Scanner, Printer, Copier, але
     * насправді викликалося 5 (п'ять) конструкторів. А саме, двічі викликався PoweredDevice, бо його викликали
     * і Scanner і Printer.
     *
     * PoweredDevice: 3
     * Scanner: 1
     * PoweredDevice: 3
     * Printer: 2
     * Copier
     *
     */


    /**
     * Якщо ми хочемо уникнути такої проблеми, то можна використати virtual base class
     * Це досягається через додавання спеціального слова virtual у успадкування
     */

    std::cout << "-----------------------------------------------------------\n";
    CopierV copier_v { 1, 2, 3 };


    return 0;
}