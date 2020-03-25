#include <stdio.h>
//#include "static_lib.h"
#include "dynamic_lib.h"

int main() {

    char filename[] = "/home/talkytitan/CLionProjects/dz2/testfile.txt";

    struct data* result = process(filename);

    print_result(result);

    clear(result);

    return 0;
}
