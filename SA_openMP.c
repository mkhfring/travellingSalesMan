#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define T_start 5000.0      //starting temp
#define T_end (1e-8)        //end temp
#define q 0.98              //coef
#define L 1024              //max it
#define N 12                //number of cities
int city_result[N];         //solution
double city[N][2] = {{48.431245619848255, -123.36537967024275},{49.3021919632101, -123.12226115279474},{53.62208783069036, -113.40039303223175},{52.19013778041407, -106.55403987156726},{50.46323151354986, -104.61637921649017},{66.24852413463175, -128.6448217733023},{49.88727582165454, -97.22962773518263}
,{43.680419790975435, -79.39774675313382},{45.30760994851931, -75.76618887317751},{51.32530434372933, -73.22678830471655},{64.34788639923939, -95.9939920711135},{69.368278920504, -124.12344886103753}};
//double city[N][2] = {{9932, 4439}, {10109, 4351}, {11552, 3472}, {10302, 3290}, {8776, 3333}, {7040, 4867}, {9252, 4278}, {9395, 4539}, {11101, 2540}, {9825, 5087}, {10047, 4879}, {10227, 4648}, {100027, 4229}, {9878, 4211}, {9087, 4065}, {10438, 4075}, {10382, 3865}, {11196, 3563}, {11075, 3543}, {11544, 3365}, {11915, 2900}, {11305, 3189}, {11073, 3137}, {10950, 3394}, {11576, 2575}, {12239, 2785}, {11529, 2226}, {9328, 4006}, {10012, 3811}, {9952, 3410}, {10612, 2954}, {10349, 2784}, {11747, 2469}, {11673, 2461}};
double haversine(double *city1, double *city2); //distance between two city
double path(int city_result[N]);               //calculate distance of the solution
void init();                                   //init solution
void creat();                                  //creat new solution

int main()
{
    time_t start, end;
    double time_sum;
    start = clock();
    int i;
	int count = 0;
    double T;
    srand(time(NULL));
    omp_set_num_threads(8);
    init();
    T = T_start;
    while (T > T_end)
    {
		#pragma omp parallel
    	{
    		int result[N];
    		int j;
    		int result_copy[N];
    		double p1;
    		double p2;
    		memcpy(result, city_result, N * sizeof(int));
    		for(j = 0; j < L/8; j++)
    		{
    			memcpy(result_copy, result, N * sizeof(int));
    			creat(result);
				p1 = path(result_copy); //old
				p2 = path(result); //new
				double d = p2 - p1;
				if(d >= 0)
				{
					double r = rand() / (RAND_MAX);
					if(exp(-d/T) <= r)
					{
						memcpy(result, result_copy,N * sizeof(int));
					}
				}
    		}
    		p1 = path(result);
    		p2 = path(city_result);
    		if(p1 < p2)
    		{
				#pragma omp critical
    			{
    			memcpy(city_result, result,N * sizeof(int));
    			}
    		}
    	}
    	T *= q;
    	count++;
    }
    end = clock();
    time_sum = (double)(end - start ) / (CLOCKS_PER_SEC);
    printf("total annealing：%d times\n", count);
    printf("shortest distance：%0.2f km.\n", path(city_result));
    for (i = 0; i < N; i++)
    {
        printf("%d->", city_result[i]);
    }
    printf("%d\n", city_result[0]);
    printf("total time taken %fs.\n",time_sum);
    system("pause");
    return 0;
}

double haversine(double *city1, double *city2)
{
    double lat1, lat2, lon1, lon2;
    lat1 = *city1;
    lat2 = *city2;
    lon1 = *(city1 + 1);
    lon2 = *(city2 + 1);
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    // convert to radians
	lat1 = (lat1) * M_PI / 180.0;
	lat2 = (lat2) * M_PI / 180.0;
	double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
	double rad = 6371;
	double c = 2 * asin(sqrt(a));
	return rad * c;
}

double path(int city_result[N])
{
    int i, city1_num, city2_num;
    double sum = 0;
    for (i = 0; i < N - 1; i++)
    {
        city1_num = city_result[i];
        city2_num = city_result[i + 1];
        sum += haversine(city[city1_num], city[city2_num]);
    }
    sum += haversine(city[0], city[N - 1]);
    return sum;
}

void init()
{
    int i;
    for (i = 0; i < N; i++)
        city_result[i] = i;
}

void creat(int city_result[N])
{
    int point1, point2, temp;
    point1 = rand() % N;
    point2 = rand() % N;
    temp = city_result[point1];
    city_result[point1] = city_result[point2];
    city_result[point2] = temp;
}
