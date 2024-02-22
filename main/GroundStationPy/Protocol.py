import json

import sys

# workspace = sys.argv[1]

def get_protocol():
    protocol = None
    with open("main/GroundStationPy/Protocol.json", 'r') as f:
        protocol = json.load(f);
    return protocol

if __name__ == "__main__":
    protocol = get_protocol()
    print(json.dumps(protocol, sort_keys=True, indent=2))