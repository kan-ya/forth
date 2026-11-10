#pragma once

#include "error.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct head head;
typedef enum wordType {Primitive, Colon} wordType;
typedef struct body body;
typedef union code code;

extern head *Dictionary;


union code {
    function *primitive;
    body **colon;
};

struct body {
    bool isImmediate;
    wordType flag;
    unsigned int codeCount;
    code code;
    unsigned int parameterCount;
    long long int *parameter;
};

struct head {
    const char *name;
    head *next;
    body *body;
};


head *add_primitive_word(
        const char *name,
        function *code);
