#include "program.h"
#include "evaluation.h"

program::program()
{                               /////// exprs_ will just default to an empty vector if we do not do anything here right, which is what we want?
}

void program::append_expression(
    int expr_id,
    const char *op_name,
    const char *op_type,
    int inputs[],
    int num_inputs)
{
    exprs_.push_back(expression(expr_id, op_name, op_type, inputs, num_inputs));   //// we can just access exprs_ becuase it is a member function of program?   
}

int program::add_op_param_double(
    const char *key,
    double value)
{
    return -1;
}

int program::add_op_param_ndarray(
    const char *key,
    int dim,
    size_t shape[],
    double data[])
{
    return -1;
}

evaluation *program::build()
{
    return nullptr;
}
