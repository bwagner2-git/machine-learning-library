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
                terms_[expr.get_expr_id()]= tensor(dim, terms_[expr.get_inputs()[0]].get_shape_array(), dat); 
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
                terms_[expr.get_expr_id()]=tensor(total);
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
                terms_[expr.get_expr_id()]=tensor(dim, terms_[expr.get_inputs()[0]].get_shape_array(), dat);
                result_=tensor(dim, terms_[expr.get_inputs()[0]].get_shape_array(), dat);       
            }
        } else if (expr.get_op_type()=="Mul"){
            if ((terms_[expr.get_inputs()[0]].get_dim()==0) && (terms_[expr.get_inputs()[1]].get_dim()==0)){
                terms_[expr.get_expr_id()]= tensor(terms_[expr.get_inputs()[0]].item()*terms_[expr.get_inputs()[1]].item());
                result_=tensor(terms_[expr.get_inputs()[0]].item()*terms_[expr.get_inputs()[1]].item());
            } else if ((terms_[expr.get_inputs()[0]].get_dim()==0) && (terms_[expr.get_inputs()[1]].get_dim()!=0)) {
                double dat[terms_[expr.get_inputs()[1]].get_data_vector().size()];
                for (size_t i = 0; i<terms_[expr.get_inputs()[1]].get_data_vector().size(); ++i){
                    double b = terms_[expr.get_inputs()[1]].get_data_vector()[i];
                    dat[i]=b*terms_[expr.get_inputs()[0]].item();
                }
                terms_[expr.get_expr_id()]=tensor(terms_[expr.get_inputs()[1]].get_dim(),terms_[expr.get_inputs()[1]].get_shape_array(),dat); 
                result_=tensor(terms_[expr.get_inputs()[1]].get_dim(),terms_[expr.get_inputs()[1]].get_shape_array(),dat);  ////////////////make sure all the ones and zeros line up with what they should be on this one and below
            } else if ((terms_[expr.get_inputs()[0]].get_dim()!=0) && (terms_[expr.get_inputs()[1]].get_dim()==0)) {
                double dat[terms_[expr.get_inputs()[0]].get_data_vector().size()];
                for (size_t i = 0; i<terms_[expr.get_inputs()[0]].get_data_vector().size(); ++i){
                    double b = terms_[expr.get_inputs()[0]].get_data_vector()[i];
                    dat[i]=b*terms_[expr.get_inputs()[1]].item();
                }
                terms_[expr.get_expr_id()]=tensor(terms_[expr.get_inputs()[0]].get_dim(),terms_[expr.get_inputs()[0]].get_shape_array(),dat); 
                result_=tensor(terms_[expr.get_inputs()[0]].get_dim(),terms_[expr.get_inputs()[0]].get_shape_array(),dat);
            } else {
                size_t row = terms_[expr.get_inputs()[0]].get_shape_array()[0];
                size_t col = terms_[expr.get_inputs()[1]].get_shape_array()[1];
                size_t x = terms_[expr.get_inputs()[0]].get_shape_array()[1];  /// need to make array of size row * col to pass to tensor construtor later on
                double dat[row*col];
                tensor A = terms_[expr.get_inputs()[0]];
                tensor B = terms_[expr.get_inputs()[1]];
                for (size_t i = 0; i<row; ++i){
                    for (size_t j = 0; j<col; ++j){
                        double total = 0;
                        for (size_t p = 0; p<x; ++p){
                            total+= A.at(i,p)*B.at(p,j);                        /// A[i][x]*B[x][j]
                        }
                        dat[i*col+j] = total;
                    }
                }
                // dim = 2
                size_t shape[2]= {row,col};

                terms_[expr.get_expr_id()]=tensor(2, shape, dat);
                result_= tensor(2, shape, dat);
            }


        } else if (expr.get_op_type()=="ReLU") {
            std::vector<double> a=terms_[expr.get_inputs()[0]].get_data_vector();
            double dat[a.size()];
            for (size_t i =0; i<a.size(); i++){
                if (a[i]<0){
                    dat[i]=0;
                }else {
                    dat[i]=terms_[expr.get_inputs()[0]].get_data_vector()[i];
                }
            }
            result_ = tensor(terms_[expr.get_inputs()[0]].get_dim(),terms_[expr.get_inputs()[0]].get_shape_array(),dat);

        } else if (expr.get_op_type()=="Flatten") {
            // works
            // size_t next = terms_[expr.get_inputs()[0]].get_dim();
            // size_t shape[]={1, next};
            // result_ = tensor(1,shape,terms_[expr.get_inputs()[0]].get_data_array());
            // works
            size_t first = terms_[expr.get_inputs()[0]].get_shape_array()[0];
            size_t second = terms_[expr.get_inputs()[0]].get_shape_array()[1]*terms_[expr.get_inputs()[0]].get_shape_array()[2]*terms_[expr.get_inputs()[0]].get_shape_array()[3];
            size_t sha[]={first,second};
            result_ = tensor(2,sha,terms_[expr.get_inputs()[0]].get_data_array());

        }

         ///// is this where I return 0 for no errors, what other integer should I return shoulnt integer be returned in get_result below?
        /// why do in some of the function i return a 0 and in the others i dont care ???

    }
    return 0;
}

tensor &evaluation::get_result()
{
    return result_; // is the result always the last espression evaluated?
}
