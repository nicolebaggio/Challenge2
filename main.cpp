#include "SparseMatrix.hpp"
#include "Sparse_implementation.hpp"
#include "chrono.hpp"
#include <complex>
#include <iostream>
#include <vector>
using namespace algebra;
using namespace Timings;


int main(){
    SparseMatrix<double, StorageOrder::Row_wise> matrix(131,131);
    SparseMatrix<std::complex<double>, StorageOrder::Row_wise> matrix2(131,131);
    matrix.reader_market_matrix("lnsp_131.mtx");
    matrix2.reader_market_matrix("lnsp_131.mtx");
    std::cout<<"The Frobenius norm of the matrix is: "<<matrix.norm<Norm::Frobenius>()<<std::endl;
    std::cout<<"The One norm of the matrix is: "<<matrix.norm<Norm::One>()<<std::endl;
    std::cout<<"The Infinity norm of the matrix is: "<<matrix.norm<Norm::Infinity>()<<std::endl;
    std::vector<double> vec(131,0);
    std::vector<std::complex<double>> vec2(131,0);
    vec[0]=1; vec2[0]=1;
    Chrono clock;
    clock.start();
    std::vector<double> res_uncom=matrix*vec;
    std::vector<std::complex<double>> res_uncom_2=matrix2*vec2;
    std::cout << "Elapsed time for uncompressed method" << clock.wallTimeNow() << " microseconds"
            << std::endl;

    std::cout<<"The product of the uncompressed matrix and the vector is:"<<std::endl;
    for(std::size_t i=0; i<131; ++i){
        std::cout<< res_uncom[i]<<std::endl;
    }

    std::cout<<"The product of the uncompressed matrix (with complex data) and the vector is:"<<std::endl;
    for(std::size_t i=0; i<131; ++i){
        std::cout<< res_uncom_2[i]<<std::endl;
    }
    
    matrix.compress();
    matrix2.compress();
    std::cout<<"The Frobenius norm of the matrix is: "<<matrix.norm<Norm::Frobenius>()<<std::endl;
    std::cout<<"The One norm of the matrix is: "<<matrix.norm<Norm::One>()<<std::endl;
    std::cout<<"The Infinity norm of the matrix is: "<<matrix.norm<Norm::Infinity>()<<std::endl;
    std::vector<double> res_com=matrix*vec;
    std::vector<std::complex<double>> res_com_2=matrix2*vec2;
    clock.stop();
    std::cout <<"For the compress method the " << clock;

    std::cout<<"The product of the compresed matrix and the vector is:"<<std::endl;
    for(std::size_t i=0; i<131; ++i){
        std::cout<< res_com[i]<<std::endl;
    }
    std::cout<<"The product of the compressed matrix (complex data) and the vector is:"<<std::endl;
     for(std::size_t i=0; i<131; ++i){
        std::cout<< res_com_2[i]<<std::endl;
    }


    return 0;
 
}


/* To try with a smaller matrix:
int main(){
    SparseMatrix<double, StorageOrder::Column_wise> matrix(3,3);
    matrix(0,0,1);
    matrix(1,1,1);
    matrix(2,2,1);
    std::vector<double> vec(3,0);
    vec[0]=1;
    std::vector<double> res_uncom=matrix*vec;
    std::cout<<"The Frobenius norm of the matrix is: "<<matrix.norm<Norm::Frobenius>()<<std::endl;
    std::cout<<"The One norm of the matrix is: "<<matrix.norm<Norm::One>()<<std::endl;
    std::cout<<"The Infinity norm of the matrix is: "<<matrix.norm<Norm::Infinity>()<<std::endl;
    std::cout<<"The product of the uncompressed matrix and the vector is:"<<std::endl;
    for(std::size_t i=0; i<3; ++i){
        std::cout<< res_uncom[i]<<std::endl;
    }
    matrix.compress();
    std::vector<double> res_com=matrix*vec;
    std::cout<<"The Frobenius norm of the matrix is: "<<matrix.norm<Norm::Frobenius>()<<std::endl;
    std::cout<<"The One norm of the matrix is: "<<matrix.norm<Norm::One>()<<std::endl;
    std::cout<<"The Infinity norm of the matrix is: "<<matrix.norm<Norm::Infinity>()<<std::endl;
    std::cout<<"The product of the compresed matrix and the vector is:"<<std::endl;
    for(std::size_t i=0; i<3; ++i){
        std::cout<< res_com[i]<<std::endl;
    }

}
*/