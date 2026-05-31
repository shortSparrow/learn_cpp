#ifndef INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ORDER_SEQ_CST_H
#define INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ORDER_SEQ_CST_H


namespace order_seq_cst {
    void write_x();

    void write_y();

    void read_x_then_y();

    void read_y_then_x();

    void run_code();
};


#endif //INC_20_STD_MEMORY_ORDER_IN_DETAILS_MEMORY_ORDER_SEQ_CST_H
