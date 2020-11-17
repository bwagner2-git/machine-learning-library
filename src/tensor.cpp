#include "tensor.h"
#include <assert.h>


tensor::tensor():
    data_(1,0){}

tensor::tensor(double v): data_(1,v){

}

tensor::tensor(int dim, size_t shape[], double data[]): shape_(shape, shape+dim) {   //// what does the shape_(shape, shape+dim) do again 
    size_t n = shape[0];   ///  this is just saying a built in array type of double is passed in right?
    for (int i=1; i<dim; ++i) {
        n = n*shape[i];
    }                 ///why do we use promitive types in construction again

    data_.assign(data, data+n); /// how does this work
}  

double tensor::item() const{
    assert(shape_.empty());
    return data_[0];
}

double &tensor::item(){
    assert(shape_.empty());
    return data_[0];
}


int tensor::get_dim() const{ /// might need to change this to size_t instead of int
    return shape_.size();
}


double tensor::at(size_t i) const { // access of 1d tensors
    assert(shape_.size()==1);
    assert(i<shape_[0]);
    return data_[i];
}

double tensor::at(size_t i, size_t j) const { //  access for 2d tensors 
    assert(shape_.size()==2);
    assert((i<shape_[0]) && (j<shape_[1]));
    return data_[i*shape_[1]+j];
}

double tensor::at(size_t n, size_t h, size_t w, size_t c) const { //  access for 4d tensors 
    assert(shape_.size()==4);
    assert((n<shape_[0]) && (h<shape_[1]) && (w<shape_[2]) && (c<shape_[3]));
    return data_[n*shape_[3]*shape_[2]*shape_[1]+h*shape_[2]*shape_[3]+w*shape_[3]+c];
}

size_t *tensor::get_shape_array() {
    return shape_.empty()? nullptr: &shape_[0]; // might need to derefernce shape array later on when this function is called?
}

double *tensor::get_data_array(){
    return &data_[0];  // can we not point to the whole tensor object, why do we point to just the first element instead? 
}

std::vector<double> tensor::get_data_vector(){ /// does this defeat the purpose of keeping data vector private? why does it need to private to begin with?
    std::vector<double> copy(data_);

    return copy;
}



//old execute function
// int evaluation::execute()
// {
//     terms_.clear();
//     for(auto &expr: exprs_) {  /// for in loop
//         if (expr.get_op_type()=="Input"){
//             terms_[expr.get_expr_id()]=kwargs_[expr.get_op_name()];
//             result_=kwargs_[expr.get_op_name()];///is this right?
//         }
//         else if (expr.get_op_type()=="Add"){
//             double total = 0;
//             for(int i =0; i<expr.get_num_inputs(); i++){
//                 total += terms_[expr.get_inputs()[i]];    //////////// this should be right right? i should start at 0 because its iterating through expressions list of inputs?
//             }
//             terms_[expr.get_expr_id()]=total;
//             result_=total;
//         } else if (expr.get_op_type()=="Const"){
//             terms_[expr.get_expr_id()]= expr.get_op_params()["value"];   // the value of the constant is always under key "value" in op_params_ the value is always set right after adding the constant expressions
//             //// what happens if i have more than one constant and the value gets overwritten
//             result_= expr.get_op_params()["value"];
//         } else if (expr.get_op_type()=="Sub") {
//             double total = 0;
//             double first = terms_[expr.get_inputs()[0]];
//             double second =terms_[expr.get_inputs()[1]];
//             total = first - second;                 
//             terms_[expr.get_expr_id()]=total;
//             result_=total;
//         } else if (expr.get_op_type()=="Mul"){
//             double total = 0;
//             double first = terms_[expr.get_inputs()[0]];
//             double second =terms_[expr.get_inputs()[1]];
//             total = second * first;                 
//             terms_[expr.get_expr_id()]=total; 
//             result_=total;
//         }


//         }
//         return 0; ///// is this where I return 0 for no errors, what other integer should I return shoulnt integer be returned in get_result below?
//         /// why do in some of the function i return a 0 and in the others i dont care ???
// }
