
#ifndef INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ACQUIRE_RELEASE_H
#define INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ACQUIRE_RELEASE_H


namespace order_acquire_release {
    void write_x();

    void write_y();

    void read_x_then_y();

    void read_y_then_x();

    void run_code();
}



#endif //INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ACQUIRE_RELEASE_H
