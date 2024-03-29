#include <assert.h>
#include "evaluation.h"
#include <vector>
#include <iostream>


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
            terms_[expr.get_expr_id()] = result_;
            std::cout<<"relu"<<std::endl;

        } else if (expr.get_op_type()=="Flatten") {
            size_t first = terms_[expr.get_inputs()[0]].get_shape_array()[0];
            size_t second = terms_[expr.get_inputs()[0]].get_shape_array()[1]*terms_[expr.get_inputs()[0]].get_shape_array()[2]*terms_[expr.get_inputs()[0]].get_shape_array()[3];
            size_t sha[]={first,second};
            result_ = tensor(2,sha,terms_[expr.get_inputs()[0]].get_data_array());
            terms_[expr.get_expr_id()] = result_;
            std::cout<<"flatten"<<std::endl;

        } else if (expr.get_op_type()=="Input2d"){
            tensor a = kwargs_[expr.get_op_name()];
            size_t N = a.get_shape_array()[0];
            size_t H = a.get_shape_array()[1];
            size_t W = a.get_shape_array()[2];
            size_t C = a.get_shape_array()[3];
            double dat[N*C*H*W];
            for (size_t n = 0; n< N; n++){
                for (size_t h =0; h<H; h++){
                    for (size_t w = 0; w<W; w++){
                        for (size_t c =0; c<C; c++){
                            // std::cout<< a.get_shape_array()[0] <<' '<< H << ' ' << W << ' ' << C << std::endl;
                            // std::cout<< n <<' '<< h << ' ' << w << ' ' << c << std::endl;
                            dat[n*W*H*C + c*W*H + h*W + w] = a.at(n,h,w,c);                            
                        }
                    }
                }
            } 
            size_t sha[] = {N, C, H, W}; 
            result_ = tensor(4, sha, dat);
            terms_[expr.get_expr_id()] = result_;
            std::cout<<"input2d"<<std::endl;
        } else if (expr.get_op_type()=="Linear"){
            tensor bias =expr.get_op_params()["bias"];
            tensor weight =expr.get_op_params()["weight"];
            tensor x = terms_[expr.get_inputs()[0]];
            size_t N = x.get_shape_array()[0];  
            size_t O = weight.get_shape_array()[0];  
            size_t I = x.get_shape_array()[1];

            std::vector<double> trans_vec;
            for (size_t j = 0; j<I; j++){
                for (size_t i =0; i<N; i++){            ///////////////transpose the input array using vector
                    trans_vec.push_back(x.at(i,j));
                }
            }

            double xt[trans_vec.size()];
            for (size_t i =0; i<trans_vec.size(); i++){
                xt[i]=trans_vec[i];                         //////////// put the vecotr into array format so it can be input into temp tensor for mat mul
            }
            
            //std::cout<< "here"<< std::endl;
            size_t d[2]= {I,N};
            tensor tmp =tensor(2,d, xt); ///tmp is the matrix with transposed input so i can use at in multiplication below

            double total;
            double dat[N*O];
            for (size_t i = 0; i<O; ++i){                
                for (size_t j = 0; j<N; ++j){
                    total = 0;
                    for (size_t p = 0; p<I; ++p){
                        total+= weight.at(i,p)*tmp.at(p,j);                  /// A[i][x]*B[x][j]
                    }
                    dat[i*N+j] = total;
                }
            }
            //std::cout<< "here"<< std::endl;
            for (size_t q = 0; q<O; ++q){ ///// DOES SAME BIAS COLUMN VECTOR GET ADDED TO EVERY COLUMN?!?!?!?!?!
                for (size_t m = 0; m<N; ++m){
                    dat[q*N + m] = dat[q*N + m] + bias.get_data_array()[q];
                }
            }
            std::vector<double> tmp_vec;
            size_t shatwo[2] = {O,N};
            tensor tmp_tens = tensor(2,shatwo, dat);
            for (size_t j =0; j<N; j++){
                for (size_t i =0; i<O; i++){
                    tmp_vec.push_back(tmp_tens.at(i,j));
                }
            }

            
            
            double finaldat[N*O];
            for (size_t i =0; i<tmp_vec.size(); i++){
                finaldat[i]=tmp_vec[i];                         //////////// put the vecotr into array format so it can be input into temp tensor for mat mul
            }
            
            size_t shathree[2] = {N,O};
            result_= tensor(2, shathree, finaldat);
            terms_[expr.get_expr_id()]= result_;
            std::cout<<"linear"<<std::endl;
        } else if (expr.get_op_type()=="Conv2d"){
            tensor bias =expr.get_op_params()["bias"];
            tensor weight =expr.get_op_params()["weight"];
            tensor ks =expr.get_op_params()["kernel_size"];
            size_t out_c = weight.get_shape_array()[0];
            size_t in_c = weight.get_shape_array()[1];
            size_t kernel_size = ks.item();
            size_t N = terms_[expr.get_inputs()[0]].get_shape_array()[0];
            size_t H = terms_[expr.get_inputs()[0]].get_shape_array()[2];
            size_t W = terms_[expr.get_inputs()[0]].get_shape_array()[3];
            tensor a = terms_[expr.get_inputs()[0]];
                                                  ////////// is it a bad implementation time wise to the use std vector and then copy that to data array
                                                  /// why couldnt we have just used standard vectors to hold data in tensors from the beginning
                                                  /// is it so it can go to c and then from there to python?
            double total = 0;
            std::vector<double> tot;
            for (size_t n = 0; n<N; n++){
                for (size_t out = 0; out< out_c; out++){
                        //going to loop twice more based on kernel size and on height wand width of the input matrix
                        // looking 2d input tensor a.at(N, in_, i, j) and weight.at(out, in_, p, q) the in_s should match right?
                    
                    for (size_t i = 0; i < H-kernel_size+1; i++){
                        for (size_t j =0; j< W -kernel_size+1; j++){
                            total=0;
                            for (size_t p =0; p<kernel_size; p++){
                                for (size_t q =0; q<kernel_size; q++){
                                    for (size_t in_ =0; in_<in_c; in_++){
                                        total += a.at(n, in_, i+p, j+q)*weight.at(out, in_, p, q);
                                    }
                                }
                            } 
                            tot.push_back(total+bias.get_data_array()[out]);////check this                               
                        }                  
                    }
                }
            }/// add the bias!!!
            std::cout<<tot.size()<<std::endl;
            double data_array[tot.size()];
            for (size_t i=0; i<tot.size(); i++){
                data_array[i]=tot[i];
            }

            size_t sha[4] = {N, out_c, H-kernel_size+1, W-kernel_size+1 };
            
            result_= tensor(4, sha, data_array);
            terms_[expr.get_expr_id()]= result_;
            std::cout<<"conv"<<std::endl;

        }  else if (expr.get_op_type()=="MaxPool2d"){
            tensor a = terms_[expr.get_inputs()[0]];
            size_t N = a.get_shape_array()[0];
            size_t C = a.get_shape_array()[1];
            size_t H = a.get_shape_array()[2];
            size_t W = a.get_shape_array()[3];
            tensor ks =expr.get_op_params()["kernel_size"];
            size_t kernel_size = ks.item();
            double max;
            std::vector<double> res_vec;
            for (size_t n =0; n<N; n++){
                for (size_t c =0; c<C; c++){
                    for (size_t row =0; row<H; row=row+kernel_size){
                        for (size_t col =0; col<W; col=col+kernel_size){
                            max= a.at(n,c,row,col);/// initialize max
                            for (size_t i = 0; i<kernel_size; i++){
                                for (size_t j =0; j<kernel_size; j++){
                                    if (a.at(n,c,i+row,j+col)>max){
                                        max=a.at(n,c,i+row,j+col);
                                    }
                                }
                            }
                            res_vec.push_back(max);
                        }
                    }
                }
            }
            size_t sha[4] = {N,C, H/kernel_size, W/kernel_size};
            double res_data[res_vec.size()];
            for (size_t i =0; i<res_vec.size(); i++){
                res_data[i]=res_vec[i];
            }
            
            result_ = tensor(4,sha, res_data);
            terms_[expr.get_expr_id()] = result_;
            std::cout<<"maxpool"<<std::endl;
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
