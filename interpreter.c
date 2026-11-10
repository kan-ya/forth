#include "interpreter.h"



interpreterState State;
head *CurrentDefiningWord = NULL;
bool HasReceivedWordName = false;

constexpr unsigned int CodeCountMax = 1024;
size_t CodeCount = 0;
body *CodeBuffer[CodeCountMax] = {};

constexpr unsigned int ParameterCountMax = 1024;
size_t ParameterCount = 0;
cell ParameterBuffer[ParameterCountMax] = {};

body *DummyPushBody = nullptr;


bool equal(slice sli, const char *str) {
    if (!sli.addr || sli.len <= 0 || !str) return false;
    for (unsigned int i = 0; i < sli.len; i++) {
        if (sli.addr[i] != str[i]) return false;
    }
    return (str[sli.len] == '\0');
}

head *find_word(slice name) {
    head *dict = Dictionary;
    while (dict) {
        if (equal(name, dict->name)) return dict;
        dict = dict->next;
    }
    return NULL;
}

errorCode execute_primitive(body *word) {
    if (!word) return OK;
    return word->code.primitive();
}
errorCode execute_colon(body *word) {
    if (!word) return OK;
    errorCode status;
    [[maybe_unused]] unsigned int parameterIndex = 0;
    for (unsigned int i = 0; i < word->codeCount; i++) {
        auto colon = word->code.colon[i];
        if (colon == nullptr) {
            printf("ワードの定義にヌルポインタを含めることはできない。\n");
            exit(1);
        }
        if (colon->flag == Primitive && colon == DummyPushBody) {
            if (parameterIndex >= word->parameterCount) {
                printf("nonexistent parameter\n");
                exit(1);
            }
            if ((status = push(word->parameter[parameterIndex++])) != OK)
                return status;
            continue;
        }
        if ((status = execute(word->code.colon[i])) != OK)
            return status;
    }
    return OK;
}
errorCode execute(body *word) {
    if (!word) return OK;
    switch (word->flag) {
        case Primitive:
            return execute_primitive(word);
        case Colon:
            return execute_colon(word);
    }
    return OK;
}


optionCell process_as_number(slice sli) {
    optionCell ret = {.exist = false, .val = 0};
    if (!sli.addr || sli.len <= 0) return ret;
    bool neg = false;
    if (sli.addr[0] == '-' || sli.addr[0] == '+') {
        neg = (sli.addr[0] == '-');
        sli.addr++;
        sli.len--;
    }
    for (unsigned int i = 0; i < sli.len; i++) {
            if (!isdigit((unsigned char)sli.addr[i])) return ret;
        if (ckd_mul(&ret.val, ret.val, 10)) {
            printf("overflow\n");
            return ret;
        }
        if (ckd_add(&ret.val, ret.val, sli.addr[i] - '0')) {
            printf("overflow\n");
            return ret;
        }
    }
    if (neg) ret.val = ~ret.val + 1;
    ret.exist = true;
    return ret;
}

slice next_token(const char **strp) {
    slice token = {NULL, 0};
    if (!strp || !*strp) return token;
    token.addr = *strp;
    while (isspace((unsigned char)**strp) && **strp != '\0') (*strp)++;
    token.addr = *strp;
    while (!isspace((unsigned char)**strp) && **strp != '\0') {
        token.len++;
        (*strp)++;
    }
    return token;
}
void word_not_found(slice sli) {
    for (unsigned int i = 0; i < sli.len; i++)
        putchar(sli.addr[i]);
    printf("?\n");
}
void interpret(const char *input) {
    if (!input) return;
    slice token;
    head *word;
    optionCell number;
    errorCode status = OK;

    while (*input != '\0') {
        token = next_token(&input);
        if (token.len <= 0) break;

        switch (State) {
            case Interpret:
                if ((word = find_word(token))) {
                    if ((status = execute(word->body)) != OK) {
                        print_error(status);
                        return;
                    }
                    continue;
                }
                if ((number = process_as_number(token)).exist) {
                    if ((status = push(number.val)) != OK) {
                        print_error(status);
                        return;
                    }
                    continue;
                }
                word_not_found(token);
                return;
                break;
            case Compile:
                if (HasReceivedWordName) {
                    if ((word = find_word(token))) {
                        if (word->body->isImmediate) {
                            if((status = execute(word->body)) != OK) {
                                print_error(status);
                                return;
                            }
                            continue;
                        }
                        if (CodeCount >= CodeCountMax) {
                            printf("too long word difinition\n");
                            return;
                        }
                        CodeBuffer[CodeCount++] = word->body;
                        continue;
                    }
                    if ((number = process_as_number(token)).exist) {
                        if (ParameterCount >= ParameterCountMax) {
                            printf("too many word parameter\n");
                            return;
                        }
                        if (CodeCount >= CodeCountMax) {
                            printf("too long word difinition\n");
                            return;
                        }
                        CodeBuffer[CodeCount++] = DummyPushBody;
                        ParameterBuffer[ParameterCount++] = number.val;
                        continue;
                    }
                    word_not_found(token);
                    return;
                } else {
                    char *name = new(char[token.len+1]);
                    for (unsigned int i = 0; i < token.len; i++)
                        name[i] = token.addr[i];
                    name[token.len] = '\0';
                    CurrentDefiningWord->name = name;
                    if ((word = find_word(token))) {
                        for (unsigned int i = 0; i < token.len; i++)
                            putchar(token.addr[i]);
                        printf(" is not unique ");
                    }
                    HasReceivedWordName = true;
                    continue;
                }
                //break;
            default:
                printf("unknown intrepreter state\n");
                return;
        }
    }
    printf(" ok\n");
}


errorCode colon(void) {
    State = Compile;
    CurrentDefiningWord = new(head);
    CurrentDefiningWord->body = new(body);
    CodeCount = 0;
    ParameterCount = 0;
    HasReceivedWordName = false;
    return OK;
}
errorCode semicolon(void) {
    State = Interpret;

    auto CDW = CurrentDefiningWord;

    CDW->body->codeCount = CodeCount;
    CDW->body->code.colon = new(body*[CodeCount]);
    memcpy(CDW->body->code.colon, CodeBuffer, CodeCount * sizeof(body *));

    CDW->body->parameterCount = ParameterCount;
    CDW->body->parameter = new(cell[ParameterCount]);
    memcpy(CDW->body->parameter, ParameterBuffer, ParameterCount * sizeof(cell));

    CDW->body->isImmediate = false;
    CDW->body->flag = Colon;
    CDW->next = Dictionary;

    Dictionary = CDW;
    return OK;
}
errorCode immediate(void) {
    Dictionary->body->isImmediate = true;
    return OK;
}
