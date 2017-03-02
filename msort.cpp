#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <mpi.h>

using namespace std;

void merge(int *a,int *b,int l,int mid,int r);
void mergeSort(int *a,int *b,int l,int r);

int main(int argc,char** argv){		
    int num = 5;          //默认为5个数
    int *array_ori = new int[num];	
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
    MPI_Scatter(array_ori,size,MPI_INT,array_sub,size,MPI_INT,0, MPI_COMM_WORLD);	

    int *array_tmp = new int[size];
    mergeSort(array_sub,array_tmp,0,(size-1));

    int *sorted = NULL;
    if (ProcessID == 0) {		
	sorted = new int[num];		
    }	
    MPI_Gather(array_sub,size,MPI_INT,sorted,size,MPI_INT,0,MPI_COMM_WORLD);

    if (ProcessID == 0) 
      {		
	int *array_other = new int[num] ;
	mergeSort(sorted,array_other,0,(num-1));
	cout<<"sorted array:\n";
	for(int i=0;i<num;i++){			
	    cout << sorted[i]; 			
	}	
	cout<<"\n\n\n";
	free(sorted);
	free(array_other);
      }
    free(array_ori);
    free(array_sub);
    free(array_tmp);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void merge(int *a,int *b,int l,int mid,int r){
    int i,j,k,p;
    i = l;
    j = mid+1;
    p = l;
    while ((p<=mid) && (j<=r))
      {
	if (a[p]<=a[j])
	  {
	    b[i]=a[p];
	    p++;
	  }
	else
	  {
	    b[i]=a[j];
	    j++;
	  }
	i++;
      }

    if (mid<p)
      {
	for (k=j;k<=r;k++)
	  {
	    b[i]=a[k];
	    i++;
	  }
      }
    else
      {
	for (k=p;k<=mid;k++)
	  {
	    b[i]=a[k];
	    i++;
	  }
      }

    for (k=l;k<=r;k++)
      {
        a[k]=b[k];
      }
}

void mergeSort(int *a,int *b,int l,int r){
    int mid;
    if (l<r) 
      {
	mid = (l+r) / 2;
	mergeSort(a,b,l,mid);
	mergeSort(a,b,(mid+1),r);
	merge(a,b,l,mid,r);
      }
}
