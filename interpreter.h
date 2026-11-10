#pragma once

#include "util.h"

#include "dictionary.h"
#include "stack.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>


extern body *DummyPushBody;


typedef struct slice {
    const char *addr;
    unsigned int len;
} slice;

typedef enum interpreterState: int8_t {
    Interpret,
    Compile
} interpreterState;

typedef struct optionCell {
    bool exist;
    cell val;
} optionCell;


head *find_word(slice name);

errorCode execute(body *word);
errorCode execute_primitive(body *word);
errorCode execute_colon(body *word);
optionCell process_as_number(slice sli);

void interpret(const char *input);

function colon;
function semicolon;
function immediate;
function push_parameter;
