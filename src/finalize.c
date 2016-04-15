#include "../include/allvars.h"
#include "../include/transfer_options.h"
#include "../include/proto.h"
#include <stdio.h>
#include <stdlib.h>

void Finalize()
{
  if (ThisTask == 0)
    {
      
      printf("\nWriting temperature.bin...\n");
      fileout = fopen("OUTPUTS/temperature.bin","wb");
      fwrite(T_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);

      printf("Writing beta.bin...\n");
      fileout = fopen("OUTPUTS/beta.bin","wb");
      fwrite(Beta_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);

      printf("Writing partition.bin...\n");
      fileout = fopen("OUTPUTS/partition.bin","wb");
      fwrite(Partition_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);
    
    }

}
