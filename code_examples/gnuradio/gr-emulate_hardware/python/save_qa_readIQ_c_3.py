#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2013 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import emulate_hardware_swig as emulate_hardware
import numpy as np
import struct
#from matplotlib import pyplot as plt

class qa_readIQ_c (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
	# set up fg
	# read data from flowchart
	#size = 1000
	#filepath = "/home/stefan/src/read_binary/IQ_Data.bin"
	#src = emulate_hardware.readIQ_c(filepath)
	#head = blocks.head(8,size)
	#snk = blocks.vector_sink_c()
	#self.tb.connect(src,head,snk)
        #self.tb.run ()
	#test = snk.data()
	#test_real = []
	#test_imag = []
	#if 0:
        #	for i in range(len(test)):
	#		#print test[i]
	#		test_real.append(test[i].real)
	#		test_imag.append(test[i].imag)
	#	plt.plot(range(len(test)),test_real)
	#	#plt.plot(range(len(test)),test_imag)
	#	plt.show()
	# read data from python and compare
	#file = open(filepath, mode='rb')
	#for i in range(size):
	#	bin = np.complex64(np.float32(struct.unpack('d', file.read(8)))+1j*np.float32(struct.unpack('d',file.read(8))))
		#self.assertEqual(bin,test[i])
	#	print snk.data()[i]

if __name__ == '__main__':
    gr_unittest.run(qa_readIQ_c, "qa_readIQ_c.xml")
