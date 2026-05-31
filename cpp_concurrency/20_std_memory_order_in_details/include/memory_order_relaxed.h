#ifndef INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ORDER_RELAXED_H
#define INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ORDER_RELAXED_H

namespace order_relaxed {
    void write_x();

    void write_y();

    void read_x_then_y();

    void read_y_then_x();

    void run_code();

} // namespace order_relaxed

#endif //INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ORDER_RELAXED_H