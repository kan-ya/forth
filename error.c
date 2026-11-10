#include "error.h"

void print_error(errorCode error) {
    const char *message;
    switch (error) {
        case OK: return;
        case StackOverflow:
            message = "stack overflow";
            break;
        case StackUnderflow:
            message = "stack underflow";
            break;
        case ReturnStackOverflow:
            message = "return stack overflow";
            break;
        case ReturnStackUnderflow:
            message = "return stack underflow";
            break;
        case Overflow:
            message = "overflow";
            break;
        case Underflow:
            message = "underflow";
            break;
        case DivisionByZero:
            message = "division by zero";
            break;
        case DivisionOverflow:
            message = "division overflow";
            break;
    }
    printf("\n[Error]: %s\n", message);
}
