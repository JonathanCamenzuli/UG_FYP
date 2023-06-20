##
# @file lookup.py
#
# @author Jonathan Camenzuli
#
# @brief Public IP Lookup
#
# Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
# "Miniature implementation of an IoT-based Smart City"
#
# @date 26/02/2022
#

import flask
import os
import time
from requests import get
from ipify import get_ip
from ipify.exceptions import ServiceError

# Setting Time Zone to CET
os.environ['TZ'] = 'Europe/Berlin'
time.tzset()

app = flask.Flask(__name__)

@app.route('/', methods=['GET'])
def get_public_ip():
    try:
        ip_address = get_ip()
        return ip_address
    except ServiceError:
        # Using another service in case ipify is down
        ip_address = get('https://ident.me/').text
        return ip_address

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80)