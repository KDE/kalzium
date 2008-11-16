// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2007 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with Eigen; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. This exception does not invalidate any other reasons why a work
// based on this file might be covered by the GNU General Public License.

/** \file ludecompositionbase.h
  * \brief Internal file
  */

#ifndef EIGEN_LUDECOMPOSITIONBASE_H
#define EIGEN_LUDECOMPOSITIONBASE_H

#include "util.h"

namespace Eigen
{

/** \ingroup internalbases
  *
  * \ingroup ludecomp
  *
  * \brief internal base class
  *
  * This class template is only internally used in Eigen.
  */
template<typename T,
         typename MatrixType,
         typename VectorType,
         typename IntVecType>
class LUDecompositionBase
{

public:

    ~LUDecompositionBase() {}

    /**
      * This method returns the determinant of the square matrix A of which
      * *this is the LU decomposition. It has only linear complexity
      * (that is, O(n) where n is the dimension of the square matrix)
      * as the LU decomposition has already been computed.
      *
      * Warning: a determinant can be very big or small, so for matrices
      * of large dimension (like a 50-by-50 matrix) there can be a risk of
      * overflow/underflow.
      */
    T determinant() const;

    /**
      * Returns the dimension (size) of square matrix A
      * of which *this is the LU decomposition -- just in case you forgot it!
      */
    int dim() const
    { return m_dim; }

    /**
      * Returns the dimension of the kernel of the square matrix A
      * of which *this is the LU decomposition. It is very fast because the
      * computation has already been done during the LU decomposition.
      */
    int dimKer() const
    { return m_dimKer; }

    /**
      * Returns the rank of the square matrix A of which *this is
      * the LU decomposition. It is very fast because the
      * computation has already been done during the LU decomposition.
      */
    int rank() const
    { return( m_dim - m_dimKer ); }

    /**
      * Returns true if the square matrix A, of which *this is
      * the LU decomposition, is invertible. It returns false if it is singular.
      * It is very fast because the computation has already been done during
      * the LU decomposition.
      */
    bool isInvertible() const
    { return( m_dimKer == 0 ); }

    /**
      * This method computes a basis of the kernel of the matrix A of which
      * *this is the LU decomposition.
      *
      * The \a result argument is a pointer to the matrix in which the result
      * will be stored. After calling this method, the \a dimKer() first
      * columns of \a result form a basis of the kernel of A. If A
      * is invertible, then dimKer()==0 and this method does nothing.
      *
      * @returns true is the matrix is non-invertible, hence has a nonzero
      * kernel; false if the matrix is invertible.
      */
    bool computeBasisKer( MatrixType * result) const;

    /**
      * Computes an antecedent of \a v by the matrix \a A of which *this is a
      * LU decomposition. In other words, this method computes a vector \a u
      * such that \a A \a u = \a v.
      * If such an antecedent \a u doesn't exist, this method does nothing.
      *
      * @returns true if an antecedent exists, false if no antecedent exists.
      *
      * Notes:
      *
      * 1. The returned vector u is only one solution of the equation Au=v,
      * which can have more than one solution if A is non-invertible. To get
      * a basis of the whole (affine) space of solutions, use computeBasisKer().
      *
      * \sa computeBasisKer()
      */
    bool computeSomeAntecedent
    ( const VectorType & v, VectorType * result ) const;

    /**
      * Computes the inverse matrix of A, where A
      * is the matrix of which *this is the LU decomposition, and
      * stores it in *result.
      *
      * If A is non-invertible, this method does nothing.
      *
      * \returns true if A is invertible, false otherwise.
      *
      */
    bool computeInverse( MatrixType * result ) const;

    /**
      * This methods returns the inverse matrix of A, where A
      * is the matrix of which *this is the LU decomposition. If A is
      * non-invertible, the returned value is undefined.
      *
      * This method calls computeInverse(), so the same remarks as for
      * computeInverse() apply here.
      *
      * \return_by_value
      *
      * \sa computeInverse()
      */
    MatrixType inverse() const
    {
        MatrixType m( m_LU.size() );
        computeInverse( &m );
        return m;
    }

    /**
      * Returns the member m_LU, which stores the matrices L and U.
      * See member m_LU and the class's comment.
      */
    MatrixType & LU() { return m_LU; }
    const MatrixType & LU() const { return m_LU; }

    /**
      * Returns the member m_P, which stores the permutation P.
      * See member m_P and the class's comment.
      */
    IntVecType & P() { return m_P; }
    const IntVecType & P() const { return m_P; }


