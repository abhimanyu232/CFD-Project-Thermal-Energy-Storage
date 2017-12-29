/*FUNCTIONS TO DO THE **ACTUAL** CALCULATIONS */
#include<cfd_hs2017.h>

/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                        FUNCTION : ALLOCATE MEMORY FOR ARRAY                              |
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
  double (*mem_allo(int N))[3]{        //mem_allo returns a pointer to N array of 3 doubles
    double (*out)[3];
    out = calloc(N , sizeof(*out) );    // allocate and initialise memory for storage
    if (out == NULL){
      printf( "Mem allocation FAIL! at ABORT!\n" );
      exit(EXIT_FAILURE);
    }
  return out;
  }
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------//


/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                          FUNCTION : CALCULATE THE SOLUTION                                |
|                                                                                           |  
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/

  int calculate( parameters* ip, const int file_choice){
    //for (int no = 1 ; no <= 8 ; no+=2){
    //  for (ip->u_f = 0.0001 ; ip->u_f<1 ; ip->u_f*=10 ){
   int save_file_in;
   printf("How often do you want to save data file? _____ timesteps (enter multiples of 10 please)\n");
   scanf("%d",&save_file_in);
    int state;
    ip->N=ip->N_max;
    double h = ip->H/ip->N;   
    const float DEL_T = ip->del_t; 
    double FINAL_T = ip->t_charge + ip->t_disch + (2 * ip->t_idle) ;
    double alpha_f = ip->k_f/( (ip->rho_f)*(ip->epsilon)*(ip->Cp_f) );
    double alpha_s = ip->k_s/( (1 - ip->epsilon)*(ip->rho_s)*(ip->C_s) ); // INSERT CLAUSE TO MAKE IT IMPOSSIBLE TO GET division by 0
    double Pe = (ip->u_f*ip->H)/alpha_f; 
    double h_v_f = ip->h_v / ( ip->epsilon * ip->rho_f * ip->Cp_f );
    double h_v_s = ip->h_v / ( (1-ip->epsilon)*ip->rho_s*ip->C_s );
    /*int no; 
    for (ip->N=5; ip->N < ip->N_max ; (ip->N)*=2 ){          // RUN OVS OVER DIFFERENT GRID SPACINGS
    h = ip->H/ip->N;   
    */
    if ( check_stability(ip) != 1){
      printf("stability limit exceeded : Solution Unstable for H= %lf, V= %lf \n", ip->H, ip->u_f);
    return 0;
    }
    //    printf("OVS Underway ; Grid Size : %d cells\n", ip->N );  
    int time_step;
    int cycle = 0;
    double (*prv)[3]=mem_allo(ip->N);
    for (int i=0;i<ip->N;i++){                                    // INITIALISE DOMAIN 
      double l = (i+1)*h; 
      prv[i][0]= l;                                               // length at loc i
      prv[i][1]=  ip->ti; //sol_man(no,l,ip->H,0);                 // solid temp at loc i at l
      prv[i][2]=  ip->ti;//sol_man(no,l,ip->H,1);//                // liq temp at loc i
    }
    write2file(prv,ip->N,0,file_choice,DEL_T,cycle);
    init_mfile(prv,ip->N);                            // HERE DELETE THIS ONCE DONE WITH MATLAB     
    double err_max=1.0;                              // WRITE INITIAL CONDTIONS TO FILE (T=0)
    //do {      float sim_time = DEL_T;  
    //do {  
    //time_step=0;
    //printf("current cycle : %d \n", cycle); 
    do{
      time_step=0;
      for (double sim_time = 0; sim_time <= FINAL_T; sim_time += DEL_T ){           // -----------TIME LOOP START-------------//
            double (*nxt)[3]=mem_allo(ip->N);        

            if (sim_time <= ip->t_charge){            // CHARGING 
              state = 1; 
              if (time_step%save_file_in==0){
                printf("the thermocline is charging...\n");  
                write_state(state,time_step,DEL_T);
              }
              Charging_eqn( ip, alpha_f, alpha_s, DEL_T, h,prv ,nxt);
            }
            else if (sim_time > ip->t_charge && sim_time <= (ip->t_charge+ip->t_idle)){     // IDLE STATE
              state = 0;      
              if (time_step%save_file_in==0){
                printf("the thermocline is idling...\n");              
                write_state(state,time_step,DEL_T);
              }
              Idle_eqn( ip, alpha_f, alpha_s, DEL_T, h, prv , nxt);
            }                                                    
            else if (sim_time > (ip->t_charge+ip->t_idle ) && sim_time <= (ip->t_charge+ip->t_idle+ip->t_disch)){   // DISCHARGING 
              state = -1;                      
              if (time_step%save_file_in==0){
                printf("the thermocline is discharging...\n");  
                write_state(state,time_step,DEL_T);
              }
              Discharging_eqn( ip, alpha_f, alpha_s, DEL_T, h,prv ,nxt);
            }    
            else if (sim_time > (ip->t_charge + ip->t_idle + ip->t_disch) && sim_time<=FINAL_T  ){      // IDLE STATE
              state = 0;      
              if (time_step%save_file_in==0){
                printf("the thermocline is idling...\n");  
                write_state(state,time_step,DEL_T);
              }
              Idle_eqn( ip, alpha_f, alpha_s, DEL_T, h,prv ,nxt);
            }

            //SOLVE LINEAR SYSTEM //
            double A[2][2] = { { 1+(h_v_f*DEL_T) , (-1)*h_v_f*DEL_T } , { (-1)*h_v_s*DEL_T , 1+(h_v_s*DEL_T) } };
            for (int i = 0; i < ip->N; i++) {
              double x1[2] = {  };  
              double b[2][1] = { {nxt[i][2]} , {nxt[i][1]}  };
              LUSolver(A,b,x1);
              nxt[i][2] = x1[0];
              nxt[i][1] = x1[1];
            } 
            // SOLVE LINEAR SYSTEM//
            
            double err_i;
            err_max=0.0000;                     // PER ITERATION ERROR
            for (int i = 0; i < ip->N; i++){
              err_i = fabs( nxt[i][2] - prv[i][2] );    
              if (err_i > err_max){ 
                err_max = err_i;
              }  
            }          
            // CONVERGENCE CHECK //            
            /*       
            double res=0;
            for (int i = 0; i < ip->N; i++){
              res = res + fabs( nxt[i][2]-prv[i][2] );        // CHECKING CONVERGENCE FOR FLUID NOW, CHANGE j to 1 for SOLID 
            }
            err = res/(ip->N);
            */
            // CONVERGENCE CHECK //

            // OVS ERROR //
            /* 
            if (err <= 1e-6){
              double error = 0;
              double error_avg = 0;
              // CHECKING CONVERGENCE FOR FLUID NOW, 
              //FOR SOLID CHANGE j to 1 and LAST ARGUEMENT of sol_man() to 0 (from 1)
              for ( int i = 0; i < ip->N; i++){
                error = error + fabs( (nxt[i][2]-sol_man(no,i*h,ip->H,1)) / (2+sol_man(no,i*h,ip->H,1)) );     
              }
              printf("average residual at convergence = %lf \n",err);
              error_avg = error/ip->N;
              error2file(error,error_avg, h, Pe, no);
            }
            */
            // OVS ERROR // 
            
            free(prv);  
            double (*prv)[3]=mem_allo(ip->N);
            memcpy(prv,nxt,(ip->N) * sizeof(*nxt));
            free(nxt);
            if (time_step%save_file_in==0){
              printf("writing file for timesep %d \n",time_step );
              printf("error max = %lf\n", err_max );
              write2file(prv,ip->N,time_step,file_choice,DEL_T,cycle);    
            }
            if (time_step==10800 || time_step==21600 ||  time_step==32400 || time_step==43190){
              printf("writing file for timesep %d \n",time_step );
              printf("error max = %lf\n", err_max );
              //write2matlab(prv,ip->N,time_step,DEL_T,cycle);    
              write2file(prv,ip->N,time_step,file_choice,DEL_T,cycle);    
            }   
            time_step++;                          
      }
      //sim_time+=DEL_T;
      cycle++;
    } while(cycle <=ip->n_cycle);     // -----------TIME LOOP END-------------// 
      free(prv); 
      printf("solution converged after %d CYCLES\n", cycle );                         
  return 0; 
  }   
  //} // velocity loop 
  //} //no of wavelengths loop
  //}//-------------------FUNCTION END--------------------------//
