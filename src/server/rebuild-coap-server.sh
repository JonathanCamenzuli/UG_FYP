##
# rebuild-coap-server.sh
#
# Jonathan Camenzuli
#
# Bash Script Which Rebuilds CoAP Server
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# 16/03/2023
#
#

# Get ID of container
coap_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_coap-server')

# Stop Container
docker stop --time 1 $coap_id

# Remove Continer
docker rm $coap_id

# Remove Image
docker image rm fyp_smart_city_model-coap-server

# docker-build routine for CoAP server
docker compose run --detach -p 5683:5683/udp coap-server
new_coap_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_coap-server')
docker stop --time 0 $new_coap_id
docker update --restart always $new_coap_id
docker start $new_coap_id