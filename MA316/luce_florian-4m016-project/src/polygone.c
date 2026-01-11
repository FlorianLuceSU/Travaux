#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "../include/polygone.h"

Polygone* convert_dat_to_tab(const char* filename){

    size_t longueur = strlen(filename);

    if (longueur<4){
        printf("Le nom du fichier.dat n'est pas conforme\n");
        return NULL;
    }

    if (strcmp(filename+longueur-4,".dat")!=0){
        printf("Ce n'est pas un fichier.dat\n");
        return NULL;
    }

    //initialisation du tableau de points (ne pas oublier le free)

    Polygone* Tableau_Points = malloc(sizeof(Polygone));
    Tableau_Points->tab = malloc(sizeof(Point)*10);
    Tableau_Points->nombre_de_points = 0;
    Tableau_Points->taille = 10;

    //on met les données du fichier.dat dans le tableau

    FILE *f = fopen(filename,"r");
    if (!f){
        printf("fichier.dat not found pour la conversion en tableau\n");
        free(Tableau_Points->tab);
        free(Tableau_Points);
        return NULL;
    } 

    char buffer[256];
    double coordX, coordY;


    while ((fgets(buffer, 256, f))!=NULL){ 

        if (sscanf(buffer, "%lf %lf", &coordX, &coordY)!=2){
            printf("Ne peut pas lire deux coordonnees dans le fichier.dat\n");
            free(Tableau_Points->tab);
            free(Tableau_Points);
            return NULL;
        }

        //je me prépare à rajouter un point, on controle alors la taille du tableau
        
        if (Tableau_Points->nombre_de_points>=Tableau_Points->taille){ 
            Point* new_temp = realloc(Tableau_Points->tab, Tableau_Points->taille * sizeof(Point) * 2);

            if(!new_temp){
                free(Tableau_Points->tab);
                free(Tableau_Points);
                fclose(f);
                printf("Erreur realloc dans 'convert_dat_to_tab'\n");
                return NULL;
            }
            Tableau_Points->tab = new_temp;
            Tableau_Points->taille*=2;
        }
        
        Tableau_Points->tab[Tableau_Points->nombre_de_points].x = coordX;
        Tableau_Points->tab[Tableau_Points->nombre_de_points].y = coordY;
        Tableau_Points->nombre_de_points++;
    }

    fclose(f);
    return Tableau_Points;
}

void exec_gp_file(const char *filename){
    char instruction[768]="";
    snprintf(instruction, sizeof(instruction), "gnuplot %s",filename);
    system(instruction);
    //snprintf permet de concaténer le troisième argument au buffer en s'assurant que la taille du résultat ne dépasse pas la mémoire allouée pour le buffer

}

const char* Creation_gp_file(const char *filename, Polygone* polygone){

    //le fichier est il bien un fichier.dat ?

    size_t longueur = strlen(filename);

    if (longueur<4){
        printf("Le nom du fichier.dat n'est pas conforme\n");
        return NULL;
    }

    if (strcmp(filename+longueur-4,".dat")!=0){
        printf("Ce n'est pas un fichier.dat\n");
        return NULL;
    }

    //création du nom du fichier en remplassant dat par gp

    static char nouveau_fichier[256];

    strncpy(nouveau_fichier, filename, longueur-3);
    
    nouveau_fichier[longueur-3]='g';
    nouveau_fichier[longueur-2]='p';
    nouveau_fichier[longueur-1]='\0';

    //on écrit dans le fichier.gp

    FILE *f = fopen(nouveau_fichier,"w");
    if (!f) return NULL;

    fprintf(f, "set title \"Please, click to select a point\"\n" );
    fprintf(f, "set xlabel \"x\"\n" );
    fprintf(f, "set ylabel \"y\"\n" );
    fprintf(f, "set grid\n" );

    //je prends les plus petites et plus grandes coordonnées pour bien encadrer le graphique

    double x_min = polygone->tab[0].x , x_max = x_min;
    double y_min = polygone->tab[0].y, y_max = y_min;

    for(int i=1; i < polygone->nombre_de_points; i++){
        if ((polygone->tab[i].x)>x_max){
            x_max = polygone->tab[i].x;
        } 
        if ((polygone->tab[i].x)<x_min){
            x_min = polygone->tab[i].x;
        }
        if ((polygone->tab[i].y)>y_max){
            y_max = (polygone->tab[i].y);
        }
        if ((polygone->tab[i].y)<y_min){
            y_min = (polygone->tab[i].y);
        }
    }
    
    double marge_x = (x_max - x_min)*0.1; //rajoute une marge équitable de part et d'autre du polygone
    double marge_y = (y_max - y_min)*0.1;

    fprintf(f, "set xrange [%lf:%lf]\n",  x_min - marge_x ,  x_max + marge_x );
    fprintf(f, "set yrange [%lf:%lf]\n",  y_min - marge_y  ,  y_max + marge_y );
    fprintf(f,"set style line 1 lt 1\n"); //ligne continue
    fprintf(f,"set mouse\n"); //active l'intéraction de la souris avec le graphique
    fprintf(f, "bind \"Button1\" \"set print 'dat_gp_file/chosen_point.dat'; print MOUSE_X, MOUSE_Y; unset print; exit gnuplot\"\n");

    //bind [<key-sequence>] ["<gnuplot commands>"]
    //les éléments dans [] signifie optionnel
    //les éléments dans " " signifie qu'il faut mettre des guillemets

    //Button1 = clic gauche
    //Button2 = clic droit
    //Button3 = clic molette

    //set print = print vers
    fprintf(f,"plot \"%s\" using 1:2 with lines lw 2\n",filename);
    fprintf(f,"pause mouse close\n");

    //le graphique reste ouvert jusqu'à ce qu'on clique

    fclose(f);

    return nouveau_fichier;
}

