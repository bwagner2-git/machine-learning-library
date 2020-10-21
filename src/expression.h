#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <string>
#include <map>
#include "tensor.h"

class evaluation;

class expression
{
    friend class evaluation;
    //////////////////////
    int expr_id_; /////////////added in
    std::string op_name_;
    std::string op_type_;
    std::vector<int> inputs_; /// these are other expression ids so they are ints
    std::map<std::string, tensor> op_params_;   // make this hold tensors?
    /////////////////////////////
public:
    expression(
        int expr_id,
        const char *op_name,
        const char *op_type,
        int *inputs,
        int num_inputs);

    void add_op_param_double(
        const char *key,
        double value);

    void add_op_param_ndarray(
        const char *key,
        int dim,
        size_t shape[],
        double data[]);

    std::string get_op_type();
    std::string get_op_name();
    int get_expr_id();
    int get_num_inputs();
    std::vector<int> get_inputs();
    std::map<std::string, tensor> get_op_params();

}; // class expression

#endif // EXPRESSION_H
