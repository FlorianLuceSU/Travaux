#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "../include/polygone.h"


int main(){


    Polygone* polygone = convert_dat_to_tab("dat_gp_file/main_spirale.dat");
    if (!polygone || polygone->nombre_de_points==0){
        printf("Erreur polygone depuis creation_graphique\n");
        return 0;
    }

    const char* fichier_gp = Creation_gp_file("dat_gp_file/main_spirale.dat", polygone);
    if (!fichier_gp){
        printf("Erreur creation du fichier_gp\n");
        return 0;
    }

    exec_gp_file(fichier_gp);

    Polygone* point = convert_dat_to_tab("dat_gp_file/chosen_point.dat");
    if (!point){
        printf("Erreur creation point\n");
        return 0;
    }
    Point clicked_point = point->tab[0];

    printf("Le point (%lf,%f) est ",clicked_point.x, clicked_point.y);
    if (Dans_polygone_auto_intersection(clicked_point, polygone)){
        printf("dans le polygone\n");
    } else {
        printf("n'est pas le polygone\n");
    }
   
       

    free(polygone->tab);
    free(polygone);
    free(point->tab);
    free(point);

    return 0;
}