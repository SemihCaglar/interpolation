import math
import plotly.graph_objects as go
import numpy as np
import os

directory="/home/semih/html"
lis=[]
cnt=1

Scatters=[]

all_fig=go.Figure()
best_fig=go.Figure()
err_fig=go.Figure()

with open("points.txt","r") as f:
    for i in f.read().strip().split(sep="\n"):
        lis.append([ float(i.split()[0]) , float(i.split()[1]) ])

points_max_x=lis[0][0]
points_min_x=lis[0][0]
points_max_y=lis[0][1]
points_min_y=lis[0][1]

for point in lis:
    points_max_x=max(points_max_x,point[0])
    points_min_x=min(points_min_x,point[0])
    points_max_y=max(points_max_y,point[1])
    points_min_y=min(points_min_y,point[1])

left=points_min_x-(points_max_x-points_min_x)/8
right=points_max_x+(points_max_x-points_min_x)/8
X=np.linspace(left,right,2000)

cnt=1
for i in lis:
    Scatter=go.Scatter(
        mode="markers",
        x=[i[0]],
        y=[i[1]],
        name="Point {}".format(cnt),
        marker=dict(
            color="MediumPurple",
            size=10
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
            size=11
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
all_fig.layout.yaxis.range=[points_min_y-(points_max_y-points_min_y)/8 , points_max_y+(points_max_y-points_min_y)/8]
all_fig.layout.xaxis.range=[left,right]
all_fig.update_xaxes(tickfont=dict(size=14), tickfont_family="Arial Black")
all_fig.update_yaxes(tickfont=dict(size=14), tickfont_family="Arial Black")

best_fig.layout.xaxis.zerolinecolor="blue"
best_fig.layout.yaxis.zerolinecolor="blue"
best_fig.layout.yaxis.range=[points_min_y-(points_max_y-points_min_y)/8 , points_max_y+(points_max_y-points_min_y)/8]
best_fig.layout.xaxis.range=[left,right]
best_fig.update_xaxes(tickfont=dict(size=14), tickfont_family="Arial Black")
best_fig.update_yaxes(tickfont=dict(size=14), tickfont_family="Arial Black")

best_Scatter=0

errors=[]

with open ("formulas.txt","r") as f:
    for temp in f:

        def func(n):            
            n=eval(temp)
            return n

        if(cnt%2==1):
            error=float(temp.split()[0])
            start=int(temp.split()[1])
            diff=int(temp.split()[2])
            if(error<=1.0):
                errors.append(0)
            else:
                errors.append(math.log2(error))

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
                showlegend=True
            )
            os.system("mkdir -p "+directory+"/Number_{}".format(cnt//2))
            fig=go.Figure()
            fig.layout.xaxis.zerolinecolor="blue"
            fig.layout.yaxis.zerolinecolor="blue"
            fig.layout.yaxis.range=[points_min_y-(points_max_y-points_min_y)/8 , points_max_y+(points_max_y-points_min_y)/8]
            fig.layout.xaxis.range=[left,right]
            fig.update_xaxes(tickfont=dict(size=14), tickfont_family="Arial Black")
            fig.update_yaxes(tickfont=dict(size=14), tickfont_family="Arial Black")

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

cnt=1

for i in errors:
    Scatter=go.Scatter(
        mode="markers",
        x=[cnt],
        y=[i],
        name="Error {}".format(cnt),
        marker=dict(
            color="red",
            size=10
        ),
        showlegend=True
    )
    err_fig.add_trace(Scatter)
    cnt+=1

for cnt in range(1,len(errors)):
    err_X=np.linspace(cnt,cnt+1,200)
    err_Y=[]
    for i in err_X:
        err_Y.append( errors[cnt] - (errors[cnt]-errors[cnt-1]) * (cnt+1-i) )
    
    Scatter=go.Scatter(
        mode="lines",
        x=err_X,
        y=np.array(err_Y),
        marker=dict(color="green"),   
        showlegend=False
    )
    err_fig.add_trace(Scatter)
    
err_fig.layout.xaxis.zerolinecolor="blue"
err_fig.layout.yaxis.zerolinecolor="blue"
err_fig.update_xaxes(tickfont=dict(size=14), tickfont_family="Arial Black")
err_fig.update_yaxes(tickfont=dict(size=14), tickfont_family="Arial Black")

os.system("mkdir -p "+directory+"/Errors")
err_fig.write_html(directory+"/Errors/index.html")