    /**
      * Returns the member m_Q, which stores the permutation Q.
      * See member m_Q and the class's comment.
      */
    IntVecType & Q() { return m_Q; }
    const IntVecType & Q() const { return m_Q; }

protected:

    /**
      * The helper method actually computing the LU decomposition.
      * Called by the constructor.
      *
      * For internal reasons it is public, but you should never call it.
      */
    void perform ( const MatrixType & A );

    /**
      * The dimension of the matrix A of which *this is the LU decomposition
      */
    int m_dim;

    /**
      * The permutation matrices P and Q are stored in these permutations p, q.
      * They are understood as follows: the permutation p maps k to p[k].
      * Same for q. So, in terms of matrices, P moves the k-th row to the
      * p[k]-th row, and Q moves the k-th column to the q[k]-th column.
      */
    IntVecType m_P, m_Q;

    /**
      * This is equal to the determinant of the product matrix PQ, or
      * equivalently, to the signature of the permutation pq.
      * This is used by the determinant() method.
      */
    int m_detPQ;

    /**
      * This matrix holds the data of the L and U matrices of the LU
      * decomposition, as follows. The part above the diagonal (including
      * the diagonal) is U. The part strictly below the diagonal is L.
      * As U is upper triangular, L is lower triangular, and L has its diagonal
      * entries all equal to 1, this holds all the data of the matrices L and U.
      * Also note that the Eigenvalues of U are stored in decreasing order of
      * absolute value.
      */
    MatrixType m_LU;

    /**
      * The dimension of the kernel of the square matrix A
      * of which *this is the LU decomposition.
      */
    int m_dimKer;

