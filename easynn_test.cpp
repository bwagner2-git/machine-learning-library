/**
 * A simple test program helps you to debug your easynn implementation.
 */

#include <stdio.h>
#include "src/libeasynn.h"

int main()
{
    program *prog = create_program();

    int inputs0[] = {};
    append_expression(prog, 0, "a", "Input", inputs0, 0);

    int inputs1[] = {0, 0}; /// i believe that these are the expr_ids of the expressions to be added!!!
    append_expression(prog, 1, "", "Add", inputs1, 2);

    evaluation *eval = build(prog);    /// eval is a pointer
    add_kwargs_double(eval, "a", 5);   // telling it what a equals go throgh and look for a and then you know thats a 5

    int dim = 0;
    size_t *shape = nullptr;
    double *data = nullptr;
    if (execute(eval, &dim, &shape, &data) != 0)
    {
        printf("evaluation fails\n");
        return -1;
    }

    if (dim == 0)
        printf("res = %f\n", data[0]);
    else
        printf("result as tensor is not supported yet\n");

    return 0;
}
