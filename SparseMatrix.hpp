#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP


#include "mmio.h"
#include <map>
#include <array>
#include <vector>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>


namespace algebra{

    enum class StorageOrder{
        Row_wise,
        Column_wise  
    };


    
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

        void resize(std::size_t row, std::size_t col){
            if (flag==1){
                std::cerr<<"Can't resize in a compress form!"<<std::endl;
                return;
            }
            if(row> n_of_row )
                  n_of_row=row;
            if(col>n_of_col)
                  n_of_col=col;
            if constexpr(so==StorageOrder::Row_wise){
                auto it=data.begin();
                while(it!=data.end()){
                    if(it->first[0]>=row || it->first[1]>=col)
                        data.erase(it);
                    ++it;    
                }
                n_of_col=col;
                n_of_row=row;
                return;
            } 
            if constexpr(so==StorageOrder::Column_wise){
                auto it=data.begin();
                while(it!=data.end()){
                    if(it->first[1]>=row || it->first[0]>=col)
                        data.erase(it);
                    ++it;    
                }
                n_of_col=col;
                n_of_row=row;
                return;
            }           
                   

        }

        void operator()(std::size_t row, std::size_t col, T n){
            if (flag==0){
            std::array<std::size_t,2> indexes;
            if constexpr(so==StorageOrder::Row_wise)
                indexes={row,col};
            if constexpr(so==StorageOrder::Column_wise)
                indexes={col,row};
            //if(data.find(indexes)!=data.end()){
               // std::cerr<<"Error! Indexes already exist"<<std::endl;
                //return;}
            data[indexes]=n;
            if constexpr(so==StorageOrder::Row_wise){
                n_of_row=data.rbegin()->first[0]+1;
                std::size_t max_second_index = 0;
                for (const auto& element : data) {
                    max_second_index = std::max(max_second_index, element.first[1]);}
                n_of_col=max_second_index+1;  }
            if constexpr(so==StorageOrder::Column_wise){
                 n_of_col=data.rbegin()->first[0]+1;
                std::size_t max_second_index = 0;
                for (const auto& element : data) {
                    max_second_index = std::max(max_second_index, element.first[1]);}
                n_of_row=max_second_index+1;  }
               
            return;}
            if(flag==1){
                if constexpr(so==StorageOrder::Row_wise){
                for(std::size_t i=inner[row]; i<inner[row+1]; ++i){
                    if(outer[i]==col)
                       elements[i]=n;;
                }
                } 
                if constexpr(so==StorageOrder::Column_wise){
                for(std::size_t i=inner[col]; i<inner[col+1]; ++i){
                    if(outer[i]==row)
                         elements[i]=n;
                }
                
                }
                return;
            }

        }
        
        const bool is_compressed(){
            return flag;
        }
        

        void compress (){
            std::size_t count=0;
            inner.reserve(n_of_row+1);
            outer.reserve(data.size());
            elements.reserve(data.size());
            if constexpr(so==StorageOrder::Row_wise){
            for (std::size_t i=0; i<n_of_row; ++i){
                inner.push_back(count);
                for(std::size_t j=0; j<n_of_col; ++j){
                    if(data.find({i,j})!=data.end()){
                        elements[count]=data.at({i,j});
                        outer[count]=j;
                        count++;
                    }
                }
            }
            inner.push_back(count);
            }
            if constexpr(so==StorageOrder::Column_wise){
            for (std::size_t i=0; i<n_of_col; ++i){
                inner.push_back(count);
                for(std::size_t j=0; j<n_of_row; ++j){
                    if(data.find({i,j})!=data.end()){
                        elements[count]=data.at({i,j});
                        outer[count]=j;
                        count++;
                    }
                }
            }
            inner.push_back(count);
            }
            flag=1;
            data.clear();
        }



        void uncompress(){
            std::size_t count=0;
            if constexpr(so==StorageOrder::Row_wise){
            for(std::size_t i=0; i<n_of_row; ++i){
                for(std::size_t j=0; j<inner[i+1]; ++j){
                    data[{i,outer[j+inner[i]]}]=elements[j+inner[i]];
                }
            }
            }
            if constexpr(so==StorageOrder::Column_wise){
                for(std::size_t i=0; i<n_of_col; ++i){
                for(std::size_t j=0; j<inner[i+1]; ++j){
                    data[{i,outer[j+inner[i]]}]=elements[j+inner[i]];
                }
            }
            }
            inner.clear();
            outer.clear();
            elements.clear();
            flag=0;
        }


