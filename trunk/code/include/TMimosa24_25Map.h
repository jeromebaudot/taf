#ifndef CLASS_TMimosa24_25Map
#define CLASS_TMimosa24_25Map

//
//Example of usage:
//
// #include "TMimosa24_25Map.h"
//int main(int argc, char **argv) {
//	TMimosa24_25Map *Mi = new TMimosa24_25Map("Mi25A_20um"); // or "Mi24", "Mi25B_20um","Mi25A_30um","Mi25B_30um","Mi25A_40um","Mi25B_40um"      
//	Mi->print_map(2); // print n sample and k channel of TNT board in a Mimosa matrix layout..
//	Mi->print_map(1); // print n sample of TNT board in a Mimosa matrix layout..
//	delete Mi;
//}



#include <string.h>
#include <stdio.h>

class TMimosa24_25Map {
public:
  int mats;
  
  static const int buff_k_max=8;
  static const int buff_n_max=4096;
  
  static const int buff_m_max=13;
  static const int buff_c_max=64;
  static const int buff_r_max=64;
  
  
  int k_max;
  int n_max;
  int m_max;
  int c_max;
  int r_max;
  
  int cols[buff_m_max];
  int rows[buff_m_max];
  
  short c[buff_k_max][buff_n_max];
  short r[buff_k_max][buff_n_max];
  short m[buff_k_max][buff_n_max];
  
  short k[buff_m_max][buff_c_max][buff_r_max];
  short n[buff_m_max][buff_c_max][buff_r_max];
  
  void inverse_map(void) {
    for(int ki=0;ki<buff_k_max;ki++) {
      for(int ni=0;ni<buff_n_max;ni++) {
	m[ki][ni] = buff_m_max-1;
	c[ki][ni] = 0;
	r[ki][ni] = 0;
      }
    }	
    
    for(int im=0;im<mats;im++) {
      for(int ic=0;ic<cols[im];ic++) {
	for(int ir=0;ir<rows[im];ir++) {
	  c[k[im][ic][ir]][n[im][ic][ir]] = ic;
	  r[k[im][ic][ir]][n[im][ic][ir]] = ir;
	  m[k[im][ic][ir]][n[im][ic][ir]] = im;
	}
      }
    }
  };
  
