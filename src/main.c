#include "opap.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int i, j;
    OPAP_T opap;
    opap = create_opap(800, 800, 100, 100, "OPAP TEST");
    while(!opap.is_closed) {
        start_handle_opap(&opap);
        end_handle_opap(&opap);
    }
}
