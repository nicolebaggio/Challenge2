#include "SparseMatrix.hpp"
#include <iostream> 
namespace algebra{

template<typename T, StorageOrder so>
void SparseMatrix<T,so>::resize(std::size_t row, std::size_t col){
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
};

template<typename T, StorageOrder so>
void SparseMatrix<T,so>::operator()(std::size_t row, std::size_t col, T n){
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

};

template<typename T, StorageOrder so>
const bool SparseMatrix<T,so>::is_compressed(){
            return flag;
        };

template<typename T, StorageOrder so>        
void SparseMatrix<T,so>::compress (){
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
 };

template<typename T, StorageOrder so>     
 void SparseMatrix<T,so>::uncompress(){
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
        };


template<typename T, StorageOrder so> 
const T SparseMatrix<T,so>::operator()(std::size_t row, std::size_t col){ //indixes starts from zero; is it right?
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
        };


    template<typename T, StorageOrder so>
    SparseMatrix<T,so> SparseMatrix<T,so>::reader_market_matrix(const std::string& filename){
        std::ifstream infile(filename);
        if (!infile) {
          std::cerr << "Error opening file" << std::endl;
          exit(1);
      }
    std::size_t M,N,nz;
    std::vector<std::size_t> I, J;
    std::vector<T> val;
    std::string line;
    getline(infile, line) ;
    if(line!="%%MatrixMarket matrix coordinate real general"){
        std::cerr<<"Error! The file is not in the right format"<<std::endl;
        exit(1);
    }
    infile >> M >> N >> nz;       
    I.resize(nz);
    J.resize(nz);
    val.resize(nz);
    SparseMatrix<T,so> matrix(M,N);

    for (std::size_t i = 0; i < nz; i++)
    {
        infile >> I[i] >> J[i] >> val[i];
        I[i]--;  // adjust from 1-based to 0-based
        J[i]--;
         matrix(I[i],J[i],val[i]);
    }
    if (infile.is_open())
        infile.close();
    return matrix;
    };




   /* MM_typecode matcode;
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
    return matrix;*/
};

     



