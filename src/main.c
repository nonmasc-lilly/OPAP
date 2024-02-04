#include "opap.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int i, j;
    ASCII in;
    UCODE uin;
    OPAP_T opap;
    opap = create_opap(800, 800, 100, 100, "OPAP TEST");
    while(!opap.is_closed) {
        start_handle_opap(&opap);
        in = opap_consume_key(&opap, &uin);
        if(in != 0 || uin != 0) {
            printf("%c", in);
            fflush(stdout);
        }
        end_handle_opap(&opap);
    }
}
