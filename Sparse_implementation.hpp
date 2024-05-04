#include "SparseMatrix.hpp"
#include <iostream> 
#include <fstream>
namespace algebra{

/*!
* @brief Resize the matrix
* @param row number of rows
* @param col number of columns
* @param T type of the elements of the matrix
* @param so storage order of the matrix
*/
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

/*!
* @brief Insert a value in the matrix
* @param row row index
* @param col column index
* @param n value to insert
* @param T type of the elements of the matrix
* @param so storage order of the matrix
*/
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

/*!
* @brief Check if the matrix is compressed
* @return true if the matrix is compressed
* @param T type of the elements of the matrix
* @param so storage order of the matrix
*/
template<typename T, StorageOrder so>
const bool SparseMatrix<T,so>::is_compressed(){
            return flag;
        };

/*!
* @brief Compress the matrix
* @param T type of the elements of the matrix
* @param so storage order of the matrix
*/
template<typename T, StorageOrder so>        
void SparseMatrix<T,so>::compress (){
            std::size_t count=0;
            outer.reserve(data.size());
            elements.reserve(data.size());
            if constexpr(so==StorageOrder::Row_wise){
            inner.reserve(n_of_row+1);
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
            inner.reserve(n_of_col+1);
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

/*!
* @brief Uncompress the matrix
* @param T type of the elements of the matrix
* @param so storage order of the matrix
*/
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


/*!
* @brief Multiply a matrix by a vector
* @param M matrix
* @param vec vector
* @param T type of the elements of the matrix
* @param so storage order of the matrix
* @return the result of the multiplication
*/
template<typename T, StorageOrder so> 
const T SparseMatrix<T,so>::operator()(std::size_t row, std::size_t col){ //indixes starts from zero; 
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


/*!
* @brief Multiply a matrix by a vector
* @param M matrix
* @param vec vector
* @param T type of the elements of the matrix
* @param so storage order of the matrix
* @return the result of the multiplication
*/
    template<typename T, StorageOrder so>
    void SparseMatrix<T,so>::reader_market_matrix(const std::string& filename){
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
    this->resize(M,N);

    for (std::size_t i = 0; i < nz; i++)
    {
        infile >> I[i] >> J[i] >> val[i];
        I[i]--;  // adjust from 1-based to 0-based
        J[i]--;
        this->operator()(I[i],J[i],val[i]);
        
    }
    if (infile.is_open())
        infile.close();
    };


/*!
* @brief Multiply a matrix by a vector
* @param M matrix
* @param vec vector
* @param T type of the elements of the matrix
* @param so storage order of the matrix
* @return the result of the multiplication
*/
    template<typename T, StorageOrder so>
    template<Norm n>
    T SparseMatrix<T,so>::norm(){
        if(flag==0){
            T norm=0;
            if constexpr(n==Norm::One){
                if constexpr(so==StorageOrder::Row_wise){
                    for(std::size_t i=0; i<n_of_col; ++i){
                        T s=0;
                        for(std::size_t j=0; j<n_of_row; ++j){
                            if(data.find({j,i})!=data.end())
                                s+=std::abs(data.at({j,i}));
                        }
                        norm=std::max(norm,s);
                        s=0;
                    }
                    return norm;
                }
                if constexpr(so==StorageOrder::Column_wise){
                    for(std::size_t i=0; i<n_of_col; ++i){
                        T s=0;
                        for(std::size_t j=0; j<n_of_row; ++j){
                            if(data.find({i,j})!=data.end())
                                s+=std::abs(data.at({i,j}));
                        }
                        norm=std::max(norm,s);
                        s=0;
                    }
                    return norm;
                }
            }

            if constexpr(n==Norm::Infinity){
                if constexpr(so==StorageOrder::Row_wise){
                    for(std::size_t i=0; i<n_of_row; ++i){
                        T s=0;
                        for(std::size_t j=0; j<n_of_col; ++j){
                            if(data.find({i,j})!=data.end())
                                s+=std::abs(data.at({i,j}));
                        }
                        norm=std::max(norm,s);
                        s=0;
                    }
                    return norm;
                }
                if constexpr(so==StorageOrder::Column_wise){
                    for(std::size_t i=0; i<n_of_row; ++i){
                        T s=0;
                        for(std::size_t j=0; j<n_of_col; ++j){
                            if(data.find({j,i})!=data.end())
                                s+=std::abs(data.at({j,i}));
                        }
                        norm=std::max(norm,s);
                        s=0;
                    }
                    return norm;
                }
            }

            if constexpr(n==Norm::Frobenius){
                if constexpr(so==StorageOrder::Row_wise){
                    T s=0;
                    for(std::size_t i=0; i<n_of_row; ++i){
                        for(std::size_t j=0; j<n_of_col; ++j){
                            if(data.find({i,j})!=data.end())
                                s+=(data.at({i,j}))*(data.at({i,j}));
                        }
                    }
                    norm=std::sqrt(s);
                    return norm;
                }
                if constexpr(so==StorageOrder::Column_wise){
                    T s=0;
                    for(std::size_t i=0; i<n_of_row; ++i){
                        for(std::size_t j=0; j<n_of_col; ++j){
                            if(data.find({j,i})!=data.end())
                                s+=(data.at({j,i}))*(data.at({j,i}));
                        }
                    }
                    norm=std::sqrt(s);
                    return norm;
                }
            }
        }

        if(flag==1){
            T norm=0;
            if constexpr(n==Norm::Frobenius){
                for(std::size_t i=0; i<elements.size(); ++i){
                    norm+=(elements[i]*elements[i]);
                }
                return std::sqrt(norm);
            }

            if constexpr(n==Norm::Infinity){
                if constexpr(so==StorageOrder::Row_wise){
                    for(std::size_t i=0; i<n_of_row; ++i){
                        T s=0;
                        for(std::size_t j=inner[i]; j<inner[i+1]; ++j){
                            s+=std::abs(elements[j]);
                        }
                        norm=std::max(norm,s);
                        s=0;
                    }
                    return norm;
                }
                if constexpr(so==StorageOrder::Column_wise){
                    for(std::size_t i=0; i<n_of_row; ++i){
                        T s=0;
                        for(std::size_t j=0; j<elements.size(); ++j){
                            if(outer[j]==i)
                                s+=(std::abs(elements[j]));

                        }
                        norm=std::max(norm,s);
                    }
                    return norm;
                }

            }
            if constexpr(n==Norm::One){
                if constexpr(so==StorageOrder::Row_wise){
                    for(std::size_t i=0; i<n_of_col; ++i){
                        T s=0;
                        for(std::size_t j=0; j<elements.size(); ++j){
                            if(outer[j]==i)
                                s+=(std::abs(elements[j]));

                        }
                        norm=std::max(norm,s);
                        return norm;
                    }
                }
                if constexpr(so==StorageOrder::Column_wise){
                    for(std::size_t i=0; i<n_of_col; ++i){
                        T s=0;
                        for(std::size_t j=inner[i]; j<inner[i+1]; ++j){
                            s+=std::abs(elements[j]);
                        }
                        norm=std::max(norm,s);
                        s=0;
                    }
                    return norm;
                }
           }
        }  
    return norm;
    };

};



     



