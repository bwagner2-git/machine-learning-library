#include "expression.h"
#include "tensor.h"

expression::expression(
    int expr_id,
    const char *op_name,
    const char *op_type,
    int *inputs,
    int num_inputs):
    expr_id_(expr_id), op_name_(op_name), op_type_(op_type), inputs_(inputs, inputs+num_inputs)
{
}

void expression::add_op_param_double(
    const char *key,
    double value)
{
    op_params_[key]= value;  /// should this be a tensor instead of a double? I would just use the ctor with the double v inside it right?
}

void expression::add_op_param_ndarray(
    const char *key,
    int dim,
    size_t shape[],
    double data[])
{
    std::string k = key;
   // std::vector<size_t> = shape[]; // is this what you mean when you say make copies
                                        //// cant I just make a tensor with these values and store it with the key? Do I have to make copies before I do that?
    op_params_[key]= tensor(dim, shape, data);
}


std::string expression::get_op_type(){
    return op_type_;
}

std::string expression::get_op_name(){
    return op_name_;
}
int expression::get_expr_id(){
    return expr_id_;
}

int expression::get_num_inputs(){
    return inputs_.size();
}

std::vector<int> expression::get_inputs(){
    return inputs_;
}


std::map<std::string, double> expression::get_op_params() {
    return op_params_;
}
