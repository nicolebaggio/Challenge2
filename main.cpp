#include "SparseMatrix.hpp"
#include "chrono.hpp"
#include "mmio.h"
#include <iostream>
#include <vector>
using namespace algebra;
using namespace Timings;

int main(){
    SparseMatrix<double, StorageOrder::Row_wise> matrix=reader_market_matrix<double,StorageOrder::Row_wise >("Insp_131.mtx");
    std::vector<double> vec(131,0);
    vec[0]=1;
    Chrono clock;
    clock.start();
    std::vector<double> res_uncom=matrix*vec;
    std::cout << "Elapsed time for compressed method" << clock.wallTimeNow() << " microseconds"
            << std::endl;
    for(std::size_t i=0; i<131; ++i){
        std::cout<< res_uncom[i]<<std::endl;
    }
    matrix.compress();
    std::vector<double> res_com=matrix*vec;
    clock.stop();
    std::cout <<"For the uncompressd method the" << clock;
    for(std::size_t i=0; i<131; ++i){
        std::cout<< res_com[i]<<std::endl;
    }
 
}