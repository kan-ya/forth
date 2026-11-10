#include "stack.h"
#include "dictionary.h"
#include "interpreter.h"
#include "vocabulary.h"

#include <stdint.h>


int main(void) {
    add_primitive_word("dummy_push", &dummy_push);
    DummyPushBody = Dictionary->body;
    add_primitive_word(":", &colon);
    // ; must be immediate word!
    add_primitive_word(";", &semicolon);
    immediate();
    add_primitive_word("immediate", &immediate);

    add_primitive_word(".", &print);
    add_primitive_word("emit", &emit);
    add_primitive_word(".s", &print_stack);

    add_primitive_word("pop", &pop);
    add_primitive_word("dup", &dup);
    add_primitive_word("swap", &swap);
    add_primitive_word("nip", &nip);
    add_primitive_word("rot", &rot);
    add_primitive_word("pick", &pick);
    add_primitive_word("roll", &roll);

    add_primitive_word("+", &add);
    add_primitive_word("-", &sub);
    add_primitive_word("*", &mul);
    add_primitive_word("/", &divi);

    add_primitive_word("not", &not);

    add_primitive_word("invert", &invert);
    add_primitive_word("and", &and);
    add_primitive_word("or", &or);
    add_primitive_word("xor", &xor);

    add_primitive_word("=", &eq);
    add_primitive_word("!=", &neq);
    add_primitive_word("<", &lt);
    add_primitive_word("<=", &lte);
    add_primitive_word(">", &gt);
    add_primitive_word(">=", &gte);

    add_primitive_word("R>", &fromR);
    add_primitive_word(">R", &toR);
    add_primitive_word("depth", &depth);
    add_primitive_word("Rdepth", &Rdepth);


    add_primitive_word("bye", &bye);

    /*
    add_primitive_word("@", &fetch);
    add_primitive_word("!", &store);
    */


    char buffer[1024] = {0};
    while (fgets(buffer, 1024, stdin)) {
        interpret(buffer);
    }
    return 0;
}
