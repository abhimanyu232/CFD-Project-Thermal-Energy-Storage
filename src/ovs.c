///////////////////////// ORDER VERIFICATION STUDY FUNCTIONS ////////////////////////////////
#include"cfd_hs2017.h"

/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
//                  FUNCTIONS : MMS SOURCE TERM & Manufactured Solution                    //
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/

  double source(double x,double alpha_f,double alpha_s,double u, float L, int n, double h_v_f, double h_v_s, int choice){
    double f_src, s_src;
    if (choice == 1){
      f_src = ( ( 4*alpha_f*n*n*PI*PI*cos((2*PI*n*x)/L) )/(L*L) ) - ( (2*n*u*PI*sin((2*PI*n*x)/L))/L ) + h_v_f*( sol_man(n,x,L,1) - sol_man(n,x,L,0)  );
      return f_src;
    }
    else if (choice == 0){
      s_src = ( ( 4*alpha_s*n*n*PI*PI*cos((2*PI*n*x)/L) )/(L*L) ) + h_v_s*( sol_man(n,x,L,0) - sol_man(n,x,L,1) );
      return s_src;
    }
    return 0;
  }
  // ---------------------------************************------------------------------------//

  double sol_man(int n, double x, double L, int choice){      // CHOICE = 1 FOR FLUID ;  CHOICE = 0 FOR SOLID
    double sol_f,sol_s;
    if (choice == 1){
      sol_f = cos( (2*PI*n*x)/L );
      return sol_f;
    }
    else if (choice == 0){
      sol_s = sin( (2*PI*n*x)/L );
      return sol_s;
    }
    return 0;
  }
// ---------------------------************************------------------------------------//

/*-------------------------XXXXXXXXXXXXXxxxxxxxxXXXXXXXXXXXX-------------------------------//
|                  FUNCTIONS : WRITE MMS ERROR TO FILE IN TECPLOT FORMAT                    |
// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------*/
  void error2file_init(parameters* ip){
  FILE* init = fopen("results/ovsplot.dat","a");
      if (init!=NULL){
         fprintf(init,"**************************\nTime Step Size: %f \n"
        "-----------------------------------------------------------------------\n"
        "GRID SIZE \t ERR_ABS \t ERR_L1 \t N \t PECLET NUMBER \n"
        "----------------------------------------------------------------------- \n" , ip->del_t);
        if (fclose(init)!=0){
            fprintf(stderr,"CANT CLOSE FILE error2file\n");
            exit(1);
          }
      } else fprintf(stderr, "ERROR OPENING FILE\n" );
  }
  // ---------------------------************************------------------------------------//


  void error2file(double error,double error_avg, float h, float Pe, int n){
    FILE* er1 = fopen("results/ovsplot.dat","a");
    if (er1!=NULL){
      //fprintf(er1, "Grid Spacing, Error, no. of wavelengths, peclet number \n" );
      fprintf(er1,"%f \t %lf \t %lf \t %d \t %f \n \n", h, error , error_avg , n , Pe );
      if (fclose(er1)!=0){
        fprintf(stderr,"CANT CLOSE FILE error2file\n");
        exit(1);
      }
    } else fprintf(stderr, "ERROR OPENING FILE\n" );
  }
// ---------------------------************************------------------------------------//


// ---------------------------XXXXXXXXXXXXXXXXXXXXXXXXX------------------------------------//
