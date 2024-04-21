#include <iostream>
#include <map>
#include <array>
#include <vector>

namespace algebra{
    enum class StorageOrder{
        Row_wise,
        Column_wise  
    };


    template <typename T, StorageOrder so=StorageOrder::Row_wise> 
    class SparseMatrix{
        private:
        std::map<std::array<std::size_t,2>,T> data;
        std::vector<std::size_t> inner;
        std::vector<std::size_t> outer;
        std::vector<T> elements;
        bool flag=0; //if 0 is uncompressed
        std::size_t n_of_col=0;
        std::size_t n_of_row=0;
        
        public:
        SparseMatrix(std::size_t row, std::size col): n_of_row(row), n_of_col(col){};

        void resize(std::size_t row, std::size col){
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
                        data.erase(it->first);
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

        void operator()(int row,int col, T n){
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
            n_of_row=data.rbegin()->first[0]+1;
            std::size_t max_second_index = 0;
            for (const auto& element : data) {
                 max_second_index = std::max(max_second_index, element.first[1]);}
            n_of_col=max_second_index+1;     
            return;}
            if(flag==1){
                if constexpr(so==StorageOrder::Row_wise){
                for(int i=inner[row]; i<inner[row+1]; ++i){
                    if(outer[i]==col)
                       elements[i]=n;;
                }
                } 
                if constexpr(so==StorageOrder::Column_wise){
                for(int i=inner[col]; i<inner[col+1]; ++i){
                    if(outer[i]==row)
                         elements[i]=n;
                }
                
                }
                
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
                    if(data.at({i,j})!=data.end()){
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
                    if(data.at({i,j})!=data.end()){
                        elements.[count]=data.at({i,j});
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
            for(int i=0; i<n_of_row; ++i){
                for(int j=0; j<inner[i+1]; ++j){
                    data[{i,outer[j+inner[i]]}]=elements[j+inner[i]];
                }
            }
            }
            if constexpr(so==StorageOrder::Column_wise){
                for(int i=0; i<n_of_col; ++i){
                for(int j=0; j<inner[i+1]; ++j){
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
                     throw std::out_of_range("Out of range")}
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
                for(int i=inner[row]; i<inner[row+1]; ++i){
                    if(outer[i]==col){
                        check=1;
                        return elements[i];
                    }
                }
                if(check==0)
                    return 0;
                } 
                if constexpr(so==StorageOrder::Column_wise){
                for(int i=inner[col]; i<inner[col+1]; ++i){
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

        friend std::vector<T> operator*(std::vector<T> vec);


    

    };

    template<typename T>
    std::vector<T> operator*(std::vector<T> vec){
        std::vector<T> res(n_of_row);
        if(flag==0){
            if constexpr(so==StorageOrder::Row_wise){
                auto it=data.begin();
                while(it!=data.end()){
                    res[it->first[0]]+=it->second*vec[it->first[1]];
                    ++it;
                }
            }
             if constexpr(so==StorageOrder::Column_wise){
                auto it=data.begin();
                while(it!=data.end()){
                    res[it->first[1]]+=it->second*vec[it->first[0]];
                    ++it;
                }
            }

        }
        if(flag==1){
            if constexpr(so==StorageOrder::Row_wise){
                for(int i=0; i<n_of_row; ++i){
                    T s=0;
                    for(int j=0; j<inner[i+1]; ++j){
                        s+=elements[j+inner[i]]*vec[outer[j+inner[i]]];
                    }
                    res[i]=s;
                    s=0;
                }
            }
            if constexpr(so==StorageOrder::Column_wise){
                for(int i=0; i<n_of_col; ++i){
                    for(int j=0; j<inner[i+1]; ++j){
                        res[outer[j+inner[i]]]+=vec[outer[j+inner[i]]]*elements[j+inner[i]];
                    }
                }
            }

        }
        return res;
    }



    
}
