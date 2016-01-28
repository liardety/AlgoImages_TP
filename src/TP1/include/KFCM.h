//
// Created by Patataa on 02/12/2015.
//

#ifndef ALGOIMAGE_KFCM_H
#define ALGOIMAGE_KFCM_H

#include <assert.h>
#include <bits/stl_bvector.h>
#include <cmath>
#include <numeric>

template <typename F, typename  KernelFunctorType>
struct KFCM {
    const F m_constA;
    const KernelFunctorType & f_kernel;


    KFCM(KernelFunctorType && kernel, const F & fuzziness = 0.f) :
            m_constA(-1 / (fuzziness - 1)),
            f_kernel(kernel)
    {
        assert( fuzziness > 1);
    }

    template <typename T>
    T intermediaryFunction(T xK, T nuI) {
        return T(std::pow((1 - f_kernel(xK, nuI)), m_constA));
    }
    //TODO : Memorize Kernel Operation with pre computed matrix


    template <typename T>
    std::vector<T> updateClusters(const T & xK, const std::vector<T> & nu) {
        T denominator = 0;
        unsigned int sizeNu = nu.size();

        std::vector<T> functionMemorized(sizeNu);

        for(unsigned int i = 0; i < sizeNu; ++i) {
            //Ca être lent
            functionMemorized[i] = intermediaryFunction(xK, nu[i]);
            denominator += functionMemorized[i];
        }
        for(unsigned int i = 0; i < sizeNu; ++i) {
            functionMemorized[i] /=  denominator;
        }
        return functionMemorized;
    }

    template <typename T>
    T updateKernel(const T & nu) {

    }

    template <typename T>
    T operator()(const T & xK, const std::vector<T> & nu, unsigned int i) {
        T numerator = intermediaryFunction(xK, nu[i]);
        //TODO: fix the denominator
        T denominator = 0;
        for(unsigned int j = 1; i <= nu.size(); ++j) {
            denominator += intermediaryFunction(xK, nu[j]);
        }

        return  numerator / denominator;
    }
};

template <typename F, typename  KernelFunctorType>
KFCM<F, KernelFunctorType> make_KFCM(KernelFunctorType && kernel, F && fuzziness) {
    return {std::forward<KernelFunctorType>(kernel), std::forward<F>(fuzziness)};
};

#endif //ALGOIMAGE_KFCM_H
