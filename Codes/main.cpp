#include<bits/stdc++.h>
using namespace std;

typedef pair<double,double> dd;
vector<dd> pts;
int compress_size=15;
double width;
double n0;
double min_error;
double aralik,ilk; 

typedef struct term{  // (a/b)*(n^c)*(d^n)
    term(double a, double b, int c, int d){
        this->a=a,this->b=b,this->c=c,this->d=d;
    }
    double a,b;
    int c,d;
}term;

vector<term> bestformula;

typedef struct matrix{
    vector<double> ar;
    int sp;
    int size;
    struct matrix *next;
    struct matrix *previous;   
}matrix;

void cmatrix(vector<double>::iterator ar,vector<double>::iterator end){
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

void customs(vector<double> &ar,int n,vector<int> sps,int nsps,int n_c,int c_n,int c){
    for(int i=0;i<n;++i)
        ar[i]=pow(i+1,n_c)*pow(c_n,i+1)*c;
    cmatrix(ar.begin(),ar.begin()+n);

    for(int i=0;i<nsps;i++)
        cmatrix(ar.begin()+sps[i],ar.begin()+n);
}

void solve(matrix *p,int n){
    int nsps=0,t=0;
    vector<int> sps(n);
    vector<double> ar(n);

    while(p!=NULL){
        if(p->sp!=p->size)
            sps[nsps++]=t+p->sp;
        for(int i=0;i<min(p->size,p->sp);++i){
            ar[t++]=p->ar[i];
        }
        p=p->next;
    }
    t--;
    while(t>-1 and !ar[t])
        t--;

    vector<term> formula;
    formula.push_back(term(0.0,0.0,ilk,aralik));

    while(t>-1){
        int spsi=upper_bound(sps.begin(),sps.begin()+nsps,t)-sps.begin(), c_n=max(1,spsi+1), n_c=spsi ? max(t-sps[spsi-1],0) : t;
        vector<double> temp(t+1);
        customs(temp,t+1,sps,nsps,n_c,c_n,1);
        formula.push_back(term(ar[t],temp[t],n_c,c_n));

        for(int i=0;i<t+1;++i)
            ar[i]-=temp[i]*ar[t]/temp[t];
        while(t>-1 && !ar[t])
            t--;
    }
    double error=0;
    for(int i=0;i<pts.size();++i){
        double x1=pts[i].first;
        double x2 = ( (x1-n0)+width )/(width);
        double x3 = (x2-ilk+aralik)/aralik;

        double sum=0;
        for(int j=1;j<formula.size();j++)
            sum += formula[j].a * pow(x3,formula[j].c) * pow(formula[j].d,x3) / formula[j].b;
        error+=(pts[i].second-sum)*(pts[i].second-sum);
    } 
    if(min_error<0 || error<min_error){
        min_error=error;
        bestformula=formula;
    }

}

matrix *brute(matrix *t,int mode=0,int first=0){
    if(t->size==1){
        if(mode){
            int size=1;
            while(t->previous!=NULL)
                t=t->previous,size+=t->size;
            solve(t,size);
        }
        return NULL;
    }
    cmatrix(t->ar.begin(),t->ar.begin()+t->size);

    bool found=0;
    int i=t->size-2;
    while(!found and i>(-1+first)){
        t->sp=i;
        t->next=new matrix;
        t->next->next=NULL;
        t->next->previous=t;
        t->next->size=t->size-i;
        t->next->ar.clear();
        t->next->ar=vector<double>(t->next->size);
        t->next->sp=t->next->size;

        for(int i=0;i<t->next->ar.size();++i)
            t->next->ar[i]=t->ar[i+t->sp];

        t->next=brute(t->next,mode,1);
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

void find_formulas(int n,vector<double> ar){
    matrix a={.sp=n,.size=n};
    a.ar=ar;
    brute(&a,1,0);
}


vector<double> compress(vector<dd> ar){
    if(ar.size()<compress_size)
        compress_size=ar.size();

    sort(ar.begin(),ar.end());
    width=(ar.rbegin()->first - ar.begin()->first)/(compress_size-1);
    n0=ar.begin()->first;

    double left = ar.begin()->first - width/2, right=left+width, sum=0;
    int cnt=0,range=0,ind=0;

    vector<bool> not_found(ar.size(),false);
    vector<double> ans(compress_size);

    while(ind<ar.size()){
        if(ar[ind].first>=right){
            left=right;
            right=left+width;
            if(cnt==0)
                not_found[range]=true;
            else
                ans[range]=sum/cnt;
            sum=0;
            cnt=0;
            range++;
        }
        else{
            sum+=ar[ind].second;
            cnt++;
            ind++;
        }
    }
    if(cnt==0)
        not_found[range]=true;
    else
        ans[range]=sum/cnt;
    
    int near_right=-1;
    for(int i=1;i+1<compress_size;i++){
        if(not_found[i]==true){
            if(i>near_right){
                int j;
                for(j=i;j<ar.size() && not_found[j]==true;j++);
                near_right=j;
            }
            ans[i] = ans[i-1] + (ans[near_right]-ans[i-1]) / (near_right-i+1) ;
            not_found[i]=false;
        }
    }

    return ans;
}

int main(){
    freopen("/home/semih/Desktop/Proj/test/test.in","r",stdin);

    cout<<"N ve sayilari girin:"<<endl;
    int n; 
    cin>>n;
    pts.resize(n);
    for(int i=0;i<n;++i){
        double x,y;
        cin>>x>>y;
        pts[i]={x,y};
    }
    FILE *fp=fopen("points.txt","w");
    for(int i=0;i<pts.size();i++)
        fprintf(fp,"%f %f\n",pts[i].first,pts[i].second);
    fclose(fp);
    
    vector<double> zip = compress(pts);

    fp=fopen("zip.txt","w");
    fprintf(fp,"%f %f\n",n0,width);
    for(int i=0;i<zip.size();i++)
        fprintf(fp,"%f %f\n",n0+i*width,zip[i]);
    fclose(fp);

    fp=fopen("formulas.txt","w");

    for(int num=1;num<=compress_size;num++){
        min_error=-5;
        for(int diff=1;diff<=(compress_size)/(num)+1;diff++)
            for(int start=0;start<=compress_size-diff*(num-1)-1;start++){
                vector<double> ar(num);
                for(int i=0;i<num;++i)
                    ar[i]=zip[start+i*diff];
                ilk=start+1,aralik=diff;
                find_formulas(num,ar);
            }
        fprintf(fp,"%f %d %d\n", sqrt(min_error/(n-1)), bestformula[0].c, bestformula[0].d);
        for(int i=1;i<bestformula.size();i++)
            fprintf(fp,"+(%f/%f)*(n**%d)*(%d**n) ",bestformula[i].a,bestformula[i].b,bestformula[i].c,bestformula[i].d);
        fprintf(fp,"\n");
    }
    fclose(fp);

    return 0;
}