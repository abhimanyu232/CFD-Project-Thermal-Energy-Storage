#include<cfd_hs2017.h>


/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                  FUNCTIONS : INITIALISE FILES FOR TES STATE AND TEMP                      |
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
  void init_file(int* file_choice, double diameter, const int N){
    FILE *fp1 = fopen("results/TES_state.dat","w");
    if (fp1!=NULL){
      //printf("writing file for time step: %d \n", time_step );
      fprintf(fp1,"TITLE = \"TES_State\" \nVARIABLES = \"time\",\"State\" \n"
      "I=%d,F=POINT \n",N);
      if (fclose(fp1) != 0){
        printf("ERROR CLOSING FILE TES_State DURING INIT\n" );
        exit(1);// ERROR HANDLE
      }      
    } else printf("cant open file TES_State.dat\n");  
    // conditional statement checking for file storage OPTION : INDIVIDUAL or CONDENSED //
    do {
      printf("Multiple files or one condensed dat file.? 1 for Multiple and 0 for condensed\n");  
      scanf("%d",file_choice);
    } while ( (*file_choice)!=0 && (*file_choice)!=1);
    if ((*file_choice)==0){       // FOR A CONDENSED TEMP.DAT INITIALISATION // OTHER BRANCH IN WRITE2FILE
      FILE *fp2 = fopen("results/temp.dat","w");
      if (fp2!=NULL){
        if (fclose(fp2) != 0){
        fprintf(fp2,"TITLE = \" TEMP PLOTS FOR D= %f \" \nVARIABLES = \"Position\",\"Solid Temp\",\"Liquid Temp\" \n", diameter);
          printf("ERROR CLOSING FILE TEMP DATA \n" );
          exit(1);// ERROR HANDLE
        }
      } else printf("cant open file temp.dat\n");
    }
  }
// ---------------------------************************------------------------------------//

/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                  FUNCTION : WRITE DATA TO FILE IN TECPLOT FORMAT                          |
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
  void write2file(double ar[][3], const int N, int time_step,const int file_choice,float DEL_T, int cycle){
    char temp[40]="";
    char f_type[5]=".dat";
    snprintf(temp, 40, "results/c%d_%.4f", cycle, time_step*DEL_T );
    strncat(temp,f_type,40);
    FILE *fp;

    if (file_choice==1) {    // conditional statement checking for file storage OPTION : INDIVIDUAL (1) or CONDENSED (0) //
      fp = fopen(temp,"w");
      if (fp!=NULL){
        fprintf(fp,"TITLE = \" cycle= %d, T_step=%d \" \nVARIABLES = \"Position\",\"Solid Temp\",\"Liquid Temp\" \n", cycle, time_step);
      }  
    }
    else if (file_choice==0){
      fp = fopen("results/temp.dat","a");
    }
    if (fp!=NULL){
      printf("Cycle: %d; writing file for time step: %d \n",cycle, time_step );

      fprintf(fp, "ZONE T=\"%.2f seconds\", I=%d \n",  (cycle*86400 + time_step*DEL_T), N);
      for (int i = 0; i < N; i++ ){
        fprintf(fp,"%lf %lf %lf \n", ar[i][0], ar[i][1], ar[i][2]);
      }
     fputs("  \n", fp);
     if (fclose(fp) != 0){
       printf("ERROR CLOSING FILE TEMP DATA \n" );
       exit(1);// ERROR HANDLE
      }
    } else printf("CANT WRITE DATA/OPEN FILE" ); // ERROR HANDLE
  }
// ---------------------------************************------------------------------------//

/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                  FUNCTIONS : WRITE TES STATE TO FILE IN TECPLOT FORMAT                    |
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
  void write_state(int state, int time_step, float DEL_T){
    FILE *fp1 = fopen("results/TES_state.dat","a");
    if (fp1!=NULL){
      fprintf(fp1,"%f \t %d \t \n", time_step*DEL_T,state);
      if (fclose(fp1) != 0){
        printf("ERROR CLOSING FILE TES_State DURING WRITE \n" );
        exit(1);// ERROR HANDLE
      }
    } else printf("CANT WRITE DATA/OPEN FILE FOR TIME = " ); // ERROR HANDLE
  }
// ---------------------------************************------------------------------------//


/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                     FUNCTIONS : WRITE FILES IN MATLAB FRIENDLY FORMAT                    |
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
  void init_mfile(double ar[][3], const int N){
    FILE *fpl;
    FILE *fps;
      fpl = fopen("matlab/fl_temp.dat","a");
      fps = fopen("matlab/sl_temp.dat","a");

    if (fpl!=NULL){
      fputs("cyc-dx\t", fpl);
      for (int i = 0; i < N; i++ ){
        fprintf(fpl,"%lf\t ", ar[i][0]);
      }
      fputs("  \n", fpl);
    if (fclose(fpl) != 0){
       printf("ERROR CLOSING FILE fluid TEMP DATA \n" );
       exit(1);// ERROR HANDLE
      }
    } else printf("CANT WRITE DATA/OPEN fluid temp FILE" ); // ERROR HANDLE

    if (fps!=NULL){
      fputs("cyc-dx\t", fpl);
      for (int i = 0; i < N; i++ ){
        fprintf(fps,"%6.3lf\t", ar[i][0]);
      }
      fputs(" \n", fps);
    if (fclose(fps) != 0){
       printf("ERROR CLOSING FILE solid TEMP DATA \n" );
       exit(1);// ERROR HANDLE
      }
    } else printf("CANT WRITE DATA/OPEN solid temp FILE" ); // ERROR HANDLE
  }
  // ---------------------------************************------------------------------------//


  void write2matlab(double ar[][3], const int N, int time_step, float DEL_T, int cycle){
    char f_type[5]=".dat";
    FILE *fpl;
    FILE *fps;

      fpl = fopen("matlab/fl_temp.dat","a");
      fps = fopen("matlab/sl_temp.dat","a");

    if (fps!=NULL){
      printf("Cycle: %d; writing file for time step: %d \n",cycle, time_step );
      fprintf(fps,"c%d_%6.3lf \t", cycle, time_step*DEL_T);
      for (int i = 0; i < N; i++ ){
        fprintf(fps,"%6.3lf\t ", ar[i][1]);
      }
     fputs("  \n", fps);
     if (fclose(fps) != 0){
       printf("ERROR CLOSING FILE solid TEMP DATA \n" );
       exit(1);// ERROR HANDLE
      }
    } else printf("CANT WRITE DATA/OPEN solid temp FILE" ); // ERROR HANDLE

    if (fpl!=NULL){
      printf("Cycle: %d; writing file for time step: %d \n",cycle, time_step );
      fprintf(fpl,"c%d_%6.3lf \t", cycle, time_step*DEL_T);
      for (int i = 0; i < N; i++ ){
        fprintf(fpl,"%6.3lf\t", ar[i][2]);
      }
     fputs("  \n", fpl);
     if (fclose(fpl) != 0){
       printf("ERROR CLOSING FILE fluid TEMP DATA \n" );
       exit(1);// ERROR HANDLE
      }
    } else printf("CANT WRITE DATA/OPEN fluid temp FILE" ); // ERROR HANDLE
  }
// ---------------------------************************------------------------------------//

// ---------------------------************************------------------------------------//
