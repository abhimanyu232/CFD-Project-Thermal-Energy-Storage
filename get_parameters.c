#include<cfd_hs2017.h>
/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                    FUNCTION : TAKE INPUT PARAMETERS DURING RUN TIME                      | 
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
void get_parameters(parameters* input){
  printf("enter height : " );
  scanf("%lf", &input->H);
  printf("enter diameter:");
  scanf("%lf", &input->d);

  printf("enter intial temperature:" );
  scanf("%f", &input->ti);

  printf("enter the left boundary condition (Celsius):" );
  scanf("%f", &input->T_bcl );
  printf("enter the right boundary condition (Celsius):" );
  scanf("%f", &input->T_bcr );

  printf("enter fluid velocity: " );
  scanf("%lf",&input->u_f);

  do { 
  printf("please ensure that epsilon is not 1 \n"); 
  printf("enter epsilon :" );
  scanf("%lf",&input->epsilon);
  } while ( input->epsilon == 1 );

  printf("enter density of fluid: " );
  scanf("%lf",&input->rho_f);
  printf("enter specific heat of fluid: ");
  scanf("%lf",&input->Cp_f);

  printf("enter k_f: " );
  scanf("%lf",&input->k_f);

  printf("enter density solid: " );
  scanf("%lf",&input->rho_s);

  printf("enter k_s: " );
  scanf("%lf",&input->k_s);

  printf("enter C_s: " );
  scanf("%lf",&input->C_s);

  printf("enter h_v: " );
  scanf("%lf",&input->h_v);

  printf("Now, enter the duration of each state  :\n");
  printf("Charging: ");
  scanf("%lf",&input->t_charge);
  printf("Discharging: ");
  scanf("%lf",&input->t_disch);
  printf("Idle: ");
  scanf("%lf",&input->t_idle);

  /*
  printf("enter the total simualtion time \n");
  scanf("%f",&input->total_time);
  */
  do {
    printf("enter the (MAX)number of cells: " );
    scanf("%d", &input->N_max );  
    /*printf("enter the number of cells:" );
    scanf("%d", &input->N );*/
    printf("enter the time step, del_t: " );
    scanf("%f", &input->del_t );
  } while (check_stability(input) !=1);

  printf("thanks \n" );
}
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------//


/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                        FUNCTION : READ PARAMETERS FROM FILE                              |  
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/

int read_parameters(parameters* ip){
  int count;
  FILE* fp = fopen("parameters.txt","r");
  if (fp!=NULL){
    count= fscanf(fp, "H=%lf d=%lf t_charge=%lf t_disch=%lf t_idle=%lf ti=%f T_bcl=%f T_bcr=%f epsilon=%lf u_f=%lf rho_f=%lf Cp_f=%lf k_f=%lf rho_s=%lf C_s=%lf k_s=%lf h_v=%lf",  
    &ip->H, &ip->d, &ip->t_charge, &ip->t_disch, &ip->t_idle, &ip->ti, &ip->T_bcl, &ip->T_bcr, &ip->epsilon, &ip-> u_f, &ip-> rho_f, &ip-> Cp_f, &ip-> k_f, &ip-> rho_s, &ip-> C_s, &ip-> k_s, &ip-> h_v);
    if (fclose(fp)!=0){
      printf("error closing parameter.solver after read\n");
      return 0;
    }   
    do {
      printf("enter the (MAX)number of cells: " );
      scanf("%d", &ip->N_max );  
      printf("enter the time step, del_t: " );
      scanf("%f", &ip->del_t );
     } while (check_stability(ip) !=1);

  if (count  == 17)
    return 1;
  }
  else return 0;
}
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------//


/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                        FUNCTION : CHECK STABILITY OF PARAMETERS                           |  
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
int check_stability(parameters* ip){
  double upper =1;
  double lower, stab_val,stab_val_s;
  double DEL_T = ip->del_t; 
  double alpha_s = ip->k_s / ( (1 - ip->epsilon)*(ip->rho_s)*(ip->C_s) ); // INSERT CLAUSE TO MAKE IT IMPOSSIBLE TO GET division by 0
  double alpha_f = ip->k_f / ( (ip->rho_f)*(ip->epsilon)*(ip->Cp_f) );
  double dx = ip->H/ip->N_max;
  lower = (  ip->u_f * ip->u_f * DEL_T * DEL_T )/ (dx*dx);
  stab_val= ( (ip->u_f*DEL_T)/dx ) + (2*alpha_f*DEL_T)/(dx*dx);
  stab_val_s = ( (ip->u_f*DEL_T)/dx ) + (2*alpha_s*DEL_T)/(dx*dx);
    if ( stab_val >= lower && stab_val <= upper){
      if (  stab_val_s >= lower && stab_val_s <= upper)
      return 1;
    }
  printf(" parameters outside known stability region, please reduce del_t (recommended) or increase dx \n");
return 0;
}
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------//

