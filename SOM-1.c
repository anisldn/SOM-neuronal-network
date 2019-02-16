#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "structure.h"



void read_file(data_iris *data_iris){
char buffer[NLIGNE];
int j = 0;
	FILE *fichier = fopen("test.txt", "r+");
    if (fichier == NULL){
        printf("no data file");
    }
	
	while (!feof(fichier)) {
        fgets(buffer, sizeof buffer, fichier);
		(data_iris[j]).vect = (double *) malloc(NVALEUR*sizeof(double));
		(data_iris[j]).nom = (char *) malloc(NCHAR*sizeof(char));
		char *split = strtok(buffer, ",");
       int i = 0;
		while ( i < NVALEUR) {
            (data_iris[j]).vect[i] = atof(split);
			split = strtok(NULL, ",");
			//printf("%f",(data_iris+j)->vect[i]);
			i++;
		}
//        (data_iris+j)->nom =(char*) split;
        strcpy(data_iris[j].nom,strtok((char*) split,"\n"));
        
//        printf("NOM     !  %s\n",(data_iris[j]).nom);
		j++;
	}
    
//    fclose(fichier);
}
void calcul_norme(data_iris *data_iris){
//    printf("NO     !  %s\n",(data_iris[1]).nom);
	int i,j=0;
	double s;
    for(j=0;j<NLIGNE;j++){
    	s=0;
        i=0;
	while ( i < NVALEUR) {
			s+=((data_iris[j]).vect[i])*((data_iris[j]).vect[i]);
			i++;
			
		}
        (data_iris[j]).norme=sqrt(s);
		//printf("norme %d: %f \n",j, (data_iris+j)->norme);


	}

	
}
void vecteur_normalise(data_iris *data_iris)
{
	for(int j=0;j<NLIGNE;j++)
	{
    for(int i=0;i<NVALEUR;i++){
	(data_iris+j)->vect[i]=(data_iris+j)->vect[i]/(data_iris+j)->norme;
//    printf("NORM  %s \n", (data_iris+j)->nom);
}
}
}
double* calcul_vectmoyen(data_iris *data_iris){
    
	double s=0.0;
	int j=0;
   double *calc_moy;

    calc_moy =  malloc(NVALEUR*sizeof(double));
    
   for(int i=0;i<NVALEUR;i++){
   	j=0;
   	s=0;
	while ( j< NLIGNE) {

			s+=((data_iris[j]).vect[i]);
        
        
			
			j++;
			
		}
		calc_moy[i]=s/NLIGNE;
		//calc_vectmax[i]=calc_moy[i]+MAX;
		//calc_vectmin[i]=calc_moy[i]-MIN;
		//printf("%f %f ",calc_vectmax[i], calc_vectmin[i]);

	}
		return calc_moy;
}


double rand_(double min, double max){
	return (rand()/(double)RAND_MAX)*(max-min)+min;
	}
grille* create_matrix(double* vectmoyen){
	srand(time(0));

    
	grille* matrix= malloc(sizeof(grille));
    matrix->neuron=malloc(NRCOLUMN*sizeof(neurone*));
    for(int k=0;k<NRCOLUMN;k++)
    {
    	matrix->neuron[k]=malloc(NRLIGNE*sizeof(neurone));
    }
    	
    for(int j=0;j<NRCOLUMN;j++)
    {
    	for(int i=0;i<NRLIGNE;i++)
    	{

    		matrix->neuron[j][i].vect=malloc(4*sizeof(double));
    		matrix->neuron[j][i].nom=malloc(20*sizeof(char));
    	}
    }       
       for(int j=0;j<NRCOLUMN;j++)
    {
    	for(int i=0;i<NRLIGNE;i++)
    	{
    		for(int h=0;h<NVALEUR;h++){

    			matrix->neuron[j][i].vect[h]=rand_(vectmoyen[h]-0.02,vectmoyen[h]+0.04);

    			printf("%f",matrix->neuron[j][i].vect[h]);
    		}
    		printf("   ||  %s %d %d \n",matrix->neuron[j][i].nom,j,i);
    		printf("\n");

    	}
    }
//    printf("Fini \n");
	return matrix;
}
	
double distance_euclid(double* vect1, double* vect2){
	double distance=0.0;
	for(int i=0;i<4;i++){
		distance+=pow(vect1[i]-vect2[i],2);
	}

	return sqrt(distance);
}
int* init_shuffle(){
	int *tab=malloc(NLIGNE*sizeof(int));
	for(int i=0;i<NLIGNE;i++){
        tab[i]=i;
	}
return tab;

}
void shuffle(int* table){
	int temp=0;
	int nb_tire=0;
	srand(time(0));
	for(int i=0;i<NLIGNE;i++)
	{
		nb_tire=rand()%NLIGNE;
		temp=table[i];
		table[i]=table[nb_tire];
		table[nb_tire]=temp;
	}
}
double getalpha(int total_iteration,int nb_iteration)
{
	return (0.7*(double)(1.0-(double)nb_iteration/(double)total_iteration));
}

