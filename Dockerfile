FROM httpd:alpine
RUN apk add --no-cache --upgrade openrc openssh git vim gcc g++ gdb make binutils python3 python3-dev py3-pip libquadmath libgfortran openblas &&\
    python3 -m pip install --no-cache-dir --upgrade pip setuptools wheel &&\
    python3 -m pip install --no-cache-dir --upgrade colour plotly numpy cython &&\
    mkdir -p "/home/www-data/html/All Formulas" "/home/www-data/html/Best Formula"
ADD Codes "/home/www-data/Codes"
COPY httpd.conf "/usr/local/apache2/conf"
EXPOSE 80
CMD [ "sh", "-c", "cd /home/www-data/Codes && make -s && httpd -D FOREGROUND" ]