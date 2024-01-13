import json

def get_protocol():
    protocol = None
    with open("src/EByteSystem/GroundStationPy/Protocol.json", 'r') as f:
        protocol = json.load(f);
    return protocol

if __name__ == "__main__":
    protocol = get_protocol()
    print(json.dumps(protocol, sort_keys=True, indent=2))