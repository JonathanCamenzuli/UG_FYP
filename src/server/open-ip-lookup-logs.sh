##
# open-coap-logs.sh
#
# Jonathan Camenzuli
#
# Bash Script For Opening IP Lookup Server Logs
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# 21/05/2023
#
#

clear

# Get ID of Container
coap_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model_ip-lookup')

# Open Container Logs
docker logs -f $coap_id