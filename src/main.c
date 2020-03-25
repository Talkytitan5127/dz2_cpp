#include <interface.h>

int main() {

    char filename[] = "/home/talkytitan/CLionProjects/dz2/testfile.txt";

    struct data* result = process(filename);

    print_result(result);

    clear(result);

    return 0;
}
