#include "vocabulary.h"


[[nodiscard]] errorCode bye(void) {
    exit(0);
}

/*
[[nodiscard]] errorCode fetch(void) {
    if (!stack_check_1()) return;
    Stack[SP-1] = *(long long int *)Stack[SP-1];
}
[[nodiscard]] errorCode store(void) {
    if (!stack_check_2()) return;
    *(long long int *)Stack[SP-1] = Stack[SP-2];
    SP -= 2;
}
*/

[[nodiscard]] errorCode if(void) {
    if (SP < 1) return StackUnderflow;
    cell cond = Stack[--SP];
    if (cond) {
    }
}
