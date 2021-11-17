#include<bits/stdc++.h>
using namespace std;

typedef pair<double,double> dd;
int compress_size=20;
double width;

vector<double> compress(vector<dd> ar){
    if(ar.size()<compress_size)
        compress_size=ar.size();

    sort(ar.begin(),ar.end());
    width=(ar.rbegin()->first - ar.begin()->first)/compress_size;

    double left=ar.begin()->first, right=left+width, sum=0;
    int cnt=0,range=0,ind=0;

    vector<bool> not_found(ar.size(),false);
    vector<double> ans(compress_size);

    while(ind<ar.size()){
        if(ar[ind].first>right){
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
    freopen("/home/semih/Desktop/Proj/test/test.out","w",stdout);

    int n;
    cin>>n;
    vector<dd> ar(n);
    for(int i=0;i<n;++i){
        double x,y;
        cin>>x>>y;
        ar[i]={x,y};        
    }
    vector<double> ans=compress(ar);
    for(int i=0;i<compress_size;++i)
        cout<<ans[i]<<"\n";
}