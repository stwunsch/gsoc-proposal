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
from gnuradio import blocks, analog
import radar_swig as radar
from time import sleep
from matplotlib import pyplot as plt
import numpy as np

class qa_simulator_cc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		samp_rate = 20000
		packet_len = 2048
		freq = 0
		ampl = 1
		
		noise_ampl = 0.1
		
		test_len = packet_len*200
		rng = (10,10, 10)
		vlc = (5,-10, 0)
		rcs = (1e8,1e8, 1e8)
		center_freq = 6e9;
		
		src = radar.signal_generator_cw_c(packet_len,samp_rate,freq,ampl)
		head = blocks.head(8,test_len)
		sim = radar.simulator_cc(rng,vlc,rcs,samp_rate,center_freq)
		noise = analog.noise_source_c(analog.GR_GAUSSIAN,noise_ampl,0)
		add = blocks.add_cc()
		
		# estimator
		samp_compare = 10
		samp_protect = 0
		rel_threshold = 1
		avg_mult = 6
		
		dec_factor = 2
		
		mult = blocks.multiply_cc()
		dec = radar.decimator_cc(dec_factor)
		fft = radar.tsfft_cc()
		cfar = radar.os_cfar_c(samp_rate/dec_factor, samp_compare,samp_protect, rel_threshold, avg_mult)
		est_v = radar.est_v_cw(center_freq)
		# end: estimator
		
		snk = blocks.vector_sink_c()
		
		# connect
		self.tb.connect(src,head,sim)
		self.tb.connect((sim,0),(add,0))
		self.tb.connect((noise,0),(add,1))
		self.tb.connect((head,0),(mult,0))
		self.tb.connect((add,0),(mult,1))
		self.tb.connect(mult,dec,fft,cfar)
		self.tb.msg_connect(cfar,'cfar_out',est_v,'cw_in')
		
		self.tb.connect(fft,snk)
		
		# run
		self.tb.start()
		sleep(0.5)
		self.tb.stop()
		self.tb.wait()
		
		# check data
		data = snk.data()
		y = [0]*packet_len
		for k in range(packet_len):
			y[k] = pow(abs(data[k+0*packet_len/2]),2)
		x = [0]*packet_len
		for k in range(packet_len/2):
			x[k] = k*samp_rate/packet_len
			x[k+packet_len/2]=-samp_rate/2.0+k*samp_rate/packet_len
		plt.plot(x, y) # frequency on x, shifted
		#plt.plot(y) # samples on x, nonshifted
		plt.yscale('log')
		plt.title("FFT spectrum")
		#plt.show()

if __name__ == '__main__':
	gr_unittest.run(qa_simulator_cc)#, "qa_simulator_cc.xml")
