#include<bits/stdc++.h>
using namespace std;

typedef struct matrix{
    double *ar;
    int sp;
    int size;
    struct matrix *next;
    struct matrix *previous;   
}matrix;

void cmatrix(double *ar,double *end){
    double k;
    int n=end-ar;
    for(int i=1;i<n;ar[n-1]=k,i++){
        k=ar[i-1];

        for(int j=i;j<n;j++){
            ar[j-1]=ar[j]-ar[j-1];
            swap(k,ar[j-1]);
        }
    }
}

void customs(double *ar,int n,int *sps,int nsps,int n_c,int c_n,int c){
    for(int i=0;i<n;++i)
        ar[i]=pow(i+1,n_c)*pow(c_n,i+1)*c;
    cmatrix(ar,ar+n);

    for(int i=0;i<nsps;i++)
        cmatrix(ar+sps[i],ar+n);
}

void solve(matrix *p,int n){
    int sps[n],nsps=0,t=0;
    double ar[n];

    while(p!=NULL){
        // if(p->sp==0)
            // printf("| ");
        if(p->sp!=p->size)
            sps[nsps++]=t+p->sp;
        for(int i=0;i<min(p->size,p->sp);++i){
            // printf("%g ",p->ar[i]);
            // printf( (i+1==p->sp && i+1!=p->size)?"| ":"");
            ar[t++]=p->ar[i];
        }
        p=p->next;
    }
    // printf("\n");
    t--;
    while(t>-1 and !ar[t])
        t--;
    while(t>-1){
        int spsi=upper_bound(sps,sps+nsps,t)-sps, c_n=max(1,spsi+1), n_c=spsi ? max(t-sps[spsi-1],0) : t;
        double temp[t+1];
        customs(temp,t+1,sps,nsps,n_c,c_n,1);
        printf("+(%f/%f)*(n^%d)*(%d^n) ",ar[t],temp[t],n_c,c_n);
        for(int i=0;i<t+1;++i)
            ar[i]-=temp[i]*ar[t]/temp[t];
        while(t>-1 && !ar[t])
            t--;
    }
    printf("\n");
    // printf("\n");
}

matrix *brute(matrix *t,double epsilon,int mode=0,int first=0){
    if(t->size==1){
        if(mode){
            int size=1;
            while(t->previous!=NULL)
                t=t->previous,size+=t->size;
            solve(t,size);
        }
        return NULL;
    }
    cmatrix(t->ar,t->ar+t->size);

    if(t->ar[t->size-1]<=epsilon && t->ar[t->size-1]>=-epsilon)
        return t;
    bool found=0;
    int i=t->size-2;
    while(!found and i>(-1+first)){
        t->sp=i;
        t->next=(matrix*)malloc(sizeof(matrix));
        t->next->next=NULL;
        t->next->previous=t;
        t->next->size=t->size-i;
        t->next->ar=(double*)malloc(sizeof(double)*(t->next->size));
        t->next->sp=t->next->size;

        memcpy(t->next->ar,t->ar+t->sp,sizeof(double)*t->next->size);
        t->next=brute(t->next,epsilon,mode,1);
        if(t->next!=NULL)
            found=1;
        i--;
    }
    if(mode){
        int size=0;
        t->sp=t->size;
        while(t->previous!=NULL)
            size+=t->size,t=t->previous;
        size+=t->size;
        solve(t,size);
    }
    if(!found){
        // free(t);
        return NULL;
    }
    return t;
}

int main(){
    freopen("/home/semih/Desktop/Proj/test/test.in","r",stdin);
    freopen("out.txt","w",stdout);

    int n;
    double epsilon;
    cin>>n>>epsilon;
    double *ar=(double*)malloc(n*sizeof(double));
    for(int i=0;i<n;++i)
        cin>>ar[i];
    matrix a={.sp=n,.size=n};
    a.ar=ar;

    double *ar2;
    ar2=(double*)malloc(n*sizeof(double));
    for(int i=0;i<n;++i)
        ar2[i]=ar[i];
    matrix *p=brute(&a,epsilon);
    int loop=(p==NULL);
    while(loop){
        // printf("Kararsiz veri\n");
        int extra;
        cin>>extra;
        if(!extra)
            break;
        n+=extra;
        ar=(double *)realloc(ar,n*sizeof(double));
        ar2=(double *)realloc(ar2,n*sizeof(double));
        for(int i=0;i<n;++i){
            if(i>=n-extra)
                cin>>ar2[i];
            ar[i]=ar2[i];
        }
        a.ar=ar;
        a.sp=a.size=n;
        p=brute(&a,epsilon);
        loop=(p==NULL);
    }
    
    FILE *fp=fopen("in.txt","w");
    fprintf(fp,"%d %.18f\n",n,epsilon);
    for(int i=0;i<n;++i)
        fprintf(fp,"%d %f\n",i+1,ar2[i]);
    fclose(fp);

    if(p==NULL){
        a.ar=ar2;
        brute(&a,epsilon,1,0);
        return 0;
    }
    solve(p,n);

    return 0;
}