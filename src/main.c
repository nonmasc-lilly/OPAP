#include "opap.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int i, j;
    OPAP_T opap;
    opap = create_opap(800, 800, 100, 100, "OPAP TEST");
    for(i=0; i < 100; i+=1) {
        for(j=0; j < 8; j++) {
            set_opap_coord(&opap, i, j, 0xFFFFFFFF);
        } 
    }
    opap.needs_updating = OTRUE;
    while(!opap.is_closed) {
        start_handle_opap(&opap);
        end_handle_opap(&opap);
    }
}
