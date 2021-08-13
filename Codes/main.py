import plotly.graph_objects as go
import numpy as np
import plotly.io as pio

pio.renderers.default='browser'

lis=[]
t=1
with open("in.txt","r") as f:
    for i in f.read().strip().split():
        if(t==2):
            check=float(i)
        else:
            lis.append(float(i))
        t+=1

trues=[]
cnt=1
print("Check={}".format(check))
#check=float(input("Check="))

with open ("out.txt","r") as f:
    for temp in f:
        if(temp=="NO\n" or temp=="\n"):
            continue
        s=""
        for i in temp:
            if(i=='^'):
                s+='**'
            else:
                s+=i
        def func(n):
            n=eval(s)
            return n
        
        t=True
        for i in range(1,int(lis[0])+1):
            if(abs(lis[i]-func(i))>=check):
                t=False
                #
                break
        if(t):
            trues.append([s,cnt])
        cnt+=1

left=float(input("Left="))
right=float(input("Right="))
prec=float(input("Prec="))

fig=go.Figure()
X=np.linspace(left,right,int( (right-left)*prec+1 ) )
for i in trues:
    print(i[1])
    def func(n):
        return eval(i[0])
    ys=[]
    for j in X:
        ys.append(func(j))
    fig.add_trace(go.Scatter(x=X, y=np.array(ys),mode="lines",name=str(i[1]) ) ) 

fig.layout.xaxis.zerolinecolor="palevioletred"
fig.layout.yaxis.zerolinecolor="palevioletred"
fig.write_image("fig.png")
pio.show(fig)