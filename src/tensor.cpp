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

size_t *tensor::get_shape_array() {
    return shape_.empty()? nullptr: &shape_[0];
}

double *tensor::get_data_array(){
    return &data_[0];
}
