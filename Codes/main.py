from colour import Color
import plotly.graph_objects as go
import numpy as np
# import plotly.io as pio

# pio.renderers.default='browser'

lis=[]
Nnumber=0
cnt=1
e=[]

# left=float(input("Left="))
# right=float(input("Right="))
# prec=float(input("Prec="))
left=0
right=10
prec=40

Nformula=0
fig=go.Figure()
X=np.linspace(left,right,int( (right-left)*prec+1 ) )
formulas=[]

with open("in.txt","r") as f:
    for i in f.read().strip().split(sep="\n"):
        if(cnt==1):
            Nnumber=int(i.split()[0])
            check=float(i.split()[1])
        else:
            lis.append([ float(i.split()[0]) , float(i.split()[1]) ])
        cnt+=1

cnt=1
for i in lis:
    Scatter=go.Scatter(
        mode="markers",
        x=[i[0]],
        y=[i[1]],
        name="Point {}".format(cnt),
        marker=dict(
            color="black",
            size=10
        ),
        showlegend=False
    )
    fig.add_trace(Scatter)
    cnt+=1

cnt=1

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
        formulas.append(s)
        Nformula+=1

        def func(n):
            n=eval(s)
            return n

        err=0.0
        t=True
        for i in lis:
            difference=i[1]-func(i[0])
            err+=difference*difference 
        e.append(err)

cnt=0
colors=list(Color("green").range_to(Color("red"),Nformula))

for temp in np.argsort(e):
    def func(n):
        n=eval(s)
        return n
    s=formulas[temp]
    ys=[]
    for j in X:
        ys.append(func(j))
    Scatter=go.Scatter(
        mode="lines",
        x=X,
        y=np.array(ys),
        name=str(e[temp]),
        marker=dict(color=colors[cnt].get_hex()),   
        showlegend=False
    )
    fig.add_trace(Scatter)
    if(cnt==0):
        fig.write_html("/home/www-data/html/Best Formula/index.html")
    cnt+=1

fig.layout.xaxis.zerolinecolor="blue"
fig.layout.yaxis.zerolinecolor="blue"
fig.layout.yaxis.range=[-150,150]
# fig.write_image("fig.png")
fig.write_html("/home/www-data/html/All Formulas/index.html")
# pio.show(fig)