echo ___DOCKER_PULL_NGINX___
docker pull nginx

echo ___DOCKER_RMI___
docker rmi my_nginx

echo ___DOCKER_RM___
docker stop webserv-nginx
docker rm webserv-nginx

echo ___DOCKER_BUILD___
docker build -t my_nginx .

echo ___DOCKER_RUN___
docker run --name webserv-nginx -p 80:80 -d my_nginx