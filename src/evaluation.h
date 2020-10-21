#ifndef EVALUATION_H
#define EVALUATION_H

#include "expression.h"
#include "tensor.h"

class evaluation
{
public: 
    evaluation(const std::vector<expression> &exprs);  ///what does using & in construction mean?  why would i doit that way 
         //////// do i put a map in here too?
    std::map<int, tensor> terms_;
    std::vector<expression> exprs_;
    std::map<std::string, tensor> kwargs_;
    void add_kwargs_double(
        const char *key,     //// x=5 char is x 
        double value); //// value =5

    void add_kwargs_ndarray(
        const char *key,
        int dim,
        size_t shape[],
        double data[]);

    // return 0 for success
    int execute();

    // return the variable computed by the last expression
    tensor &get_result();


private:
    tensor result_;    /// we should be returning a tensor now right?
}; // class evaluation

#endif // EVALUATION_H