    /**
      * The Eigenvalue of U that has biggest absolute value.
      */
    T m_biggestEigenValueU;

};

template<typename T,
         typename MatrixType,
         typename VectorType,
         typename IntVecType>
void LUDecompositionBase<T, MatrixType, VectorType, IntVecType>::perform
( const MatrixType & A )
{
    m_dim = A.size();

    m_P.resize( m_dim );
    m_Q.resize( m_dim );


    //initially set m_LU to be a copy of A
    m_LU = A;

    // initially set P and Q to be the identity permutation
    for( int k = 0; k < dim(); k++ )
    {
        P()[k] = k;
        Q()[k] = k;
    }

    // if dim() == 1, then we're already almost done!
    if( dim() == 1 )
    {
        m_detPQ = 1;
        m_biggestEigenValueU = LU().array()[0];
        m_dimKer = ( std::abs(m_biggestEigenValueU) == static_cast<T>(0) );
            // there's nothing to compare m_biggestEigenValueU to, so we have
            // to content ourselves with that test. abs() is here because
            // in some implementations, std::complex has no operator==.
        return;
    }

    // temporary arrays used to store the lists of transpositions whose products
    // will be P and Q respectively
    IntVecType transpositionsP( dim() );
    IntVecType transpositionsQ( dim() );

    // the number of transpositions stored in transpositionsP
    // and transpositionsQ. This will be used to compute m_detPQ.
    int number_of_transpos = 0;

    // Let's now perform the LU decomposition with complete pivoting
    // following Algorithm 3.4.2 in the book Matrix Computations (3rd ed.)
    // by Golub & van Loan
    for( int k = 0; k <= dim() - 2; k++)
    {
        // Determine row_max_abs and col_max_abs greater or equal to k
        // maximizing abs(LU()(row_max_abs, col_max_abs))
        int row_max_abs, col_max_abs, row, col;

        LU().findBiggestEntry( &row_max_abs, &col_max_abs, k );

        T biggestEntry = LU()( row_max_abs, col_max_abs );

        // swap the k-th and the row_max_abs-th rows
        T *row_k_ptr = LU().array() + k;
        T *row_max_abs_ptr = LU().array() + row_max_abs;
        for( col = 0; col < dim(); col++ )
        {
            T tmp = *row_k_ptr;
            *row_k_ptr = *row_max_abs_ptr;
            *row_max_abs_ptr = tmp;
            row_k_ptr += dim();
            row_max_abs_ptr += dim();
        }

        // swap the k-th and the col_max_abs-th columns
        T *col_k_ptr = LU().array() + k * dim();
        T *col_max_abs_ptr = LU().array() + col_max_abs * dim();
        for( row = 0; row < dim(); row++ )
        {
            T tmp = *col_k_ptr;
            *col_k_ptr = *col_max_abs_ptr;
            *col_max_abs_ptr = tmp;
            col_k_ptr++;
            col_max_abs_ptr++;
        }

        // store the transpositions
        transpositionsP[k] = row_max_abs;
        transpositionsQ[k] = col_max_abs;
        number_of_transpos += ( k != row_max_abs ) + ( k != col_max_abs );

        // test if the diagonal entry LU()(k, k) is nonzero
        T lu_k_k = LU()(k, k);
        if( ! Util::isNegligible( lu_k_k, biggestEntry ) )
        {
            // divide entries (k+1...n) in column k by lu_k_k
            T *entry_ptr = &(LU()( k + 1, k ));
            for( row = k + 1; row < dim(); row++ )
            {
                *entry_ptr /= lu_k_k;
                entry_ptr++;
            }

            // for row and col greater or equal to k+1, perform
            // LU()(row,col) -= LU()(row,k) * LU()(k,col)
            entry_ptr = &(LU()( k + 1, k + 1 ));
            row_k_ptr = &(LU()( k, k + 1));
            col_k_ptr = &(LU()( k + 1, k ));
            for( col = k + 1; col < dim(); col++ )
            {
                for( row = k + 1; row < dim(); row++ )
                {
                    *entry_ptr -= (*row_k_ptr) * (*col_k_ptr);
                    col_k_ptr++;
                    entry_ptr++;
                }
                col_k_ptr -= dim() - k - 1;
                row_k_ptr += dim();
                entry_ptr += k + 1;
            }
        }
    }
    // end of algorithm 3.4.2 in the book by Golub & van Loan.
    // it now remains to compute the permutations P, Q, the determinant
    // m_detPQ, the biggest Eigenvalue m_biggestEigenValueU of U,
    // and the dimension m_dimKer of the kernel of A.

    // let's first compute P and Q. They are the products of the transpositions
    // in transpositionsP and transpositionsQ, but not in the same order
    // (and order does matter here).

    for( int k = 0; k <= dim() - 2; k++)
    {
        int tmp;
        tmp = Q()[k];
        Q()[k] = Q()[ transpositionsQ[k] ];
        Q()[ transpositionsQ[k] ] = tmp;

        int i = dim() - 2 - k;
        tmp = P()[i];
        P()[i] = P()[ transpositionsP[i] ];
        P()[ transpositionsP[i] ] = tmp;
    }

    // let's now compute m_detPQ
    if( number_of_transpos % 2 ) // if number_of_transpos is odd
        m_detPQ = -1;
    else
        m_detPQ = 1;

    // let's now compute m_biggestEigenValueU
    // the diagonal entries in LU() are the Eigenvalues of U
    m_biggestEigenValueU = static_cast<T>(0);
    for( int k = 0; k <= dim() - 1; k++)
        if( std::abs( LU()( k, k ) ) > std::abs( m_biggestEigenValueU ) )
            m_biggestEigenValueU = LU()( k, k );

    // at last, let's compute the dimension m_dimKer of the kernel of A
    // it suffices to count how many Eigenvalues of U are zero
    // (as P, L and Q are invertible).
    m_dimKer = 0;
    for( int k = 0; k <= dim() - 1; k++)
        m_dimKer += Util::isNegligible( LU()( k, k ), m_biggestEigenValueU );
}

template<typename T,
         typename MatrixType,
         typename VectorType,
         typename IntVecType>
T LUDecompositionBase<T, MatrixType, VectorType, IntVecType>::determinant() const
{
    if( ! isInvertible() ) return static_cast<T>(0);
    T ret = m_detPQ;
    for (int i = 0; i < dim() * dim(); i += (dim()+1) )
        ret *= LU().array()[i];
    return ret;
}

template<typename T,
         typename MatrixType,
         typename VectorType,
         typename IntVecType>
bool LUDecompositionBase<T, MatrixType, VectorType, IntVecType>::computeBasisKer
( MatrixType * result) const
{
    if( isInvertible() ) return false;
    result->resize( dim() );

    /* Let us use the following lemma:
     *
     * Lemma: If the matrix A has the LU decomposition PAQ = LU,
     * then Ker A = Q( Ker U ).
     *
     * Proof: trivial: just keep in mind that P, Q, L are invertible.
     */

    /* Thus, all we need to do is to compute Ker U, and then apply Q.
     *
     * U is upper triangular, with Eigenvalues sorted in decreasing order of
     * absolute value. Thus, the diagonal of U ends with exactly
     * m_dimKer zero's. Let us use that to construct m_dimKer linearly
     * independent vectors in Ker U.
     */

    VectorType tmpvector(dim());

    for( int k = 0; k < m_dimKer; k++ )
    {
        // let tmpvector end with 0,...,0,1,0,...,0 where the 1 occurs at
        // position k counting from the last one.
        for( int i = 0; i < m_dimKer; i++ )
        {
            tmpvector[ dim() - 1 - i ] = static_cast<T>( i == k );
        }

        /* now comes the part that's difficult to explain in a comment...
         * We've just set the m_dimKer last coords of tmpvector.
         * Now we want to compute the (dim()-m_dimKer) first coords.
         * As U has rank dim()-m_dimKer, these are uniquely determined.
         * We just have to solve the equation VX=Y where V is the square
         * matrix formed of the (dim()-m_dimKer) first rows and columns
         * of U, X is what we're looking for, and Y is minus the
         * (dim() - 1 - k)-th column of U.
         */

        const T *colptr = LU().array() + (dim() - 1 - k) * dim();
        int maxrow = dim() - m_dimKer - 1;
        for( int i = maxrow; i >= 0; i-- ) // i must be signed!
        {
            T *tmpvecptr = &(tmpvector[i]);
            *tmpvecptr = - colptr[i];
            const T *rowptr = &( LU()( i, i ) );
            const T *denomptr = rowptr;
            for( int j = i + 1; j <= maxrow; j++ )
            {
                rowptr += dim();
                *tmpvecptr -= tmpvector[j] * (*rowptr);
            }
            *tmpvecptr /= *denomptr;
        }

        //now tmpvector is really a vector in Ker U, so it only remains to
        //apply Q to it and store the result as the k-th column of *ret.
        for( int i = 0; i < dim(); i++)
            (*result)(Q()[i], k) = tmpvector[i];
    }
    return true;
}

template<typename T,
         typename MatrixType,
         typename VectorType,
         typename IntVecType>
bool LUDecompositionBase<T, MatrixType, VectorType, IntVecType>::computeSomeAntecedent
( const VectorType & v, VectorType * result ) const
{
    /* The decomposition PAQ = LU can be rewritten as A = P^{-1} L U Q^{-1}.
     * So we proceed as follows:
     * Step 1: compute b = Pv.
     * Step 2: compute a such that La = b. Exists because L is invertible.
     * Step 3: compute b such that Ub = a. Check if such b really exists.
     * Step 4: return Qb;
     */

    VectorType a(dim());
    VectorType b(dim());

    // Step 1
    for( int i = 0; i < dim(); i++)
        b[ P()[i] ] = v[i];

    // Step 2
    for( int i = 0; i < dim(); i++)
    {
        a[i] = b[i];
        const T *rowptr = &( LU()( i, 0 ) );
        for( int j = 0; j < i; j++ )
        {
            a[i] -= a[j] * (*rowptr);
            rowptr += dim();
        }
    }

    // Step 3
    for( int i = dim() - 1; i >= 0; i--) // i must be signed!
    {
        const T *rowptr = &( LU()( i, i ) );
        const T *denomptr = rowptr;

        // if the current Eigenvalue of U is zero
        if( Util::isNegligible( *denomptr, m_biggestEigenValueU ) )
        {
            // if the current vector coord is also zero
            if( Util::isNegligible( a[i], m_biggestEigenValueU ) )
            {   // we can choose whatever value for b[i], so let's take
                // a nonzero value, so that b will be a nonzero vector
                b[i] = static_cast<T>(1);
            }
            else
            {
                // the equation has no solution (try solving 0*x = 1)
                return false;
            }
        }
        else // the current Eigenvalue of U is nonzero
        {
            b[i] = a[i];
            for( int j = i + 1; j <= dim() - 1; j++ )
            {
                rowptr += dim();
                b[i] -= b[j] * (*rowptr);
            }
            b[i] /= *denomptr;
        }
    }

    // Step 4
    for( int i = 0; i < dim(); i++)
        (*result)[ Q()[i] ] = b[i];
    return true;
}

template<typename T,
         typename MatrixType,
         typename VectorType,
         typename IntVecType>
bool LUDecompositionBase<T, MatrixType, VectorType, IntVecType>
    ::computeInverse( MatrixType * result ) const
{
    result->resize( dim() );
    if( ! isInvertible() ) return false;

    VectorType basis_vector(dim());
    VectorType antecedent(dim());
    for( int i = 0; i < dim(); i++ ) basis_vector(i) = static_cast<T>(0);

    for( int col = 0; col < dim(); col++ )
    {
        basis_vector(col) = static_cast<T>(1);
        computeSomeAntecedent( basis_vector, &antecedent );
        result->setColumn( col, antecedent );
        basis_vector(col) = static_cast<T>(0);
    }
    return true;
}

} // namespace Eigen

#endif // EIGEN_LUDECOMPOSITIONBASE_H
