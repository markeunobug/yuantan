#include "Auto_Zoom_Process.h"

void Auto_Zoom(unsigned char id )
{
  switch(id)
  {
    case 0:
        {
            if ((Max_avg_R1A_Q<R1A_Threshold_Low)&&(Max_avg_R1A_I<R1A_Threshold_Low))//
            {
                if(Auto_Zoom_R1A[0]<8)
                   Auto_Zoom_R1A[0]++;
            }
            if ((Max_avg_R1A_Q>R1A_Threshold_High)||(Max_avg_R1A_I>R1A_Threshold_High))//
            {
                if(Auto_Zoom_R1A[0]>1)
                   Auto_Zoom_R1A[0]--;
            }

            break;
        }
    case 1:
        {
            if ((Max_avg_R1A_Q<R1A_Threshold_Low)&&(Max_avg_R1A_I<R1A_Threshold_Low))//
            {
                if(Auto_Zoom_R1A[1]<8)
                   Auto_Zoom_R1A[1]++;
            }
            if ((Max_avg_R1A_Q>R1A_Threshold_High)||(Max_avg_R1A_I>R1A_Threshold_High))//
            {
                if(Auto_Zoom_R1A[1]>1)
                   Auto_Zoom_R1A[1]--;
            }
            if ((Max_avg_R2A_Q<R2A_Threshold_Low)&&(Max_avg_R2A_I<R2A_Threshold_Low))//
            {
                if(Auto_Zoom_R2A[0]<8)
                   Auto_Zoom_R2A[0]++;
            }
            if ((Max_avg_R2A_Q>R2A_Threshold_High)||(Max_avg_R2A_I>R2A_Threshold_High))//
            {
               if(Auto_Zoom_R2A[0]>1)
                  Auto_Zoom_R2A[0]--;
            }

            break;
        }
    case 2:
       {
           if ((Max_avg_R1A_Q<R1A_Threshold_Low)&&(Max_avg_R1A_I<R1A_Threshold_Low))//
           {
               if(Auto_Zoom_R1A[2]<8)
                  Auto_Zoom_R1A[2]++;
           }
           if ((Max_avg_R1A_Q>R1A_Threshold_High)||(Max_avg_R1A_I>R1A_Threshold_High))//
           {
               if(Auto_Zoom_R1A[2]>1)
                  Auto_Zoom_R1A[2]--;
           }
           if ((Max_avg_R2A_Q<R2A_Threshold_Low)&&(Max_avg_R2A_I<R2A_Threshold_Low))//
           {
               if(Auto_Zoom_R2A[1]<8)
                  Auto_Zoom_R2A[1]++;
           }
           if ((Max_avg_R2A_Q>R2A_Threshold_High)||(Max_avg_R2A_I>R2A_Threshold_High))//
           {
              if(Auto_Zoom_R2A[1]>1)
                 Auto_Zoom_R2A[1]--;
           }

           break;
       }
    case 3:
       {
           if ((Max_avg_R2A_Q<R2A_Threshold_Low)&&(Max_avg_R2A_I<R2A_Threshold_Low))//
           {
               if(Auto_Zoom_R2A[2]<8)
                  Auto_Zoom_R2A[2]++;
           }
           if ((Max_avg_R2A_Q>R2A_Threshold_High)||(Max_avg_R2A_I>R2A_Threshold_High))//
           {
              if(Auto_Zoom_R2A[2]>1)
                 Auto_Zoom_R2A[2]--;
          }

          break;

       }
 /*   case 4:
       {
          Zoom_R3U=Auto_Zoom_R3U[1];
          Zoom_R3D=Auto_Zoom_R3D[1];
          break;

       }*/
    default:
       {
           break;
       }
  }
};

