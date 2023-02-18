docker network create fyp_smart_city_model
docker volume create influxdb_volume
docker volume create grafana_volume
docker-compose up --detach