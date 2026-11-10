#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum errorCode: int8_t {
    OK = 0,
    // WordNotFound,
    StackOverflow,
    StackUnderflow,
    ReturnStackOverflow,
    ReturnStackUnderflow,
    Overflow,
    Underflow,
    DivisionByZero,
    DivisionOverflow
} errorCode;

typedef errorCode function(void);

void print_error(errorCode error);
