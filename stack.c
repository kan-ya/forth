#include "stack.h"


uint16_t SP = 0, RP = 0;
cell  Stack[STACK_SIZE] = {0};
cell RStack[STACK_SIZE] = {0};


[[nodiscard]] errorCode dummy_push(void) {
    printf("\x1B[1m\x1B[5m\x1B[33m\x1B[45m[Error]\x1B[0m \x1B[1m\x1B[3mこの関数はダミーであり、実行されるべきではない。\x1B[0m \x1B[1m\x1B[5m\x1B[33m\x1B[45m[Error]\x1B[0m \n");
    return OK;
}

[[nodiscard]] errorCode print_stack(void) {
    if (SP >= STACK_SIZE) return StackOverflow;
    for (unsigned int i = 0; i < SP; i++) {
        printf("%lld", Stack[i]);
        if (i+1 < SP) printf(", ");
    }
    putchar('\n');
    return OK;
}


[[nodiscard]] errorCode return_stack_check_0_1(void) {
    if (RP >= STACK_SIZE) {
        return ReturnStackOverflow;
    }
    return OK;
}
[[nodiscard]] errorCode return_stack_check_1(void) {
    if (RP < 1) {
        return ReturnStackUnderflow;
    }
    return OK;
}


// print
[[nodiscard]] errorCode print(void) {
    if (SP < 1) return StackUnderflow;
    printf("%lld ", Stack[--SP]);
    return OK;
}
[[nodiscard]] errorCode emit(void) {
    if (SP < 1) return StackUnderflow;
    printf("%c", (char)Stack[--SP]);
    return OK;
}

// stack operater
[[nodiscard]] errorCode push(cell val) {
    if (SP >= STACK_SIZE) return StackOverflow;
    Stack[SP++] = val;
    return OK;
}
[[nodiscard]] errorCode pop(void) {
    if (SP < 1) return StackUnderflow;
    --SP;
    return OK;
}
[[nodiscard]] errorCode dup(void) {
    if (SP >= STACK_SIZE) return StackOverflow;
    Stack[SP] = Stack[SP-1];
    SP++;
    return OK;
}
[[nodiscard]] errorCode swap(void) {
    if (SP < 2) return StackUnderflow;
    cell temp = Stack[SP-1];
    Stack[SP-1] = Stack[SP-2];
    Stack[SP-2] = temp;
    return OK;
}

[[nodiscard]] errorCode nip(void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = Stack[SP-1];
    --SP;
    return OK;
}
[[nodiscard]] errorCode rot(void) {
    if (SP < 3) return StackUnderflow;
    cell temp = Stack[SP-3];
    Stack[SP-3] = Stack[SP-2];
    Stack[SP-2] = Stack[SP-1];
    Stack[SP-1] = temp;
    return OK;
}

[[nodiscard]] errorCode pick(void) {
    if (SP < 1 || SP <= Stack[SP-1]) {
        return StackUnderflow;
    }
    Stack[SP-1] = Stack[SP-2-Stack[SP-1]];
    return OK;
}
[[nodiscard]] errorCode roll(void) {
    if (SP < 1) return StackUnderflow;
    unsigned int depth = Stack[--SP];
    if (SP <= depth) return StackUnderflow;
    cell temp = Stack[SP-1-depth];
    for (unsigned int i = 0; i < depth; i++) {
        Stack[SP-1-depth+i] = Stack[SP-depth+i];
    }
    Stack[SP-1] = temp;
    return OK;
}


