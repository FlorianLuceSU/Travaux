#include <stddef.h> //pour size_t

//dans le terminal, depuis le dossier luce_florian-4m016-projet, entrer :
//              -gcc src/main_couronne.c src/polygone.c -o programme
//              -gcc src/main_suisse.c src/polygone.c -o programme
//              -gcc src/main_etoile.c src/polygone.c -o programme
//              -gcc src/main_spirale.c src/polygone.c -o programme

typedef struct Point{
    double x;
    double y;
}Point;


typedef struct Polygone{ 
    Point* tab; //chaque point est relié avec le point précédent et suivant du tableau. (Le dernier point étant relié avec le premier)
    size_t nombre_de_points;
    size_t taille;
}Polygone;



Polygone* convert_dat_to_tab(const char* filename);
//prend en entrer un fichier.dat et renvoie un tableau de points


void exec_gp_file(const char *filename);
//prend en entrer un fichier.gp et l'execute


const char* Creation_gp_file(const char *filename, Polygone* polygone);
//prend en entrer un fichier.dat, son tableau correspondant et renvoie un fichier.gp
//une fois exécuté, le graphique s'affichera
//cliquer sur le graphique créera un fichier.dat contenant les coordonnées le point selectionné

//je fais ça pour éviter de devoir recréer un fichier.gp manuellement pour chaque polygone


int Dans_polygone_sans_auto_intersection(Point C,Polygone *P);
//renvoie 1 si le point est dans le polygone non auto-intersecté (avec trous possibles), sinon 0


int Dans_polygone_auto_intersection(Point C, Polygone *P);
//renvoie 1 si le point est dans le polygone auto intersecté (sans trous), sinon 0


