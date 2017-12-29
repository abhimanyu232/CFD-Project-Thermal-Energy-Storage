#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

// mkdir.c headers
#include <limits.h>     /* PATH_MAX */
#include <sys/stat.h>   /* mkdir(2) */
#include <errno.h>
// mkdir headers

#define PI 3.141592653589793
#define MAX_FILE_NAME 15
//#define DEL_T 0.05
//#define FINAL_T 10				// CHARGE + DISCHARGE + IDLE = TOTAL TIME
//#define TOT_TIME_STEP FINAL_T/DEL_T

typedef struct parameters{
double H;
double d;
float ti;  								// ti = initial temp of solid and liquid phase
float T_bcl;
float T_bcr;

int N;
int N_max;
double t_charge;
double t_disch;
double t_idle; 
float del_t;
float total_time;
int n_cycle;
int tstep_cycle ;

double epsilon; 
double u_f;
double rho_f;
double k_f;
double Cp_f;
double h_v;
double rho_s;
double C_s;
double k_s;	
}parameters;

int read_parameters(parameters* ip);
void get_parameters(parameters*);
int check_stability(parameters*);		// RETURNS 1 IF PARAMETERS (DX DT ) CHOSEN LEAD TO STABILITY

void Charging_eqn(parameters* ip, double alpha_f, double alpha_s, double DEL_T, double h, double prv[][3], double nxt[][3] );
void Idle_eqn(parameters* ip, double alpha_f, double alpha_s, double DEL_T, double h, double prv[][3], double nxt[][3] );
void Discharging_eqn(parameters* ip, double alpha_f, double alpha_s, double DEL_T, double h, double prv[][3], double nxt[][3] );


int calculate(parameters*, const int);
double (*mem_allo(int N))[3];			//mem_allo returns a pointer to a N array of 3 doubles

double source(double x,double alpha_f,double alpha_s,double u, float L, int n, double h_v_f, double h_v_s, int choice);
double sol_man(int n, double x, double L, int choice);
/*double sol_man(int n, double x, double L);
double sol_man_s(int n, double x, double L);
double src_fluid(double x,double alpha_f,double u, float L, int n, double h_v_f);
double src_solid(double x,double alpha_s,double u, float L, int n, double h_v_s);
*/
void init_file(int * file_choice, double diameter, const int N);
void write_state(int state, int time_step, float DEL_T);
void write2file(double (*)[3], const int n_cells, int counter, const int file_choice, float DEL_T, int cycle );
void error2file_init(parameters* );
void error2file(double error, double error_avg, float h,  float Pe, int n);


void LUSolver(double A[][2], double b[][1], double x[2]);
//////////////////////////GAUSS ELIMINATION//////////////////////////////////////
void gauss(int n, double A[n][n], double b[n],  double x[n]);
void eliminate(int n, double A[n][n], double s[n],double b[n],double tol, int* er);
void pivot(int n,double A[n][n], double b[n], double s[n], int k);
void substitute(int n, double A[n][n], double b[n], double x[n]);
/////////////////////////GAUSS ELIMINATION//////////////////////////////////////

int mkdir_p(const char *path);

//////////////////////////////////
void init_mfile(double ar[][3], const int N);
void write2matlab(double ar[][3], const int N, int time_step, float DEL_T, int cycle);