// arithmetic
[[nodiscard]] errorCode add(void) {
    if (SP < 2) return StackUnderflow;
    cell result = 0;
    if (ckd_add(&result, Stack[SP-2], Stack[SP-1]))
        return Overflow;
    Stack[SP-2] = result;
    --SP;
    return OK;
}
[[nodiscard]] errorCode sub(void) {
    if (SP < 2) return StackUnderflow;
    cell result = 0;
    if (ckd_sub(&result, Stack[SP-2], Stack[SP-1]))
        return Overflow;
    Stack[SP-2] = result;
    --SP;
    return OK;
}
[[nodiscard]] errorCode mul(void) {
    if (SP < 2) return StackUnderflow;
    cell result = 0;
    if (ckd_mul(&result, Stack[SP-2], Stack[SP-1]))
        return Overflow;
    Stack[SP-2] = result;
    --SP;
    return OK;
}
[[nodiscard]] errorCode divi(void) {
    if (SP < 2) return StackUnderflow;
    cell a = Stack[SP-2];
    cell b = Stack[SP-1];
    if (b == 0)
        return DivisionByZero;
    if (a == LLONG_MIN && b == -1)
        return DivisionOverflow;
    Stack[SP-2] = a / b;
    --SP;
    return OK;
}


// logical
[[nodiscard]] errorCode not(void) {
    if (SP < 1) return StackUnderflow;
    Stack[SP-1] = (Stack[SP-1])? 0: -1;
    return OK;
}


// compare
[[nodiscard]] errorCode eq (void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = ((Stack[SP-2] == Stack[SP-1]))? -1: 0;
    --SP;
    return OK;
}
[[nodiscard]] errorCode neq(void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = ((Stack[SP-2] != Stack[SP-1]))? -1: 0;
    --SP;
    return OK;
}
[[nodiscard]] errorCode lt (void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = ((Stack[SP-2] < Stack[SP-1]))? -1: 0;
    --SP;
    return OK;
}
[[nodiscard]] errorCode lte(void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = ((Stack[SP-2] <= Stack[SP-1]))? -1: 0;
    --SP;
    return OK;
}
[[nodiscard]] errorCode gt (void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = (Stack[SP-2] > Stack[SP-1])? -1: 0;
    --SP;
    return OK;
}
[[nodiscard]] errorCode gte(void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] = ((Stack[SP-2] >= Stack[SP-1]))? -1: 0;
    --SP;
    return OK;
}


// bit
[[nodiscard]] errorCode invert(void) {
    if (SP < 1) return StackUnderflow;
    Stack[SP-1] = ~Stack[SP-1];
    return OK;
}
[[nodiscard]] errorCode and(void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] &= Stack[SP-1];
    --SP;
    return OK;
}
[[nodiscard]] errorCode or (void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] |= Stack[SP-1];
    --SP;
    return OK;
}
[[nodiscard]] errorCode xor(void) {
    if (SP < 2) return StackUnderflow;
    Stack[SP-2] ^= Stack[SP-1];
    --SP;
    return OK;
}


// return stack
[[nodiscard]] errorCode fromR(void) {
    if (SP >= STACK_SIZE) return StackOverflow;
    if (RP < 1) return ReturnStackUnderflow;
    Stack[SP++] = RStack[--RP];
    return OK;
}
[[nodiscard]] errorCode toR(void) {
    if (SP < 1) return StackUnderflow;
    if (RP >= STACK_SIZE) return ReturnStackOverflow;
    RStack[RP++] = Stack[--SP];
    return OK;
}

[[nodiscard]] errorCode Rpop(void) {
    if (RP < 1) return ReturnStackUnderflow;
    --RP;
    return OK;
}
[[nodiscard]] errorCode Rdup(void) {
    if (RP >= STACK_SIZE) return StackOverflow;
    Rstack[RP] = Rstack[RP+1];
    RP++;
    return OK;
}
[[nodiscard]] errorCode Rswap(void) {
    if (RP < 2) return RStackUnderflow;
    cell temp = RStack[RP-1];
    RStack[RP-1] = RStack[RP-2];
    RStack[RP-2] = temp;
    return OK;
}


[[nodiscard]] errorCode depth(void) {
    if (SP >= STACK_SIZE) return StackOverflow;
    Stack[SP] = SP;
    SP++;
    return OK;
}
[[nodiscard]] errorCode Rdepth(void) {
    if (SP >= STACK_SIZE) return StackOverflow;
    Stack[SP++] = RP;
    return OK;
}
