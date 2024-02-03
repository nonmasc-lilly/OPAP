# OPAP

opap is a pixel mapping engine made with C and OPENGL

To compile with opap one must also have glfw3, glad is included
locally.

### OPAP USAGE

a general opap program:

- creates an OPAP
- loops while the OPAP is not closed
- + starts handling the OPAP
- + + works on the opap data
- + + sets need updating correctly
- + ends handling the OPAP
- terminates

an example (which draws simply a black screen is as follows:

```C
#include "opap.h"

#define WIDTH        100
#define HEIGHT       100
#define RESOLUTION_X 100
#define RESOLUTION_Y 100

int main(int argc, char **argv) {
    OPAP_T opap;

    opap = create_opap(WIDTH, HEIGHT, RESOLUTION_X, RESOLUTION_Y, "OPAP_TEST");
    while(!opap.is_closed) {
        start_handle_opap(&opap);
        end_handle_opap(&opap);
    }
}
```

see the documentation for more details
