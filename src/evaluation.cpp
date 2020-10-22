#include <assert.h>
#include "evaluation.h"

evaluation::evaluation(const std::vector<expression> &exprs)/// why is this the only constructor with things in these parentheses /// this is the consturctor like __init__ in python 
    : result_(0) ///what does this do here
{ /// make a copy and work with the copy thats why you use const in constructor above along with reference ... still allitle unsure about what exactly a reference does?
    exprs_= exprs;  /// look into difference between reference and a pointer initalize this variable with the reference &exprs 
    /// do the other things just default to empty if I do not set them to anything
}

void evaluation::add_kwargs_double(
    const char *key, //// shoulnt this be an int ???  isnt this going to be the expr id /// need something extra to store it in lecture 8 slide 28
        /// store the key wod agrguments and look
    double value)
{
    kwargs_[key]=value; //// added in
}

void evaluation::add_kwargs_ndarray(
    const char *key,
    int dim,
    size_t shape[],
    double data[])
{
    kwargs_[key]=tensor(dim, shape, data);
}

int evaluation::execute()
{
    terms_.clear();
    for(auto &expr: exprs_) {  /// for in loop!
        if (expr.get_op_type()=="Input"){
            terms_[expr.get_expr_id()]=kwargs_[expr.get_op_name()]; // make sure only tensors are stored in terms
            result_=kwargs_[expr.get_op_name()];///is this right?
        }
        else if (expr.get_op_type()=="Add"){
            int dim = terms_[expr.get_inputs()[0]].get_dim();
            if (dim==0){
                double total = 0;
                for(int i =0; i<expr.get_num_inputs(); i++){
                    assert(terms_[expr.get_inputs()[i]].get_dim()==0);
                    total += terms_[expr.get_inputs()[i]].item();    //////////// this should be right right? i should start at 0 because its iterating through expressions list of inputs?
                }
                terms_[expr.get_expr_id()]=tensor(total);
                result_=tensor(total);
            } else {
                double dat[terms_[expr.get_inputs()[0]].get_data_vector().size()];
                // std::vector<double> dat;
                for (size_t i = 0; i<terms_[expr.get_inputs()[0]].get_data_vector().size(); ++i){
                    double a = terms_[expr.get_inputs()[0]].get_data_vector()[i];
                    double b = terms_[expr.get_inputs()[1]].get_data_vector()[i];
                    // dat.push_back(a+b);
                    dat[i]=a+b;
                } 
                result_=tensor(dim, terms_[expr.get_inputs()[0]].get_shape_array(), dat);       
            }
           
        } else if (expr.get_op_type()=="Const"){
            terms_[expr.get_expr_id()]= expr.get_op_params()["value"];   // the value of the constant is always under key "value" in op_params_ the value is always set right after adding the constant expressions
            //// what happens if i have more than one constant and the value gets overwritten
            result_= expr.get_op_params()["value"];
        } else if (expr.get_op_type()=="Sub") {
            int dim = terms_[expr.get_inputs()[0]].get_dim();
            if (dim==0){
                double total = 0;
                total = terms_[expr.get_inputs()[0]].item()-terms_[expr.get_inputs()[1]].item();
                terms_[expr.get_expr_id()]=total;
                result_=tensor(total);
            } else {
                double dat[terms_[expr.get_inputs()[0]].get_data_vector().size()];
                // std::vector<double> dat;
                for (size_t i = 0; i<terms_[expr.get_inputs()[0]].get_data_vector().size(); ++i){
                    double a = terms_[expr.get_inputs()[0]].get_data_vector()[i];
                    double b = terms_[expr.get_inputs()[1]].get_data_vector()[i];
                    // dat.push_back(a+b);
                    dat[i]=a-b;
                } 
                result_=tensor(dim, terms_[expr.get_inputs()[0]].get_shape_array(), dat);       
            }
        } else if (expr.get_op_type()=="Mul"){
            double total = 0;
            double first = terms_[expr.get_inputs()[0]].item();
            double second =terms_[expr.get_inputs()[1]].item();
            total = second * first;                 
            terms_[expr.get_expr_id()]=total; 
            result_=total;
        }


        }
        return 0; ///// is this where I return 0 for no errors, what other integer should I return shoulnt integer be returned in get_result below?
        /// why do in some of the function i return a 0 and in the others i dont care ???
}

tensor &evaluation::get_result()
{
    return result_; // is the result always the last espression evaluated?
}
