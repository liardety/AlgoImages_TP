//
// Created by Patataa on 30/11/2015.
//

#ifndef ALGOIMAGE_DISTANCE_H
#define ALGOIMAGE_DISTANCE_H

#include <math.h>
#include "../include/CImg.h"

template <typename T>
struct Distance {
    Distance(){}
    ~Distance(){}

    template <typename T1, typename T2,typename ...Args>
    T operator()(const T1 & x, const T2 & y, Args && ... others)const;
};


#endif //ALGOIMAGE_DISTANCE_H
