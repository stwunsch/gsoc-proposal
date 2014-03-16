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

from gnuradio import gr, gr_unittest, blocks
import radar_swig as radar

class qa_FMCW_generate_IQ_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
	src1 = blocks.vector_source_c( (complex(1,1),complex(2,2)) )
	src2 = blocks.vector_source_c( (complex(3,3),complex(4,4)) )
	src3 = blocks.vector_source_c( (complex(5,5),complex(6,6)) )
	test = radar.FMCW_generate_IQ_cc(2)
	snk = blocks.vector_sink_c()
	self.tb.connect(src1,(test,0))
	self.tb.connect(src2,(test,1))
	self.tb.connect(src3,(test,2))
	self.tb.connect(test,snk)
        self.tb.run ()
        # check data
	ref_data = ( complex(1,1),complex(2,2),complex(3,3),complex(4,4),complex(5,5),complex(6,6) )
	self.assertTupleEqual(ref_data,snk.data())

if __name__ == '__main__':
    gr_unittest.run(qa_FMCW_generate_IQ_cc, "qa_FMCW_generate_IQ_cc.xml")
