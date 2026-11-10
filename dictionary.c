#include "dictionary.h"


head *Dictionary = NULL;


head *add_primitive_word(
        const char *name,
        function *code) {
    body *newBody = calloc(1, sizeof(body));
    head *newHead = calloc(1, sizeof(head));
    if (!newBody || !newHead) {
        printf("failed: calloc()\n");
        exit(1);
    }

    newHead->name = name;
    newHead->next = Dictionary;
    newHead->body = newBody;
    newBody->flag = Primitive;
    newBody->codeCount = 1;
    newBody->code.primitive = code;

    Dictionary = newHead;
    return newHead;
}
