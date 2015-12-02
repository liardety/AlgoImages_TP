//
// Created by Patataa on 02/12/2015.
//

#ifndef ALGOIMAGE_KFCM_H
#define ALGOIMAGE_KFCM_H

#include <assert.h>
#include <bits/stl_bvector.h>
#include <cmath>

template <typename T>
struct partitionCoeff{
    const T m_constA;

    partitionCoeff(const T & fuzziness) :
            m_constA(-1 / (fuzziness - 1)){
        assert( fuzziness > 1);
    }

    //TODO : Memorize Kernel Operation with pre computed matrix
    template <typename pixelType, typename Kernel>
    T operator()(const pixelType & xK, const std::vector<pixelType> & nu, const Kernel & kernelFunc, unsigned int i) {
        T numerator = std::pow((1 - kernelFunc(xK, nu[i])), m_constA);
        //TODO: fix the denomitator
    }
};

template <typename T>
struct KFCM {
    const unsigned int m_clusterNum;
    const T m_fuzziness;




};

#endif //ALGOIMAGE_KFCM_H
