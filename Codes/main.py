import plotly.graph_objects as go
import numpy as np
import os

directory="/home/www-data/html"
lis=[]
cnt=1

left=-500
right=500
prec=100;

Scatters=[]

all_fig=go.Figure()
best_fig=go.Figure()

X=np.linspace(left,right,int( (right-left)*prec+1 ) )

with open("points.txt","r") as f:
    for i in f.read().strip().split(sep="\n"):
        lis.append([ float(i.split()[0]) , float(i.split()[1]) ])

cnt=1
for i in lis:
    Scatter=go.Scatter(
        mode="markers",
        x=[i[0]],
        y=[i[1]],
        name="Point {}".format(cnt),
        marker=dict(
            color="black",
            size=14
        ),
        showlegend=False
    )
    Scatters.append(Scatter)
    cnt+=1

cnt=1
n0=0
width=0
zip=[]

with open("zip.txt","r") as f:
    for i in f.read().strip().split(sep="\n"):
        if(cnt==1):
            n0=float(i.split()[0])
            width=float(i.split()[1])
        else:
            zip.append([ float(i.split()[0]) , float(i.split()[1]) ])
        cnt+=1

cnt=1

for i in zip:
    Scatter=go.Scatter(
        mode="markers",
        x=[i[0]],
        y=[i[1]],
        name="Zip {}".format(cnt),
        marker=dict(
            color="red",
            size=10
        ),
        showlegend=False
    )
    Scatters.append(Scatter)
    cnt+=1

cnt=1
start=0
diff=0
error=0
min_error=-5

for i in Scatters:
    all_fig.add_trace(i)
    best_fig.add_trace(i)

all_fig.layout.xaxis.zerolinecolor="blue"
all_fig.layout.yaxis.zerolinecolor="blue"
all_fig.layout.yaxis.range=[-150,150]
best_fig.layout.xaxis.zerolinecolor="blue"
best_fig.layout.yaxis.zerolinecolor="blue"
best_fig.layout.yaxis.range=[-150,150]

best_Scatter=0

with open ("formulas.txt","r") as f:
    for temp in f:

        def func(n):            
            n=eval(temp)
            return n

        if(cnt%2==1):
            error=float(temp.split()[0])
            start=int(temp.split()[1])
            diff=int(temp.split()[2])

        else:
            print("Number: {}".format(cnt//2))
            ys=[]
            for i in X:
                x2= (i-n0+width)/width
                x3= (x2-start+diff)/diff
                ys.append(func(x3)) 

            Scatter=go.Scatter(
                mode="lines",
                x=X,
                y=np.array(ys),
                name="Number: {}".format(cnt//2),
                marker=dict(color="green"),   
                showlegend=True
            )
            os.system("mkdir -p "+directory+"/Number_{}".format(cnt//2))
            fig=go.Figure()
            fig.layout.xaxis.zerolinecolor="blue"
            fig.layout.yaxis.zerolinecolor="blue"
            fig.layout.yaxis.range=[-150,150]

            for i in Scatters:
                fig.add_trace(i)
            fig.add_trace(Scatter)

            fig.write_html(directory+"/Number_{}".format(cnt//2)+"/index.html")
            all_fig.add_trace(Scatter)

            if(min_error<0 or error<min_error):
                min_error=error
                best_Scatter=Scatter
        cnt+=1

best_fig.add_trace(best_Scatter)

os.system("mkdir -p "+directory+"/All_Formulas")
os.system("mkdir -p "+directory+"/Best_Formula")

all_fig.write_html(directory+"/All_Formulas/index.html")
best_fig.write_html(directory+"/Best_Formula/index.html")