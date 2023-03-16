##
# open-coap-logs.sh
#
# Jonathan Camenzuli
#
# Bash Script For Opening CoAP Server Logs
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# 16/03/2023
#
#

clear

# Get ID of Container
coap_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_coap-server')

# Open Container Logs
docker logs -f $coap_id