/*LINEAR SYSTEM SOLVER: NAIVE LU DECOMPOSITION*/
#include<cfd_hs2017.h>

/* 		// driver section //
int main() {
	double A[2][2] = {{1234,5678},{4321,8765}};		
	double b[2][1] = {{69},{6969}};
	double x[2] = { };
	LUSolver(A, b, x);
	printf("%lf %lf\n", x[0] , x[1]);
}		// driver section //
*/

void LUSolver(double A[][2], double b[][1], double x[2]){
double U[2][2], L[2][2];	
    int i,j,k = 0;
   	double y1, y2, x1, x2;

	// LU decomposition
	for (i = 0; i < 2; i++)	{
    	for (j = 0; j < 2; j++)	{
            if (j < i)
            	L[j][i] = 0;
            else{
                L[j][i] = A[j][i];
                for (k = 0; k < i; k++) {
                    L[j][i] = L[j][i] - L[j][k] * U[k][i];
                }
            }
        }
        for (j = 0; j < 2; j++) {
            if (j < i){
                U[i][j] = 0;
            }
            else if (j == i){
                U[i][j] = 1;
            }
			else {
                U[i][j] = A[i][j] / L[i][i];            
				for (k = 0; k < i; k++) {
                    U[i][j] = U[i][j] - ((L[i][k] * U[k][j]) / L[i][i]);
                }
            }
        }
    }
	// back substitution							
		y1 = b[0][0]/L[0][0];
		y2 = (b[1][0]-L[1][0]*y1)/L[1][1];
		double Y[2][1] = {{y1},{y2}};
		x2 = y2/U[1][1];
		x1 = (y1-U[0][1]*x2)/U[0][0];
		x[0] = x1;
		x[1] = x2;
}