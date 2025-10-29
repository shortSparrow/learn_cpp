#include <iostream>
#include <vector>
using namespace std;

int main() {

    /**
     * Напишіть коротку програму для імітації падіння м'яча з вежі.
     * Для початку користувача слід попросити вказати висоту вежі в метрах.
     * Припустимо, що сила тяжіння нормальна (9,8 м/с^2), і що м'яч не має початкової швидкості
     * (м'яч не рухається на початку). Нехай програма виведе висоту м'яча над землею через 0, 1, 2, 3, 4 та 5 секунд.
     * М'яч не повинен опускатися під землю (висота 0).
     *
     * distance_fallen = gravity_constant * x_seconds^2 / 2
     */
    const float gravity_constant = 9.8;
    pmr::vector<int> seconds {0,1,2,3,4,5};
    int tower_height = {};
    cout << "Enter the height of the tower in meters: ";
    cin >> tower_height;

    for (int sec: seconds){
        float distance_fallen = gravity_constant * (sec*sec)/2;
        float left_height = tower_height - distance_fallen;

        if (left_height <=0) {
            cout << "At "<< sec << " seconds, the ball is on the ground" << endl;
        } else {
            cout << "At "<< sec << " seconds, the ball is at height: " << left_height << " meters" << endl;
        }

    }
    return 0;
}