  TMimosa24_25Map (const char *chip) {
    if(strcmp(chip,"Mi24")==0) {
      m_max=12; c_max=63; r_max=63; n_max=4095; k_max =3;
      mats=13; 
      for(int im=0;im<mats;im++) { cols[im]=64;}  cols[6]=cols[7]=cols[11]=32;
      for(int im=0;im<6;im++) { if((im%2)==0) rows[im]=31; else rows[im]=32;}
      for(int im=6;im<8;im++) { if((im%2)==0) rows[im]=15; else rows[im]=16;}
      
      for(int im=8;im<13;im++) { rows[im]=1;}
      
      for(int im=0;im<8;im++) for(int ic=0;ic<cols[im];ic++) for(int ir=0;ir<rows[im];ir++) k[im][ic][ir]=int(im/2);			
      for(int im=8;im<13;im++) for(int ic=0;ic<cols[im];ic++) for(int ir=0;ir<rows[im];ir++) k[im][ic][ir]=im-8;
      
      for(int ix=0;ix<64;ix++) {
	n[12][0][ix]=0;
      }
      
      for(int im=8;im<12;im++) {
	int index = 0;
	for(int ix=0;ix<64;ix++) {
	  if(im==11) { if(ix<32) n[im][ix][0] = index; }
	  else n[im][ix][0] = index;
	  index++;
	}
      }
      
      int index = 64;
      for(int iy=1;iy<64;iy++) {
	for(int ix=0;ix<64;ix++) {
	  if(iy<32) n[0][ix][iy-1] = index; else n[1][ix][iy-32] = index;
	  if(iy<32) n[2][ix][iy-1] = index; else n[3][ix][iy-32] = index;
	  if(iy<32) n[4][ix][iy-1] = index; else n[5][ix][iy-32] = index;
	  if(iy<16) {
	    if (ix<32) n[6][ix][iy-1] = index;
	  }
	  else if(iy<32) {
	    if (ix<32) n[7][ix][iy-16] = index;
	  }
	  else {
	  }	
	  index++;
	}
      }
    } 
    
    else if(strcmp(chip,"Mi25A_20um")==0 || strcmp(chip,"Mi25B_20um")==0 || strcmp(chip,"Mi25A_30um")==0 || strcmp(chip,"Mi25B_30um")==0 || strcmp(chip,"Mi25A_40um")==0 || strcmp(chip,"Mi25B_40um")==0  ) {
      m_max=2; c_max=15; r_max=31; n_max=191; k_max =7;
      mats=3;
      cols[0]=cols[1]=cols[2]=16;
      
      if(strcmp(chip,"Mi25A_20um")==0 || strcmp(chip,"Mi25B_20um")==0 ) { rows[0]=rows[1]=rows[2]=32; }
      if(strcmp(chip,"Mi25A_30um")==0 || strcmp(chip,"Mi25B_30um")==0 ) { rows[0]=rows[2]=21; rows[1]=22; }
      if(strcmp(chip,"Mi25A_40um")==0 || strcmp(chip,"Mi25B_40um")==0 ) { rows[0]=rows[1]=rows[2]=16; }
      
      int row_offset1 = 0, row_offset2 = 0;
      for(int im=0;im<mats;im++) { row_offset2+=rows[im];}
      for(int im=0;im<mats;im++) {
	for(int ic=0;ic<8;ic++) { 
	  int ind = ic-4; if(ic<4) ind = ic+4;
	  for(int ir=0;ir<rows[im];ir++) {
	    k[im][ic][ir] =  ind ;
	    n[im][ic][ir] = ir + row_offset1; //orig
	    //n[im][ic][ir] = ir + row_offset2; //rita
	  }
	}
	for(int ic=8;ic<16;ic++) {
	  int ind = ic-8; if(ic>=12) ind = ic-8;
	  for(int ir=0;ir<rows[im];ir++) {
	    k[im][ic][ir] =  ind; 
	    n[im][ic][ir] = ir + row_offset2; //orig
	    //n[im][ic][ir] = ir + row_offset1; //rita 
	  }
	}
	row_offset1+=rows[im];
	row_offset2+=rows[im];
      }		
      
    }
    else {printf("TMimosa24_25Map::Unknown chip name:%s \n", chip); exit(-1);}
    inverse_map();
  };
  
  ~TMimosa24_25Map () {
  };
  
  void print_map(int no) {
    for(int im=0;im<mats;im++) {
      printf("Matrix No%d : [columns][rows]=[%d][%d] \n", im, cols[im], rows[im]);
      printf("------------------------------------------------------------ \n");
      for(int ir=0;ir<rows[im];ir++) {
	for(int ic=0;ic<cols[im];ic++) {
	  if(no==2) printf("%04d,%02d ", n[im][ic][ir], k[im][ic][ir]);
	  else printf("%04d ", n[im][ic][ir]);
	}
	printf("\n");
      }
      printf("\n\n");
    }
    
    
    /*		printf("Readout: [k][n]=[%d][%d] \n", 4, 4095);
		printf("------------------------------------------------------------ \n");
		
		for(int ni=0;ni<4096;ni++) {
		for(int ki=0;ki<4;ki++) {
		printf("%02d,%04d  %02d  %04d,%04d\n",ki, ni, m[ki][ni], c[ki][ni], r[ki][ni]);
		}
		
		}
    */
    
    
  }
  
  int get_submatrix(int i1, int i2) {
    int index = m[i1][i2];
    return index;
  }

  int get_row(int i1, int i2) {
    int index = r[i1][i2];
    return index;
  }

  int get_column(int i1, int i2) {
    int index = c[i1][i2];
    return index;
  }


};

#endif

