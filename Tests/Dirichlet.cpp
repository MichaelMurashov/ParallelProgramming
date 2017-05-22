#include <stdlib.h> 
#include <math.h> 

void GaussJacobiCalculation (double ** v_in, double ** v_out, double ** f, const double eps, const int size) { 
	int i,j; 
	double dmax, dm, temp, d; 
	double **tmp; 
	double h = 1.0/(size+1);

	omp_lock_t dmax_lock; 
	omp_init_lock(&dmax_lock); 
//#pragma omp parallel shared(v_in, dmax) private(i,temp,d,dm) 
{ 
	do { 
		dmax = 0.0; // максимальное изменение значений u 
		#pragma omp parallel for private(i,temp,d,dm) 
		for (i = 1; i < size + 1; i++) { 
			dm = 0.0; 
			for (j = 1; j < size + 1; j++) { 
				temp = v_in[i][j]; 
				v_out[i][j] = 0.25*(v_in[i - 1][j] + v_in[i + 1][j] + 
				v_in[i][j - 1] + v_in[i][j + 1] - h*h*f[i][j]); 
				d=fabs(temp - v_out[i][j]); 
				if (dm < d) dm = d; 
			} 
			omp_set_lock(&dmax_lock); 
			if (dmax < dm) dmax = dm; 
			omp_unset_lock(&dmax_lock); 
		}	 
		for ( i=1; i<size+1; i++ ) // обновление данных 
			for ( j=1; j<size+1; j++ ) 
				v_in[i][j] = v_out[i][j]; 
	} // конец параллельной областb 
	while (dmax > eps); 
} 
	v_out=v_in; 
}
