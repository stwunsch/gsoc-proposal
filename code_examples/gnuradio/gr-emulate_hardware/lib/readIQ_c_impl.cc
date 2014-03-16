/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "readIQ_c_impl.h"

namespace gr {
  namespace emulate_hardware {

    readIQ_c::sptr
    readIQ_c::make(std::string filepath)
    {
      return gnuradio::get_initial_sptr
        (new readIQ_c_impl(filepath));
    }

    /*
     * The private constructor
     */
    readIQ_c_impl::readIQ_c_impl(std::string filepath)
      : gr::sync_block("readIQ_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
		d_src = fopen(filepath.c_str(),"rb");
	}

    /*
     * Our virtual destructor.
     */
    readIQ_c_impl::~readIQ_c_impl()
    {
		fclose(d_src);
    }

    int
    readIQ_c_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
        d_items_read = 0;
        while(!feof(d_src) && d_items_read<noutput_items){ // if eof is false and there is free space in outputbuffer
			d_result = fread(d_buffer, 8, 3, d_src); // buffer[0] is crap, buffer[1] is real, buffer[2] is imag
			*out++ = gr_complex(d_buffer[1],d_buffer[2]);
			d_items_read++;
		}
		if(feof(d_src)){ // if eof is true
			for(int i=0; i<noutput_items; i++){
				*out++ = 0;
			}
			d_items_read = noutput_items;
		}
        /*
		if(!feof(d_src)){ // if eof is NOT reached, read file and push to output stream
			if(noutput_items<1024){ // if buffer takes noutput_items (<1024)
				d_result = fread(d_buffer, 8, (noutput_items/2)*2, d_src); // reduce noutput_items on item of *2
				d_items_read = (noutput_items/2)*2;
			}
			else{
				d_result = fread(d_buffer, 8, 1024, d_src);
				d_items_read = 1024; 
			}
			int i=0;
			while(i<d_items_read){
				*out++ = gr_complex(d_buffer[i],d_buffer[i+1]);
				i+=2;
			}
		}else{ // if eof reached, fill with 0 (no signal)
			for(int i=0; i<noutput_items; i++) *out++ = 0;
		}
		*/

        // Tell runtime system how many output items we produced.
        return d_items_read;
    }

  } /* namespace emulate_hardware */
} /* namespace gr */