int Dans_polygone_sans_auto_intersection(Point C,Polygone *P){

    if (P->nombre_de_points <3){
        return 0;
    }


    Point A;
    Point B;
    double x_inter;
    size_t nb_intersections_a_droite = 0;


    for(size_t i = 0; i < P->nombre_de_points ; i ++){

        A = P->tab[i];
        B = P->tab[(i+1)%(P->nombre_de_points)];

        //je ne prends que les couples de points dont le segment 
        //fait une intersection la fonction f constante à C.y
        //et je compte les intersections qui se font à droite de C.x

        if ((A.y>C.y)!=(B.y>C.y)){

            x_inter = A.x + (C.y - A.y)*(B.x - A.x)/(B.y-A.y);

            if (x_inter>C.x){
                nb_intersections_a_droite++;
            }
        }

        //Le point testé appartient il à la frontière du polygone ?

        double e = 0.00000000000001;
        double t=2; //je veux juste pas que ça entre pas dans "if (t>=0 && t<=1)" sans raison

        //Colinéarité
        double test_colineaire = (C.x - A.x)*(B.y - A.y)-(C.y - A.y)*(B.x - A.x);

        if (test_colineaire<e && test_colineaire>-e){ 
            //le point C est il entre A et B ?
            
            if ((B.x-A.x)>e || (B.x-A.x)<-e ){ //car on sait que A!=B
                t = (C.x - A.x)/(B.x - A.x);
            } else if ((B.y-A.y)>e || (B.y-A.y)<-e) {
                t = (C.y - A.y)/(B.y - A.y);
            }
            if (t>=0 && t<=1) return 1; //si t==1 alors C==B, si t==0 alors C==A
        }
        //j'ai ignoré le cas A==B, car il n'y a aucun segment nul dans les polygones
    }

    return (nb_intersections_a_droite%2);
}

int Dans_polygone_auto_intersection(Point C, Polygone *P) {

    int tour = 0;
    Point A;
    Point B;
    double x_CA, y_CA;
    double x_CB, y_CB;
    double produit_vectoriel_CA_CB;

    for (size_t i = 0; i < P->nombre_de_points; i++) {

        A = P->tab[i];
        B = P->tab[(i + 1) % P->nombre_de_points];


        x_CA = A.x-C.x; x_CB = B.x-C.x;
        y_CA = A.y-C.y; y_CB = B.y-C.y;

        produit_vectoriel_CA_CB = x_CA*y_CB - y_CA*x_CB; //signe du sinus

        
        //Soit f la droite horizontale passant par C
        //l'arête [AB] coupe-t-elle f ?
        if (A.y>C.y && B.y<=C.y) {

            //[AB] coupe-t-elle f dans le sens anti horaire ?
            if (produit_vectoriel_CA_CB > 0) {

                //Si oui, on a fait un tour
                tour++; 
                
            }
        } else if (A.y<=C.y && B.y>C.y) {

            //[AB] coupe-t-elle f dans le sens anti horaire ?
            if (produit_vectoriel_CA_CB < 0){ 
                    
                //Si oui, on a fait un tour
                tour--;
            }
            
            
        //Le point testé appartient il à la frontière du polygone ?

        double e = 0.00000000000001;
        double t=2; //je veux juste pas que ça entre pas dans "if (t>=0 && t<=1)" sans raison

        //Colinéarité
        double test_colineaire = (C.x - A.x)*(B.y - A.y)-(C.y - A.y)*(B.x - A.x);

        if (test_colineaire<e && test_colineaire>-e){ 
            //le point C est il entre A et B ?
            
            if ((B.x-A.x)>e || (B.x-A.x)<-e ){ //car on sait que A!=B
                t = (C.x - A.x)/(B.x - A.x);
            } else if ((B.y-A.y)>e || (B.y-A.y)<-e) {
                t = (C.y - A.y)/(B.y - A.y);
            }
            if (t>=0 && t<=1) return 1; //si t==1 alors C==B, si t==0 alors C==A
        }
        //j'ai ignoré le cas A==B, car il n'y a aucun segment nul dans les polygones
            
        }
    }

    return tour!=0; 
}


