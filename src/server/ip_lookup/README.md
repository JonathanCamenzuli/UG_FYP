# IP Lookup Server

The source code for the IP Lookup server is devised as follows:

- `lookup.py`: Initialises the HTTP Server which provides the server's public IP address. This source file was set as the Entrypoint when the container is initiated 

## Libraries and Dependencies
- [Python Slim 3.7 Docker Image](https://hub.docker.com/layers/library/python/3.7-slim/images/sha256-05be27b7e8722b0dfd691d8352e7b7c6bedacc8c43343711db98bd3b1a2d8177?context=explore) by The Docker Community
- [requests](https://github.com/psf/requests) by [Python Software Foundation](https://github.com/psf)
- [flask](https://github.com/pallets/flask) by [Pallets Projects](https://github.com/pallets)
- [python-ipify](https://github.com/rdegges/python-ipify) by [Randall Degges](https://github.com/rdegges)
- Other dependencies which are used by the above packages can be found in `./requirements.txt`