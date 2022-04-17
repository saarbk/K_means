#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
int const EPSILON = 0.001;

int main(int, char **);
void k_means(int, const char*,const char*, int);
void opt(int, double **, double**, double **, int*, int, int ,int);
double* update_centeroid(double *, int, int);
int find_min(double* , double** , int, int);
double calculate_dist(double*, double*, int);
void sum_vectors(double*, double*, int);
void init_clusters(double **,int , int , int*);
int decimal(char*);

int main(int argc, char **argv)
{
    int K, max_iter;
    char* input_file, *output_file;
    (void)argc;
    max_iter = 200;
    K = atoi(argv[1]);
    if(decimal(argv[2]) == 1)
    {
        max_iter = atoi(argv[2]);
        input_file = argv[3];
        output_file = argv[4];
    }
    else
    {
        input_file = argv[2];
        output_file = argv[3];
    }
    k_means(K, input_file, output_file, max_iter);
    return 0;
}

void k_means(int K, const char* dataPoints_file, const char* centeroids_file, int max_iter)
{
    int i = 0, j = 0, d = 1, N = 1, offset = 0;
    char c;
    double num;
    FILE *ifp_datapoints = NULL;
    FILE *ifp_centeroids = NULL;
    double **datapoints, **centeroids, **clusters;
    int *cluster_size;

    ifp_datapoints = fopen(dataPoints_file, "r");
    ifp_centeroids = fopen(centeroids_file, "w");
    centeroids = (double**) calloc(K, sizeof(double*));
    clusters = (double**) calloc(K, sizeof(double*));
    cluster_size = (int*) calloc(K, sizeof(int));

    printf("%d %d", max_iter, K);
    if(max_iter < 1 || K < 1)
    {
        printf("Error! 1");
        exit(1);
    }
    if(ifp_datapoints == NULL)
   {
      printf("Error! 2");
      exit(1);
   }

   while((c = getc(ifp_datapoints)) != '\n')
    {
        if(c == ',')
            d++;
        offset++;
    }
    fseek(ifp_datapoints, 0, 0);
    offset = 0;

    while(fscanf(ifp_datapoints, "%lf", &num) == 1)
    {
        c = getc(ifp_datapoints);
        if(c == '\n')
            N++;
        offset++;
    }
    fseek(ifp_datapoints,0 , 0);

    datapoints = (double**) calloc(N, sizeof(double*));
    for (i = 0; i < K; i++)
    {
        centeroids[i] = (double*)calloc(d, sizeof(double*));
        clusters[i] = (double*)calloc(d, sizeof(double*));
    }
    for(i = 0; i < N; i++)
    {
        datapoints[i] = (double*)calloc(d, sizeof(double*));
    }
    i = 0;

    while(fscanf(ifp_datapoints, "%lf", &num) == 1)
    {
        datapoints[i][j] = num;
        c = getc(ifp_datapoints);
        if(c == '\n')
        {
            i++;
            j = 0;
        }
        else
            j++;
    }

    for (i = 0; i < K; i++)
    {
        for(j = 0; j < d; j++)
        {
            centeroids[i][j] = datapoints[i][j];
            clusters[i][j] = centeroids[i][j];
        }
        cluster_size[i] = 1;
    }
    opt(K, datapoints, centeroids, clusters, cluster_size, max_iter, N, d);

    for(i = 0; i < K; i++)
    {
        for(j = 0; j < d; j++)
        {
            fprintf(ifp_centeroids, "%.4f", centeroids[i][j]);
            fprintf(ifp_centeroids, "%s", ", ");
        }
        fprintf(ifp_centeroids, "%c", '\n');
    }

    fclose(ifp_datapoints);
    fclose(ifp_centeroids);
    free(datapoints);
    free(clusters);
    free(centeroids);
}

void opt(int K, double** dataPoints, double** centeroids, double** clusters, int* clusters_size, int max_iter, int N, int d)
{
    int reps = 0, done = 0, i, min_cluster, j;
    double* new_centeroid;
    new_centeroid = (double*) calloc(d, sizeof(double));

    while(done == 0 && reps < max_iter)
    {
        init_clusters(clusters, K, d, clusters_size);
        done = 1;
        for(i = 0; i < N; i++)
        {
            min_cluster = find_min(dataPoints[i], centeroids, K, d);
            sum_vectors(clusters[min_cluster], dataPoints[i], d);
            clusters_size[min_cluster]++;
        }
        for (i = 0; i < K; i++)
        {
            new_centeroid = update_centeroid(clusters[i], clusters_size[i], d);
            if(calculate_dist(new_centeroid, centeroids[i], d) >= EPSILON)
                done = 0;
            for(j = 0; j < d; j++)
            {
                centeroids[i][j] = new_centeroid[j];
            }
        }
        reps++;
    }
    free(new_centeroid);
}
double* update_centeroid(double *cluster, int cluster_size, int d)
{
    int i;
    for (i = 0; i < d; i++)
    {
        cluster[i] = cluster[i] / cluster_size;
    }
    return cluster;
}

int find_min(double* dataPoint, double** centeroids, int K, int d)
{
    int index_min = 0, i;
    double min, dist;
    min = calculate_dist(centeroids[0], dataPoint, d);
    for(i = 1; i < K; i++)
    {
        dist = calculate_dist(centeroids[i], dataPoint, d);
        if(dist < min)
        {
            index_min = i;
            min = dist;
        }
    }
    return index_min;
}
double calculate_dist(double* point1, double* point2, int d)
{
    int i;
    double sum;
    sum = 0.0;
    for(i = 0; i < d; i++)
    {
        sum += pow(point1[i] - point2[i] , 2);
    }
    sum = pow(sum, 0.5);
    return sum;
}

void sum_vectors(double* vect1, double* vect2, int d)
{
    int i;
    for(i = 0; i < d; i++)
    {
        vect1[i] += vect2[i];
    }
}

void init_clusters(double ** clusters,int K, int d, int* sizes)
{
    int i,j;
    for(i = 0; i < K; i++)
    {
        sizes[i] = 0;
        for(j = 0; j < d; j++)
        {
            clusters[i][j] = 0.0;
        }
    }
}

int decimal(char* c)
{
    int i = 0;
    if(c[0] == '-')
        i++;
    for(; c[i] != 0; i++)
    {
        if(c[i] > '9' || c[i] < '0')
            return 0;
    }
    return 1;
}
