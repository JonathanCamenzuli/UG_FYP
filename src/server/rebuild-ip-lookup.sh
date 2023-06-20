##
# rebuild-ip-lookup.sh
#
# Jonathan Camenzuli
#
# Bash Script Which Rebuilds IP Lookup Server
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# 20/06/2023
#
#

# Get ID of container
ip_lookup_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model-ip-lookup')

# Stop Container
docker stop --time 1 $ip_lookup_id

# Remove Continer
docker rm -f $ip_lookup_id

# Remove Image
docker rmi -f fyp_smart_city_model-ip-lookup

# docker-build routine for IP Lookup server
docker compose run --detach -p 80:80/tcp ip-lookup
new_ip_lookup_id=$(docker ps --quiet --filter 'name=^fyp_smart_city_model-ip-lookup')
docker stop --time 0 $new_ip_lookup_id
docker update --restart always $new_ip_lookup_id
docker start $new_ip_lookup_id