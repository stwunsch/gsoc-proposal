#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
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
import radar_swig as radar
import numpy as np

class qa_signal_generator_cw_c (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		packet_len = 1024
		samp_rate = 15000
		freq = 500
		ampl = 0.5
		
		test_len = packet_len*100
		
		src = radar.signal_generator_cw_c(packet_len,samp_rate,freq,ampl)
		head = blocks.head(8,test_len)
		snk = blocks.vector_sink_c()
		self.tb.connect(src,head,snk)
		self.tb.run ()
		
		# create ref data
		ref_data = [0]*test_len
		phase = 0
		for i in range(test_len):
			ref_data[i] = ampl*np.exp(1j*phase)
			phase = phase+2*np.pi*freq/samp_rate
			#phase = np.modf(phase,2*np.pi)
		
		# check data
		out_data = snk.data()
		self.assertEqual(len(out_data),test_len)
		for i in range(test_len):
			self.assertAlmostEqual(out_data[i],ref_data[i],1)


if __name__ == '__main__':
	gr_unittest.run(qa_signal_generator_cw_c, "qa_signal_generator_cw_c.xml")
