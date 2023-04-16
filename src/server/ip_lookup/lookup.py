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

import requests
import flask

app = flask.Flask(__name__)


@app.route('/', methods=['GET'])
def get_public_ip():
    ip_address = requests.get('https://api.ipify.org')
    return ip_address.text


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80)
