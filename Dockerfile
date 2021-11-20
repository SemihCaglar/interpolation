FROM httpd:alpine
RUN apk add --no-cache --upgrade python3 py3-pip py3-numpy &&\
    python3 -m pip install --no-cache-dir --upgrade plotly &&\
    wget -qO- https://bin.equinox.io/c/4VmDzA7iaHb/ngrok-stable-linux-amd64.tgz | tar xvzf - -C /usr/local/bin &&\
    ngrok authtoken 1zElGTHFkqqUX7IV7yzuiATZ7qg_3jLRGEgH5rVCLmo2MU853 &&\
    echo "web-addr: 0.0.0.0:4040" >> ~/.ngrok2/ngrok.yml
ADD Codes "/home/www-data/Codes"
COPY httpd.conf "/usr/local/apache2/conf"
EXPOSE 80 4040
CMD [ "sh", "-c", "cd /home/www-data/Codes && ./main.out && python3 main.py && httpd && ngrok http 80" ]