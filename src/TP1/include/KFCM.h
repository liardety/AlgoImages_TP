//
// Created by Patataa on 02/12/2015.
//

#ifndef ALGOIMAGE_KFCM_H
#define ALGOIMAGE_KFCM_H

#include <assert.h>
#include <bits/stl_bvector.h>
#include <cmath>

//TODO : define a correct value
static const c = 10;
template <typename T, typename  KernelFunctorType>
struct partitionCoeff{
    const T m_constA;
    const KernelFunctorType & f_kernel;

    partitionCoeff(const T & fuzziness, const KernelFunctorType & kernel) :
            m_constA(-1 / (fuzziness - 1)),
            f_kernel(kernel)
    {
        assert( fuzziness > 1);
        assert( !kernel );
    }

    T intermediaryFunction(T xK, T nuI) {
        return std::pow((1 - f_kernel(xK, nuI)), m_constA);
    }
    //TODO : Memorize Kernel Operation with pre computed matrix
    template <typename pixelType>
    T operator()(const pixelType & xK, const std::vector<pixelType> & nu, unsigned int i) {
        T numerator = intermediaryFunction(xK, nu[i]);
        //TODO: fix the denominator
        T denominator = 0;
        for(unsigned int j = 1; i <= c; ++j) {
            denominator += intermediaryFunction(xK, nu[j]);
        }

        return  numerator / denominator;
    }
};

template <typename T>
struct kernelCenter{

    template <typename ClustersType, typename pixelType>
    T operator()(const pixelType & xK, const std::vector<pixelType> & nu, unsigned int i) {
        T numerator = 0;


    }
};

template <typename T>
struct KFCM {
    const unsigned int m_clusterNum;
    const T m_fuzziness;
    const T m_epsilon;





};

#endif //ALGOIMAGE_KFCM_H
