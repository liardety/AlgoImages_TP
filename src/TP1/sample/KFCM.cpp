//
// Created by Patataa on 25/01/2016.
//

#include <string>
#include <iostream>
#include "../include/CImg.h"

#include "KFCM.h"
#include "Distance.h"
#include "Kernel.h"

using namespace cimg_library;

static const int nbCluster = 2;
static const unsigned int tMax = 10;
static const double epsilon = 0.01;
static const double fuzzyCoef = 2;

int main(int argc, char **argv) {
    const std::string fileLocation("");
    const std::string filename("./img/firstSample.bmp");

    const CImg<> origin((fileLocation + filename).c_str());
    CImgList<> clusters(nbCluster, origin.width(), origin.height(), 1,1, 1.0 / float(nbCluster) );
    CImgList<> clustersSave(clusters); // sauvegarde de l'itération précédente pour le critaire d'arrêt.
    std::vector<float> nu(nbCluster);

    // initialisation des nu
    for(unsigned int i = 0; i < nbCluster; ++i){
        nu.push_back((i / nbCluster) * 255.0);
    }



    std::cout << epsilon << std::endl;
    CImgDisplay disp(origin,"Input Image");
    

    unsigned int tailleBuffer = origin.width() * origin.height();
    bool end = false;
    for(unsigned int t = 0 ; !end && t < tMax; ++t) {  // iteration principale
        
        // mise à jour des nu
        for(unsigned int i = 0; i < nbCluster; ++i) { // iteration sur les clusters
            float* ptrCluster = clusters[i].data();
            const float* ptrOrigin = origin.data();
            float denomi = 0;
            float nume = 0;
            for( unsigned int k = 0; k < tailleBuffer; ++k ){  // iteration sur les pixels

                float computedKernel = std::pow(*(ptrCluster + k), fuzzyCoef) * Kernel<float, EuclidianDistance<float>>(*(ptrOrigin + k), nu[i]);
                denomi += computedKernel;
                nume += computedKernel * (ptrOrigin + k);
            }
            assert(denomi == 0);
            nu[i] = nume / denomi;
        }
        

        // mise à jour des clusters 
        const float* ptrOrigin = Origin.data();
        for( unsigned int k = 0; k < tailleBuffer; ++k){  // iteration sur les pixels 
          float denomi = 0;
          for(unsigned int j = 0; j < nbCluster; ++j){  // calcul de dénominateur nécessitant une sommation pour toutes les classes
            denomi += std::pow(1 - Kernel<float, EuclidianDistance<float>>(*(ptrOrigin + k), nu[j]));
          }
          assert(denomi == 0);
          for(unsigned int i = 0; i < nbCluster; ++i){  // calcul pour chaque classe
            *(clusters[i].data() + k) = std::pow(1 - Kernel<float, EuclidianDistance<float>>(*(ptrOrigin + k), nu[i])) / denomi;
          }
        }
        
        //test d'arrêt

        float maxi = 0;
        for(unsigned int i = 0; i < nbCluster; ++i){  // iteration sur les classes
            for(unsigned int k = 0; k < tailleBuffer; ++k){  // iteration sur les pixels
                maxi = std::max(max, std::abs((clusters[i].data() + k) - (clustersSave[i].data() + k)));
            }
        }
        end = (maxi < epsilon);
    }


    while (!disp.is_closed())
    {

        disp.wait();
    }
    std::cout << "End"<<std::endl;

    return 1;
}
