
# Challenge 2
In this code, I manage Sparse matrix. In particular, I stored them in an uncompressed and in a compressed way. Moreover, I distinguish the cases where elements are logically ordered column-wise or row-wise.
For the uncompressed way I use a map, choosing as the key {i,j} for the row-wise order and {j,i} for the col-wise order.
For the compressed way, I use the CSR and CSC techniques.
All of this information are in the "SparseMatrix" class.


## Description of the code
All the parameters which describe the matrix (the map, the inner vector, the outer vector, the vector of non-zero elements)are stored as private elements of the class.
Moreover, I provide the const and the non const operator() to access/add directly an element of the matrix.
There is also a compress() method which stored a matrix in a compressed way, and likely there is an uncompressed() method which stored the matrix in the uncompressed way.
To report whether the matrix is compressed or uncompressed, I use the private element "flag" which is 0 if is uncompressed and 1 if is compressed.
You can notice that the class is a template class, so you can choose in the main which type of elements you will store in the matrix, and which logically-order (col-wise or row-wise) use.
The implementation of those methods is in the file "Sparse_implementation.hpp"

## Reader of matrices written in matrix market format
A useful method you can find in the code is the reader_market_matrix(const std::string& filename), stored in the class.
With this method you can read a matrix market format and stored it as a member of the SparseMatrix class.

## operator *
Finally, I have implemented a friend function that calculates the product of the sparse matrix with a vector.
It performs the operator* in all the possible cases: compressed format or uncompressed format, and row-wise or col-wise order.
The function is in the class "SparseMatrix". 

## Norm
The SparseMatrix class also have a method which evaluates the One norm, the Infinity norm, and the Frobenius norm

## How to compile the code
I have implemented the MakeFile, so to compile the code you will only to run the "make" command.

## How to test the code
In the main I test the code with the matrixmarket file "lnsp.131", which stores a Sparse matrix of order (131x131) with 536 non-zero elements.
First of all I read the matrix, the I evaluate all the norms and I perform the product with a vector (131x1).
The, I compressed the matrix and I repeat the steps.
I did it both with double-type elements and with complex<double> elements.