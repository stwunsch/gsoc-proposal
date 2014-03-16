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
import classify_swig as classify

class qa_person_vfi (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
	in_data = (0, 1, 7, 3, 3, 4, 2, 1, 2, 3)
	samp_block = len(in_data)
	src = blocks.vector_source_f(in_data)
	snk = blocks.vector_sink_i()
	stv = blocks.stream_to_vector(4, samp_block)
	test = classify.person_vfi(samp_block,0,0,0,0,0)
        self.tb.connect(src,stv,test,snk)
	self.tb.run ()
        # check data
	

if __name__ == '__main__':
    gr_unittest.run(qa_person_vfi, "qa_person_vfi.xml")
