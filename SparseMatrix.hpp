#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP


#include <map>
#include <array>
#include <vector>
#include <cstdio>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>


namespace algebra{

    /*!
    * @brief Enum class to define the storage order of the matrix
    */
    enum class StorageOrder{
        Row_wise,
        Column_wise  
    };

    /*!
    * @brief Enum class to define the norm of the matrix
    */
    enum class Norm{
        One,
        Infinity,
        Frobenius
    };


    /*!
    * @brief Class to define a sparse matrix
    * @tparam T type of the elements of the matrix
    * @tparam so storage order of the matrix
    */
    template <typename T, StorageOrder so=StorageOrder::Row_wise> class SparseMatrix{

        private:
        std::size_t n_of_row=0;
        std::size_t n_of_col=0;
        std::map<std::array<std::size_t,2>,T> data;
        std::vector<std::size_t> inner;
        std::vector<std::size_t> outer;
        std::vector<T> elements;
        bool flag=0; //if 0 is uncompressed
        
        
        public:
        SparseMatrix(std::size_t row, std::size_t col): n_of_row(row), n_of_col(col){};

        void resize(std::size_t row, std::size_t col);

        void operator()(std::size_t row, std::size_t col, T n);
        
        const bool is_compressed();

        void compress ();

        void uncompress();

        const T operator()(std::size_t row, std::size_t col);
        
        /*!
        * @brief Function to multiply a sparse matrix by a vector
        * @param T type of the elements of the matrix
        * @param so storage order of the matrix
        * @param M sparse matrix
        * @param vec vector
        * @return the result of the multiplication
        */
        friend std::vector<T> operator* (const SparseMatrix<T,so>& M,const std::vector<T>& vec){
        std::vector<T> res(M.n_of_row);
        if(M.flag==0){
            if constexpr(so==StorageOrder::Row_wise){
                auto it=M.data.begin();
                while(it!=M.data.end()){
                    res[it->first[0]]+=it->second*vec[it->first[1]];
                    ++it;
                }
            }
             if constexpr(so==StorageOrder::Column_wise){
                auto it=M.data.begin();
                while(it!=M.data.end()){
                    res[it->first[1]]+=it->second*vec[it->first[0]];
                    ++it;
                }
            }

        }
        if(M.flag==1){
            if constexpr(so==StorageOrder::Row_wise){
                for(std::size_t i=0; i<M.n_of_row; ++i){
                    T s=0;
                    for(std::size_t j=M.inner[i]; j<M.inner[i+1]; ++j){
                        s+=M.elements[j]*vec[M.outer[j]];
                    }
                    res[i]=s;
                    s=0;
                }
            }
            if constexpr(so==StorageOrder::Column_wise){
                for(std::size_t i=0; i<M.n_of_col; ++i){
                    for(std::size_t j=M.inner[i]; j<M.inner[i+1]; ++j){
                        res[M.outer[j]]+=vec[M.outer[j]]*M.elements[j];
                    }
                }
            }

        }
        return res;
        }
    
    
    void reader_market_matrix(const std::string& filename);
     
    template<Norm norm> 
    T norm();

    };
 

}

#endif