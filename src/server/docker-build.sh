##
# docker-build.sh
#
# Jonathan Camenzuli
#
# Bash Script Which Creates and Configures Docker Containers 
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# 08/03/2023
#
#

# NOTE: ONLY EXECUTE ONCE!

# Create Networks
docker network create fyp_backend
docker network create fyp_frontend
docker network create fyp_ip_lookup

# Create Volumes
docker volume create influxdb_volume
docker volume create grafana_volume

# Start containers
docker compose run --detach -p 5683:5683/udp coap-server
docker compose run --detach -p 8086:8086/tcp influxdb
docker compose run --detach -p 3001:3001/tcp grafana
docker compose run --detach -p 80:80 ip-lookup

# Get ID of containers
coap_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_coap-server')
influxdb_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_influxdb')
grafana_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_grafana')
iplookup_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_ip-lookup')

# Stop Containers
docker stop --time 0 $coap_id
docker stop --time 0 $influxdb_id
docker stop --time 0 $grafana_id
docker stop --time 0 $iplookup_id

# Update Restart Policy
docker update --restart always $coap_id
docker update --restart always $influxdb_id
docker update --restart always $grafana_id
docker update --restart always $iplookup_id

# Start Containers Again
docker start $coap_id
docker start $influxdb_id
docker start $grafana_id
docker start $iplookup_id