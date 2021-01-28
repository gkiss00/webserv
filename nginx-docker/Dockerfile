FROM nginx
COPY YoupiBanane /tmp/www/YoupiBanane
COPY nginx.conf /etc/nginx/conf.d/default.conf
COPY ubuntu_cgi_tester /run/ubuntu_cgi_tester
RUN chmod 755 /run/ubuntu_cgi_tester