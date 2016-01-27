//
// Created by Patataa on 30/11/2015.
//

#ifndef ALGOIMAGE_KERNEL_H
#define ALGOIMAGE_KERNEL_H

#include <cmath>
#include <functional>

//Todo : Tests du Kernel
// -> Kernel d'un pixel et de lui même = 1
template<typename T, typename  Functor>
struct Kernel{
    const T m_sigmaSquare;
    Functor m_distance;


    Kernel(Functor && distance, const T & sigma) :
            m_distance(std::forward<Functor>(distance)),
            m_sigmaSquare(sigma * sigma) { }

    template<typename Object>
    T operator()(Object && a, Object && b)const {
        return std::exp(- m_distance(std::forward<Object>(a),std::forward<Object>(b)) / (m_sigmaSquare) );
    }
    
    // TODO : refaire l'implémentation en tenant compte du fait que la fonction de CImg get_distance retourne une CImg.
    

};

template <typename T, typename Functor>
Kernel<T,Functor> make_kernel(Functor && distance, const T & sigma) {
    return { std::forward<Functor>(distance), sigma};
};
#endif //ALGOIMAGE_KERNEL_H
