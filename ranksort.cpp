#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <mpi.h>

using namespace std;

void rankSort(int *a,int *b,int t,int r,int n);

int main(int argc,char** argv){		
    int num = 8;
    int *array_ori = new int[num];
    int *array_pos = new int[num];	
    int ProcessID;
    int ProcessSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&ProcessID);
    MPI_Comm_size(MPI_COMM_WORLD,&ProcessSize);

    if (ProcessID == 0)
      {
	cout << "array before sorted:\n";
        ifstream fin("input.txt");
	for (int i=0;i<num;i++)
	  {
             fin >> array_ori[i];
	     cout << array_ori[i];				
	  }
	cout << "\n";
      }

    int size = num / ProcessSize;	
    int *array_sub = new int[size];
    MPI_Bcast(array_ori,num,MPI_INT,0, MPI_COMM_WORLD);	
    rankSort(array_ori,array_sub,ProcessID*size,size,num);  
    int *sorted = NULL;
    if (ProcessID == 0) {		
	sorted = new int[num];		
    }	
    
    MPI_Gather(array_sub,size,MPI_INT,array_pos,size,MPI_INT,0,MPI_COMM_WORLD);
    if (ProcessID == 0) 
      {		
        for (int i=0;i<num;i++){
            sorted[array_pos[i]]=array_ori[i];
        }	
        
	cout<<"sorted array:\n";
	for(int i=0;i<num;i++){			
	    cout << sorted[i]; 			
	}	
	cout<<"\n\n\n";
	delete sorted;
        delete array_ori;
      }
    delete array_sub;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void rankSort(int *a,int *b,int t,int r,int n){
    int i,j,k;
    for (i=t;i<t+r;i++){	
        k = 0;
        for (j=0;j<n;j++){
      	   if (a[i]>a[j]) k++;	
	   if ((a[i] == a[j]) && (i>j)) k++;			
	}
        b[i-t] = k;
    }
}
