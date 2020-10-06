#ifndef EVALUATION_H
#define EVALUATION_H

#include "expression.h"

class evaluation
{
public: 
    evaluation(const std::vector<expression> &exprs);  ///what does using & in construction mean?  why would i doit that way 
         //////// do i put a map in here too?
    std::map<int, double> terms_;
    std::vector<expression> exprs_;
    std::map<std::string, double> kwargs_;
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
    double &get_result();

private:
    double result_;
}; // class evaluation

#endif // EVALUATION_H
