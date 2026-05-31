#include "include/memory_order_seq_cst.h"
#include "include/memory_order_relaxed.h"
#include "include/memory_acquire_release/memory_acquire_release_1.h"

int main() {
    order_relaxed::run_code();
    order_seq_cst::run_code();
    order_acquire_release::run_code();

    return 0;
}

