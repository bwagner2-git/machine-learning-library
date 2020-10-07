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
} //////////should I push to the front so that i can iterate through it in forward direction

int program::add_op_param_double(   ////////////////////////WORKS WITH LAST EXPRESSION APPENDED 
    const char *key, /// this is called "value"    
    double value)
{
    exprs_.back().add_op_param_double(key, value);
    op_params_[key] = value; ///// Could I create a new expression of constant type and append this expression to the std vector of expressions
    // return -1; /// why is this returning -1 ??? 
    return 0;
}

int program::add_op_param_ndarray(
    const char *key,
    int dim,
    size_t shape[],
    double data[])
{
    return -1;
}

evaluation *program::build() ///// what are we doing here ????
{
    evaluation *eval = new evaluation(exprs_);
    return eval; ///// this is failing during make
    // return nullptr; // this was the original line here
}
