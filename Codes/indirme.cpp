#include<bits/stdc++.h>
using namespace std;

const int N=1e3;
const int indirgenecekSayi=10;
typedef pair<double,double> dd;
dd dizi[N];
int n;
double xgenislik;
double ans[indirgenecekSayi];
bool bulamadik[indirgenecekSayi];

int main(){
    freopen("/home/semih/Desktop/Proj/test/test.in","r",stdin);
    freopen("/home/semih/Desktop/Proj/test/test.out","w",stdout);
    cin>>n;
    for(int i=0;i<n;++i){
        double x,y;
        cin>>x>>y;
        dizi[i]={x,y};        
    }
    sort(dizi,dizi+n);
    xgenislik=(dizi[n-1].first-dizi[0].first)/indirgenecekSayi;

    double sol=dizi[0].first, sag=sol+xgenislik, sum=0;
    int cnt=0, aralik=0, ind=0;

    while(ind<n){
        if(dizi[ind].first>sag){
            sol=sag;
            sag=sol+xgenislik;
            if(cnt==0)
                bulamadik[aralik]=true;
            else
                ans[aralik]=sum/cnt;
            sum=0;
            cnt=0;
            aralik++;
        }
        else{
            sum+=dizi[ind].second;
            cnt++;
            ind++;
        }
    }

    if(cnt==0)
        bulamadik[aralik]=true;
    else
        ans[aralik]=sum/cnt;

    int enyakinsag=-1;

    for(int i=1;i+1<indirgenecekSayi;++i){
        if(bulamadik[i]==true){
            if(i>enyakinsag){
                int j;
                for(j=i;j<n and bulamadik[j]==true;++j);
                enyakinsag=j;
            }
            ans[i]=ans[i-1] + (ans[enyakinsag]-ans[i-1]) / (enyakinsag-i+1) ;
            bulamadik[i]=false;
        }
    }
    for(int i=0;i<indirgenecekSayi;++i)
        cout<<ans[i]<<"\n";
}