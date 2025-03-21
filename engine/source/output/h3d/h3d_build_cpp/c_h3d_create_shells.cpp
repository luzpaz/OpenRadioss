//Copyright>    OpenRadioss
//Copyright>    Copyright (C) 1986-2025 Altair Engineering Inc.
//Copyright>
//Copyright>    This program is free software: you can redistribute it and/or modify
//Copyright>    it under the terms of the GNU Affero General Public License as published by
//Copyright>    the Free Software Foundation, either version 3 of the License, or
//Copyright>    (at your option) any later version.
//Copyright>
//Copyright>    This program is distributed in the hope that it will be useful,
//Copyright>    but WITHOUT ANY WARRANTY; without even the implied warranty of
//Copyright>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//Copyright>    GNU Affero General Public License for more details.
//Copyright>
//Copyright>    You should have received a copy of the GNU Affero General Public License
//Copyright>    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//Copyright>
//Copyright>
//Copyright>    Commercial Alternative: Altair Radioss Software
//Copyright>
//Copyright>    As an alternative to this open-source version, Altair also offers Altair Radioss
//Copyright>    software under a commercial license.  Contact Altair to discuss further if the
//Copyright>    commercial version may interest you: https://www.altair.com/radioss/.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>



#ifdef _WIN32
/* Windows includes */
#include <windows.h>
#include <process.h>
#include <io.h>
#include <sys\types.h>
#include <sys/stat.h>


#elif 1

/* Linux includes */
#include <sys/resource.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#define _FCALL
#include <math.h>
#include <stdbool.h>

#endif

#include "h3dpublic_defs.h"
#include "h3dpublic_export.h"

#define _FCALL 

#include "h3d_values.h"

extern "C" 
/*=================================================================*/
{

/*=================================================================*/
/*        C_H3D_CREATE_SHELLS                                      */
/*=================================================================*/
void c_h3d_create_shells_(int *ITAB, int *NUMNOD, int *IXC, int *NIXC, int *NUMELC, int *IPARTC, int *IPART, int *LIPART1, int *H3D_PART)
{
    unsigned int * nbelems = new unsigned int [*NUMELC];
    try {


        // create Elements
        unsigned int conn[4] ;
        H3D_ID elem_id ;
        int i,j,cpt_elem,nbelemwrite;

        char SH4NPOOL[] = "SH4N";
        rc = Hyper3DAddString(h3d_file, SH4NPOOL, &sh4n_poolname_id);
        if( !rc ) throw rc;

        char SHELLPOOL[] = "Shell";
        rc = Hyper3DAddString(h3d_file, SHELLPOOL, &shell_poolname_id);
        if( !rc ) throw rc;

        unsigned int elem_count = 1;

        comp_id = 0;
        j = 0;
        for(i=0;i<*NUMELC;i++)  nbelems[i] = 0;

        for(i=0;i<*NUMELC;i++)
        {  
        if(H3D_PART[IPARTC[i] - 1] == 1)
          {
             if(IPART[*LIPART1 * (IPARTC[i] - 1) + 3] != comp_id)
             {
                  comp_id = IPART[*LIPART1 * (IPARTC[i] - 1) + 3];
                  j = i;
             }
             nbelems[j] = nbelems[j] + 1;
          }
        }

        comp_id = 0;
        cpt_elem = 0;
        nbelemwrite = 0;

        for(i=0;i<*NUMELC;i++)  
        {  
        if(H3D_PART[IPARTC[i] - 1] == 1)
          {
             conn[0] = IXC[*NIXC * i + 1];
             conn[1] = IXC[*NIXC * i + 2];
             conn[2] = IXC[*NIXC * i + 3];
             conn[3] = IXC[*NIXC * i + 4];
             elem_id = IXC[*NIXC * i + *NIXC - 1];

             if(nbelems[i] != 0)
             {
                  cpt_elem = 0;
                  nbelemwrite = nbelems[i];
                  comp_id = IPART[*LIPART1 * (IPARTC[i] - 1) + 3] ;

                  rc = Hyper3DElementBegin(h3d_file, nbelemwrite, sh4n_poolname_id, 
                                    H3D_ELEM_CONFIG_QUAD4, comp_id, 
                                    shell_poolname_id, node_poolname_id);
                  if( !rc ) throw rc;
             }
             cpt_elem++;
             rc = Hyper3DElementWrite(h3d_file, elem_id, conn);
             if( !rc ) throw rc;

             if (cpt_elem == nbelemwrite)
             {
                  rc = Hyper3DElementEnd(h3d_file);
                  if( !rc ) throw rc;
             }
          }
       }


    } // end of try

    catch(...) {
        Hyper3DExportClearError(h3d_file);
    }
    delete [] nbelems;

}

void _FCALL C_H3D_CREATE_SHELLS(int *ITAB, int *NUMNOD, int *IXC, int *NIXC, int *NUMELC, int *IPARTC, int *IPART, int *LIPART1, int *H3D_PART)
{c_h3d_create_shells_ (ITAB,NUMNOD,IXC,NIXC,NUMELC,IPARTC,IPART,LIPART1,H3D_PART);}

void c_h3d_create_shells__ (int *ITAB, int *NUMNOD, int *IXC, int *NIXC, int *NUMELC, int *IPARTC, int *IPART, int *LIPART1, int *H3D_PART)
{c_h3d_create_shells_ (ITAB,NUMNOD,IXC,NIXC,NUMELC,IPARTC,IPART,LIPART1,H3D_PART);}

void c_h3d_create_shells (int *ITAB, int *NUMNOD, int *IXC, int *NIXC, int *NUMELC, int *IPARTC, int *IPART, int *LIPART1, int *H3D_PART)
{c_h3d_create_shells_ (ITAB,NUMNOD,IXC,NIXC,NUMELC,IPARTC,IPART,LIPART1,H3D_PART);}

}
