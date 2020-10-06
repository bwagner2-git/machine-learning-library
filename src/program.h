#ifndef PROGRAM_H
#define PROGRAM_H

#include "expression.h"
#include <vector> //////////added in

class evaluation;    //// does this just import the class

class program
{
    std::vector<expression> exprs_;   ////added in
public:
    program();

    void append_expression(
        int expr_id,
        const char *op_name,
        const char *op_type,
        int inputs[],
        int num_inputs);

    // return 0 for success
    int add_op_param_double(
        const char *key,
        double value);

    // return 0 for success
    int add_op_param_ndarray(
        const char *key,
        int dim,
        size_t shape[],
        double data[]);

    evaluation *build();      //// what is the * for this means derefernece right? or not in this case
}; // class program

#endif // PROGRAM_H
