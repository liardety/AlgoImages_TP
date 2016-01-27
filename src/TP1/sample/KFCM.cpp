//
// Created by Patataa on 25/01/2016.
//

#include <string>
#include <iostream>
#include <vector>
#include <Distance.h>

#include "CImg.h"
#include "KFCM.h"
#include "Kernel.h"
#include "EuclidianDistance.h"

using namespace cimg_library;

static const int nbCluster = 2;
static const unsigned int tMax = 10;
static const double epsilon = 0.01;
static const double fuzzyCoef = 2;

static const double sigma = 1;


int main(int argc, char **argv) {

    // Getting the sample
    const std::string fileLocation("");
    const std::string filename("./img/firstSample.bmp");

    const CImg<> origin((fileLocation + filename).c_str());

    // Clusters initializing

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

    auto euclidianDistance = make_EuclidianDistance<float>(2);
    auto kernel = make_kernel(euclidianDistance,sigma);

    for(unsigned int t = 0 ; !end && t < tMax; ++t) {  // iteration principale
        
        // mise à jour des nu
        for(unsigned int i = 0; i < nbCluster; ++i) { // iteration sur les clusters
            float* ptrCluster = clusters(i).data();
            const float* ptrOrigin = origin.data();
            float denomi = 0;
            float nume = 0;
            for( unsigned int k = 0; k < tailleBuffer; ++k ){  // iteration sur les pixels

                float computedKernel = std::pow(*(ptrCluster + k), fuzzyCoef) * kernel(*(ptrOrigin + k) + 0.001, nu[i]);
                denomi += computedKernel;
                nume += computedKernel * *(ptrOrigin + k);
            }
            assert(denomi != 0);
            nu[i] = nume / denomi;
        }
        


        // mise à jour des clusters 
        const float* ptrOrigin = origin.data();
        for( unsigned int k = 0; k < tailleBuffer; ++k){  // iteration sur les pixels 
          float denomi = 0;
          for(unsigned int j = 0; j < nbCluster; ++j){  // calcul de dénominateur nécessitant une sommation pour toutes les classes
            denomi += std::pow(1 - kernel(*(ptrOrigin + k), nu[j]) + 0.0001, -1/(fuzzyCoef-1));
          }
          assert(denomi != 0);
          for(unsigned int i = 0; i < nbCluster; ++i){  // calcul pour chaque classe
            *(clusters(i).data() + k) = std::pow(1 - kernel(*(ptrOrigin + k), nu[i]),-1/(fuzzyCoef-1) ) / denomi;
          }
        }
        
        //test d'arrêt

        float maxi = 0;
        for(unsigned int i = 0; i < nbCluster; ++i){  // iteration sur les classes
            for(unsigned int k = 0; k < tailleBuffer; ++k){  // iteration sur les pixels
                maxi = std::max(maxi, float(std::abs((clusters(i).data() + k) - (clustersSave(i).data() + k))));
            }
        }
        end = (maxi < epsilon);
    }


    while (!disp.is_closed())
    {

        disp.wait();
    }

    CImgDisplay disp2(clusters(0),"Input Image");
    while (!disp2.is_closed())
    {
        disp2.wait();
    }

    CImgDisplay disp3(clusters(1),"Input Image");
    while (!disp3.is_closed())
    {
        disp3.wait();
    }
    std::cout << "End"<<std::endl;

    return 1;
}