        const T operator()(std::size_t row, std::size_t col){ //indixes starts from zero; is it right?
        if(row<0 || row>n_of_row || col<0 || col>n_of_col ){
                     throw std::out_of_range("Out of range");}
            if(flag==0){
                std::array<std::size_t,2> indexes;
                if constexpr(so==StorageOrder::Row_wise)
                indexes={row,col};
                if constexpr(so==StorageOrder::Column_wise)
                indexes={col,row};
                if(data.at(indexes)==data.end())
                     return 0;
                return data.at(indexes);     
                
            }

            if(flag==1){
                bool check=0;
                if constexpr(so==StorageOrder::Row_wise){
                for(std::size_t i=inner[row]; i<inner[row+1]; ++i){
                    if(outer[i]==col){
                        check=1;
                        return elements[i];
                    }
                }
                if(check==0)
                    return 0;
                } 
                if constexpr(so==StorageOrder::Column_wise){
                for(std::size_t i=inner[col]; i<inner[col+1]; ++i){
                    if(outer[i]==row){
                        check=1;
                        return elements[i];
                    }
                }
                if(check==0)
                    return 0;
            
                }
            }
        }

    
        friend std::vector<T> operator*  (const SparseMatrix<T,so>& M,const std::vector<T>& vec);

        friend SparseMatrix<T,so> reader_market_matrix(const std::string& filename);


    };

    
    template <typename T, StorageOrder so=StorageOrder::Row_wise>
    std::vector<T> operator* (const SparseMatrix<T,so>& M,const std::vector<T>& vec){
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
                for(int i=0; i<M.n_of_row; ++i){
                    T s=0;
                    for(int j=0; j<M.inner[i+1]; ++j){
                        s+=M.elements[j+M.inner[i]]*vec[M.outer[j+M.inner[i]]];
                    }
                    res[i]=s;
                    s=0;
                }
            }
            if constexpr(so==StorageOrder::Column_wise){
                for(int i=0; i<M.n_of_col; ++i){
                    for(int j=0; j<M.inner[i+1]; ++j){
                        res[M.outer[j+M.inner[i]]]+=vec[M.outer[j+M.inner[i]]]*M.elements[j+M.inner[i]];
                    }
                }
            }

        }
        return res;
    }

    int read_banner(std::ifstream& infile, MM_typecode* matcode) {
        infile.seekg(0, std::ios::beg); // rewind the file
        char buffer[1024];
        infile.read(buffer, 1024);
        FILE* fp = fmemopen(buffer, 1024, "r"); // create a FILE* from the buffer
        int result = mm_read_banner(fp, matcode);
        fclose(fp); // close the FILE*
        return result;
    }

    int read_mtx_crd_size(std::ifstream& infile, int* M, int* N, int* nz) {
        infile.seekg(0, std::ios::beg); // rewind the file
        char buffer[1024];
        infile.read(buffer, 1024);
        FILE* fp = fmemopen(buffer, 1024, "r"); // create a FILE* from the buffer
        int result = mm_read_mtx_crd_size(fp, M, N, nz);
        fclose(fp); // close the FILE*
        return result;
    }

    template<typename T, StorageOrder so=StorageOrder::Row_wise>
    SparseMatrix<T,so> reader_market_matrix(const std::string& filename){
    MM_typecode matcode;
    std::ifstream infile(filename);
    int M, N, nz;
    std::vector<std::size_t> I, J;
    std::vector<T> val;

    if (read_banner(infile, &matcode) != 0)
    {
        throw std::runtime_error("Could not process Matrix Market banner.");
    }

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode))
    {
        throw std::runtime_error("Sorry, this function does not support Matrix Market type: " + std::string(mm_typecode_to_str(matcode)));
    }

    if ((read_mtx_crd_size(infile, &M, &N, &nz)) != 0)
    {
        throw std::runtime_error("Error reading matrix size.");
    }

    I.resize(nz);
    J.resize(nz);
    val.resize(nz);
    SparseMatrix<T,so> matrix(M,N);

    for (int i = 0; i < nz; i++)
    {
        infile >> I[i] >> J[i] >> val[i];
        I[i]--;  // adjust from 1-based to 0-based
        J[i]--;
        matrix(I[i],J[i],val[i]);
    }
     if (infile.is_open())
        infile.close();
    return matrix;
}



}

#endif