// COMPILE WITH function.c & gauss_elim.c//
/*------------------------------------*/
#include<cfd_hs2017.h>

int main(){
	
	char path1[] = "results";
	char path2[] = "matlab";
	mkdir_p(path1);
	//mkdir_p(path2);
	parameters ip;
	int param_choice;
	do{
		printf("PRESS 1 TO READ SIMULATION PARAMETERS FROM FILE -> \"parameters.txt\". YOU CAN EDIT THE FILE BEFORE PRESSING 1 \n"
		"or \n PRESS 0 TO ENTER PARAMETERS AT RUNTIME ");
		scanf("%d",&param_choice);
	} while(param_choice!=1 && param_choice!=0 );
	
		if (param_choice==1){
			read_parameters(&ip);							//read simulation parameters from file	
		}
		else if (param_choice==0){			
			get_parameters(&ip);							//call for simulation parameters	
		}
			

  printf("enter the total number of cycles: ");
  scanf("%d",&ip.n_cycle);


		printf("\n");
		printf("L: %.2lf \t D: %.2lf \t with \t max cells: %d  &  del_t:%0.6lf  \n", ip.H,ip.d,ip.N_max, ip.del_t );
		printf("ti: %.2lf \t BC_LEFT: %.2lf \t BC_RIGHT: %.2f \n", ip.ti,ip.T_bcl,ip.T_bcr );
		printf("Duration of states (seconds): \nCharging: %.2lf \t Discharging: %.2lf \t Idle: %.2f \n", ip.t_charge,ip.t_disch,ip.t_idle );
		printf("rho_f: %.2lf \t Cp_f: %.2lf \t k_f: %.2lf \n", ip.rho_f,ip.Cp_f,ip.k_f );
		printf("rho_s: %.2lf \t C_s: %.2lf \t k_s: %.2lf \n", ip.rho_s,ip.C_s,ip.k_s );
		printf("u_f: %lf \t h_v: %.2lf \t epsilon: %.2lf \n", ip.u_f, ip.h_v,ip.epsilon );
		printf("number of cycles %d\n", ip.n_cycle );

	int file_choice;
	init_file(&file_choice,ip.d,ip.N_max);
	//error2file_init(&ip);
	calculate(&ip,file_choice);


  return 0;
}
