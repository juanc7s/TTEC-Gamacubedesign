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
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, log_file='log.txt', clear_current=False):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='Data Logger',   # will show up in GRC
            in_sig=[
                # np.uint8,
                # dict,
            ],
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

        # self.receiving_message = ""#np.ndarray(0, dtype=np.uint8)
        # self.stack_size = 0
        # self.received_message = ""
        # self.message_received = False
    
    def message_handler(self, msg):
        entry = str(msg)
        with open(self.log_file, 'a') as f:
            if not self.first_write:
                f.write('\n')
            else:
                self.first_write = False
            f.write(entry)

    def work(self, input_items, output_items):
        """example: multiply with constant"""
        # msg = ""
        # for c in input_items[0].tobytes():
        #     msg += chr(c)
        # print(msg)
        # print(len(input_items[0]))
        # input_items[0] = np.array([])
        # self.receiving_message.append(input_items[0])
        # for c,n in zip(input_items[0].tobytes(),range(len(input_items[0]))):
        #     if c == '\n':
        #         self.received_message = self.receiving_message
        #         self.stack_size -= n
        #         self.message_received = True
        #         self.receiving_message = ""
        #     else:
        #         self.receiving_message += chr(c)
        #         self.stack_size += 1
        #         print(c)
        # if self.message_received:
            # message = input_items[0]
            # crc_check = input_items[1]
            # kv = json.loads(self.received_message[6:])
            # for k,v in zip(kv.keys(), kv.values()):
            #     print(k + ":" + v)
            # self.message_received = False
        
        # return len(output_items[0])
        return 0
