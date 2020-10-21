#ifndef TENSOR_H
#define TENSOR_H /// what is the purpose of these
#include <vector>

class tensor {
    public:
        tensor(); //scalar 0
        tensor(double v); // scalar v
        tensor(int dim, size_t shape[], double data[]);
        double item() const;
        double &item();
        double at(size_t i) const;
        double at(size_t i, size_t j) const;
        int get_dim() const;

        size_t *get_shape_array();
        double *get_data_array();
        


    private:
        std::vector<size_t> shape_;
        std::vector<double> data_;


};


#endif // what does this do?