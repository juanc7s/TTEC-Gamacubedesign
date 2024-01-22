"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
import json
import pmt

from gnuradio import gr


class blk(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block"""

    def __init__(self, log_file='log.txt', clear_current=False):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='Data Logger',   # will show up in GRC
            in_sig=[],
            out_sig=[]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).

        self.message_port_register_in(pmt.intern("in"))
        self.set_msg_handler(pmt.intern("in"), self.message_handler)
        # self.message_port_register_out(pmt.intern("port name"))
        self.log_file = log_file
        self.first_write = True
        if clear_current:
            with open(self.log_file, 'w') as f:
                pass
        self.expected_telemetry_index = 0
    
    def message_handler(self, msg):
        entries = str(msg)
        with open(self.log_file, 'a') as f:
            for entry in entries.splitlines():
                if not self.first_write:
                    f.write('\n')
                else:
                    self.first_write = False
                f.write(entry)

                try:
                    tin = json.loads(entry)
                    telemetry = {
                        "type": tin["1"],
                        "message_1": tin["2"],
                        "main_temperature": tin["3"],
                        "message_2": tin["4"],
                        "batt_voltage": tin["5"],
                        "batt_current": tin["6"],
                        "batt_charge": tin["7"],
                        "batt_temperature": tin["8"],
                        "message_index": tin["9"],
                        "time": tin["10"],
                    }
                    print("")
                    for k,v in zip(telemetry.keys(), telemetry.values()):
                        print(k + ": " + str(v))
                    print("")
                    if self.expected_telemetry_index != telemetry["message_index"]:
                        f.write("\nTransmission " + str(self.expected_telemetry_index) + " lost!")
                        print("Transmission " + str(self.expected_telemetry_index) + " lost!")
                        self.expected_telemetry_index = telemetry["message_index"]+1
                    else:
                        self.expected_telemetry_index += 1
                except json.JSONDecodeError as e:
                    print(e)


    def work(self, input_items, output_items):
        return 0
