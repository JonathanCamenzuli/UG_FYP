docker network create fyp_docker_network
docker volume create influxdb_volume
docker volume create grafana_volume
docker-compose run --detach -p 5683:5683/udp coap-server
docker-compose run --detach -p 8086:8086/tcp influxdb
docker-compose run --detach -p 3001:3001/tcp grafana