int* bmu(data_iris *data_iris,grille* matrix,int ind_vect){
    int x = 0;
    int y = 0;
    double distance_tmp = 0;
    
    double distance = distance_euclid(matrix->neuron[0][0].vect,data_iris[ind_vect].vect);
    
    
    int* resultat = malloc(2*sizeof(int));
    for(int j=0;j<NRCOLUMN;j++)
    {
        for(int i=0;i<NRLIGNE;i++)
        {
//            printf("la... %d , %d \n",j,i);
            distance_tmp = distance_euclid(matrix->neuron[j][i].vect,data_iris[ind_vect].vect);
            
            if(distance_tmp < distance){
                distance = distance_tmp;
                x = j;
                y = i;
            }
            
        }
    }
    resultat[0] = x;
    resultat[1] = y;
//    printf("MAA \n");
    return resultat;
    
}
void voisin(grille* matrix,double* v,int *bmu,int rayon,double alpha){
    int x0,y0,x1,y1;
    x0 = bmu[0]-rayon;
    if(x0<0){
        x0 = 0;
    }
    x1 = bmu[0]+rayon;
    if(x1>=NRCOLUMN){
        x1 = NRCOLUMN;
    }
    y0 = bmu[1]-rayon;
    if(y0<0){
        y0 = 0;
    }
    y1 = bmu[1]+rayon;
    if(y1>=NRLIGNE){
        y1 = NRLIGNE;
    }
    for(int x=x0;x<x1;x++){
        for(int y=y0;y<y1;y++){
            for(int i=0;i<4;i++){
                
                matrix->neuron[x][y].vect[i] += alpha * (v[i] - matrix->neuron[x][y].vect[i]);
//                printf("%f,%d %d i=%d ",matrix->neuron[x][y].vect[i],x,y,i);
            }
//            printf("ALPHA %f",alpha);
//            printf("\n");
        }
    }
}
void affichage(grille* matrix){
    for(int j=0;j<NRCOLUMN;j++)
    {
        for(int i=0;i<NRLIGNE;i++)
        {
            if(strcmp("Iris-virginica",matrix->neuron[j][i].nom) == 0){
                printf("A ");
            }
            else if(strcmp("Iris-setosa",matrix->neuron[j][i].nom) == 0){
                printf("B ");
            }
            else if(strcmp("Iris-versicolor",matrix->neuron[j][i].nom) == 0){
                printf("C ");
            }
            else {
                printf(". ");
            }
        }
        printf("\n");
    }
}
void apprentissage(grille* matrix,data_iris *data_ir ){
    
    int *tab = init_shuffle();
    
    int rayon = 3;
    int nb_iterationTotal = 37500;
    int phase = nb_iterationTotal/4;
    double alpha = 0;
    
    
    for(int i=0;i<nb_iterationTotal;i++){
        shuffle(tab);
        
        if(i<=phase){
            if(i== phase*1/3 || i==phase*2/3){
                rayon--;
            }
            
            if(rayon<1) rayon = 1;
            alpha = getalpha(phase,i);
        }
        else {
            alpha = getalpha(nb_iterationTotal,i);
            rayon = 1;
        }
        
        for(int j=0;j<NLIGNE;j++){
            
            int* b = bmu(data_ir,matrix,tab[j]);
//            printf("akerkou... \n");
            matrix->neuron[b[0]][b[1]].nom = data_ir[tab[j]].nom;
//            strcpy(matrix->neuron[b[0]][b[1]].nom,data_ir[tab[j]].nom);
//            printf("%s matrice  \n",matrix->neuron[b[0]][b[1]].nom);
//            printf("%s \n",data_ir[tab[j]].nom);
            voisin(matrix,data_ir[tab[j]].vect,b,rayon,alpha);
            
        }
        
    }
    affichage(matrix);
}
int main(int argc, char *argv[])
{

srand(time(0));
data_iris *data_i;
double *calc_moy;
data_i = malloc(NLIGNE*sizeof(data_iris));
read_file(data_i);
calcul_norme(data_i);
vecteur_normalise(data_i);
 

calc_moy =  calcul_vectmoyen(data_i);
    
grille* matr_neurone=create_matrix(calc_moy);


apprentissage(matr_neurone,data_i );

printf("\n");
printf("Iris-virginica : A \n");
printf("Iris-setosa : B \n");
printf("Iris-versicolor : C \n");
}