// ---------------------------************************------------------------------------//

    // DISCRETE EQUATION DEFINITIONS //

    void Charging_eqn(parameters* ip, double alpha_f, double alpha_s, double DEL_T, double h, double prv[][3], double nxt[][3] ){
        //LEFT BC
        nxt[0][0] = h;
        nxt[0][1] = prv[0][1] +  alpha_s*( DEL_T / (h*h) )*(prv[1][1]-prv[0][1]);// + DEL_T*source(h,alpha_f,alpha_s,ip->u_f,ip->H,no,h_v_f,h_v_s,0) ;
        nxt[0][2] = prv[0][2] - ( ip->u_f*( DEL_T / h )* ( prv[0][2] - ip->T_bcl)  ) + 
                                ( alpha_f * ( DEL_T / (h*h) ) * (prv[1][2]-prv[0][2] ) );// + DEL_T*source(h,alpha_f,alpha_s,ip->u_f,ip->H,no,h_v_f,h_v_s,1);
        //LEFT BC
        for (int i=1;i<ip->N-1;i++){
          double l = (i+1)*h;
          nxt[i][0]= l;
          nxt[i][1] = prv[i][1] + ( alpha_s * ( DEL_T / (h*h) ) * ( prv[i+1][1] - 2*prv[i][1] + prv[i-1][1] ) );// + DEL_T*source(h*(i+1),alpha_f,alpha_s,ip->u_f,ip->H,no,h_v_f,h_v_s,0);
          nxt[i][2] = prv[i][2] - (ip->u_f * ( DEL_T / h ) * ( prv[i][2] - prv[i-1][2] ) ) +  
                                  ( alpha_f * ( DEL_T / (h*h) ) * ( prv[i+1][2] - 2*prv[i][2] + prv[i-1][2] ) );//+ DEL_T*source(h*(i+1),alpha_f,alpha_s,ip->u_f,ip->H,no,h_v_f,h_v_s,1);
        }
        // RIGHT BC
        nxt[ip->N-1][0] = (ip->N)*h;
        nxt[ip->N-1][1] = prv[ip->N-1][1] +  alpha_s*( DEL_T / (h*h) )* ( prv[ip->N-2][1] - prv[ip->N-1][1] );// + DEL_T*source((ip->N)*h , alpha_f,alpha_s, ip->u_f, ip->H,no,h_v_f,h_v_s,0);
        nxt[ip->N-1][2] = prv[ip->N-1][2] - (ip->u_f * ( DEL_T / h ) * (prv[ip->N-1][2]-prv[ip->N-2][2])) + 
                          ( alpha_f * ( DEL_T / (h*h) ) * ( prv[ip->N-2][2] - prv[ip->N-1][2] ) );// + DEL_T*source((ip->N)*h ,alpha_f,alpha_s,ip->u_f,ip->H,no,h_v_f,h_v_s,1);
        // RIGHT BC 
    }

    void Idle_eqn(parameters* ip, double alpha_f, double alpha_s, double DEL_T, double h, double prv[][3], double nxt[][3] ){

        //LEFT BC
        nxt[0][0] = h;
        nxt[0][1] = prv[0][1] + ( alpha_s * ( DEL_T / (h*h) ) * (prv[1][1]-prv[0][1]) );
        nxt[0][2] = prv[0][2] + ( alpha_f * ( DEL_T / (h*h) ) * (prv[1][2]-prv[0][2]) );
        //LEFT BC
        for (int i=1;i<ip->N-1;i++) {
          double l = (i+1)*h;
          nxt[i][0]= l;
          nxt[i][1]= prv[i][1] + ( alpha_s * ( DEL_T / (h*h) ) * ( prv[i+1][1] - 2*prv[i][1] + prv[i-1][1] ) );
          nxt[i][2]= prv[i][2] + ( alpha_f * ( DEL_T / (h*h) ) * ( prv[i+1][2] - 2*prv[i][2] + prv[i-1][2] ) );
        }
        // RIGHT BC
        nxt[ip->N-1][0] = (ip->N)*h;
        nxt[ip->N-1][1] = prv[ip->N-1][1] + ( alpha_s * ( DEL_T / (h*h) ) * ( prv[ip->N-2][1] - prv[ip->N-1][1]) );
        nxt[ip->N-1][2] = prv[ip->N-1][2] + ( alpha_f * ( DEL_T / (h*h) ) * ( prv[ip->N-2][2] - prv[ip->N-1][2]) );
        // RIGHT BC
    }


    void Discharging_eqn(parameters* ip, double alpha_f, double alpha_s, double DEL_T, double h, double prv[][3], double nxt[][3] ){
        double u_d = -1*ip->u_f;
        //LEFT BC
        nxt[0][0] = h;
        nxt[0][1] = prv[0][1] + ( alpha_s*( DEL_T / (h*h) )*(prv[1][1]-prv[0][1]) );
        nxt[0][2] = prv[0][2] - ( u_d*( DEL_T / h )*(prv[1][2] - prv[0][2]) ) + 
                    ( alpha_f * ( DEL_T / (h*h) ) * ( prv[1][2]-prv[0][2] ) );
        //LEFT BC
        //INTERIOR
        for (int i=1;i<ip->N-1;i++) {
          double l = (i+1)*h;
          nxt[i][0]= l;
          nxt[i][1] = prv[i][1] + ( alpha_s * ( DEL_T / (h*h) ) * ( prv[i+1][1] - 2*prv[i][1] + prv[i-1][1] ) );
          nxt[i][2] = prv[i][2] - ( u_d * ( DEL_T / h ) * ( prv[i+1][2] - prv[i][2] ) ) + 
                                  ( alpha_f * ( DEL_T / (h*h) ) * ( prv[i+1][2] - 2*prv[i][2] + prv[i-1][2] ) );
        }
        //INTERIOR
        //RIGHT BC
        nxt[ip->N-1][0] = (ip->N)*h;
        nxt[ip->N-1][1] = prv[ip->N-1][1] + ( alpha_s * ( DEL_T / (h*h) ) * (prv[ip->N-2][1] - prv[ip->N-1][1]) );
        nxt[ip->N-1][2] = prv[ip->N-1][2] - ( u_d * ( DEL_T / h ) * ( ip->T_bcr - prv[ip->N-1][2]  ) ) + 
                                            ( alpha_f*( DEL_T/(h*h) )*(prv[ip->N-2][2] - prv[ip->N-1][2]) );
    }

//***************************************************END*********************************************************///
//----------------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX--------------------------------------------//

