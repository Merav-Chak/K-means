#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <float.h>

size_t rows_counter(FILE *ifp);
int vector_dim(FILE *ifp);
double distance(double *p1, double *p2, int d);

int main(int argc, char *argv[]) {
    int i,j,r,d,convergence,index,t=0;
    int k = strtol(argv[1], NULL, 10);
    int max_iter = 200;
    char *input, *output;
    double **input_vectors,**current_center,**previous_center;
    double try_min,min;

    size_t init_size;

    printf("%d", argc);
    if (argc == 5)
    {
        max_iter = strtol(argv[2], NULL, 10);
        *input = argv[3];
        *output = argv[4];
    }
    else
    {
        *input = argv[2];
        *output = argv[3];
    }

    FILE *ifp = NULL;
    FILE *ofp = NULL;


    ifp = fopen(input, "r");

    if (ifp == NULL){
        printf("An Error Has Occurred3");
        return 1;
    }

    d = vector_dim(ifp);
    init_size = rows_counter(ifp);
    if ((int)init_size < k || max_iter <= 0 || k <= 0) {
        printf("Invalid Input!");
        return 1;
    }

    /*initiate input vectors matrix*/
    input_vectors = malloc(sizeof(double*)*init_size);
    for (i = 0; i < (int)init_size; i++) {
        input_vectors[i] = malloc(sizeof(double)*d);
    }
    /*double input_vectors[init_size][d];
    initiate input vectors - insert all vectors from the input file into a matrix*/
    for (i=0; i<(int)init_size; i++)
    {
        for (j=0; j<d; j++)
        {
                fscanf(ifp, "%lf", &(input_vectors[i][j]));
                fgetc(ifp);
        }
    }
    fclose(ifp);

    /*previous centeroids initiated as the first k vectors in the input file:*/

    /*initiate matrix*/
    previous_center = malloc(sizeof(double*)*k);
    for(i = 0; i < k; i++)
    {
        previous_center[i] = malloc(sizeof(double)*d);
    }

    /*fetch first k vectors*/
    for(i = 0; i < k; i++)
    {
        for(j = 0; j < d; j++)
        {
            previous_center[i][j] = input_vectors[i][j];
        }
    }
    current_center = malloc(sizeof(double*)*k);
    for(i = 0; i < k; i++)
        current_center[i] = malloc(sizeof(double)*(d+1));

    while(t < max_iter)
    {
        /*initiate center sum matrix to sum vectors, first cell of each row is a counter*/
        double **center_sum;
        center_sum = calloc(sizeof(double*)*k, sizeof(double*));
        for(i = 0; i < k; i++)
        {
            center_sum[i] = calloc(sizeof(double)*(d+1),sizeof(double));
        }
        /* loop to find which centeroid fits which vector*/
        for(i = 0; i < (int)init_size; i++)
        {
            try_min = 0;
            min = DBL_MAX;
            index = 0;
            /*run on all centeroids for vector i*/
            for (j = 0; j < k; j++)
            {
                try_min = distance(input_vectors[i], previous_center[j], d);
                if (try_min < min)
                {
                    min = try_min;
                    index = j;
                }
            }
            /*sum the vector to the closest centeroid*/
            center_sum[index][0] += 1;
            for (r = 1; r < d + 1; r++)
            {
                center_sum[index][r] += input_vectors[i][r - 1];
            }
        }
        /*initiate current centeroids*/
        convergence = 1;
        for(i = 0; i < k; i++)
        {
            if (center_sum[i][0] == 0) {
                printf("An Error Has Occurred2");
                return 1;
            }
            for(j = 1; j < d+1; j++)
            {
                current_center[i][j-1] = (center_sum[i][j])/center_sum[i][0];
            }
            if(distance(current_center[i],previous_center[i],d) >= 0.001)
                convergence = 0;
        }

        if(convergence == 1)
            break;
        else
        {
            for(i = 0; i < k; i++)
            {
                for(j = 0; j < d; j++)
                {
                    previous_center[i][j] = current_center[i][j];
                }
            }
        }
        t ++;
    }
    ofp = fopen(output, "w");

    if (ofp == NULL){
        printf("An Error Has Occurred1");
        return 1;
    }
    
    for (i=0; i<k; i++){
        for (j=0; j<d; j++) {
            if (j == d - 1) 
            {
                fprintf(ofp, "%.4f\n",  current_center[i][j]);
            }
            else
                fprintf(ofp, "%.4f,", current_center[i][j]);
        }
    }
    fclose(ofp);
    return 0;
}



double distance(double *p1, double *p2, int d)
{   int i;
    double result = 0;
    for (i=0; i<d; i++)
    {
        result += pow((*(p1+i)-*(p2+i)), 2);
    }
    return pow(result, 0.5);
}

size_t rows_counter(FILE *ifp) {
    size_t rows = 0;
    char c;

    while (!feof(ifp)) {
        c = fgetc(ifp);
        if (c == '\n')
            rows++;
    }
    rewind(ifp);
    return rows;
}

int vector_dim(FILE *ifp){
    int d = 1;
    char c;

    do{
        c = fgetc(ifp);
        if (c == ',')
            d++;
    }

    while (c != '\n');
    rewind(ifp);
    return d;

}