/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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
#include "os_cfar_c_impl.h"
#include <iostream>

namespace gr {
  namespace radar {

    os_cfar_c::sptr
    os_cfar_c::make(int samp_rate, int samp_compare, int samp_protect, float rel_threshold, float avg_mult)
    {
      return gnuradio::get_initial_sptr
        (new os_cfar_c_impl(samp_rate, samp_compare, samp_protect, rel_threshold, avg_mult));
    }

    /*
     * The private constructor
     */
    os_cfar_c_impl::os_cfar_c_impl(int samp_rate, int samp_compare, int samp_protect, float rel_threshold, float avg_mult)
      : gr::tagged_stream_block("os_cfar_c",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0), "packet_len")
    {
		d_samp_rate = samp_rate;
		d_samp_compare = samp_compare;
		d_samp_protect = samp_protect;
		d_rel_threshold = rel_threshold;
		d_avg_mult = avg_mult;
		
		// register output message port
		port_id = pmt::mp("cfar_out");
		message_port_register_out(port_id);
	}

    /*
     * Our virtual destructor.
     */
    os_cfar_c_impl::~os_cfar_c_impl()
    {
    }

    int
    os_cfar_c_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }

    int
    os_cfar_c_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];

        // Do <+signal processing+>
        
			// FIXME: value gets bigger with every cycle?
        float hld = 0;
        for(int k=0; k<ninput_items[0]; k++){
			hld += pow(abs(in[k]),2);
		}
		//std::cout << "---->fix: " << hld/(float)ninput_items[0] << std::endl;
        
        // OS-CFAR detection
        freq.clear();
        pks.clear();
        
        avg = 0;
        for(int k=0; k<ninput_items[0]; k++){ // calc average of input (spectrum)
			avg = avg+pow(abs(in[k]),2);
		}
		avg = avg/ninput_items[0];
		
        for(int k=0; k<ninput_items[0]; k++){ // go through input
			hold_samp.clear();
			for(int l=1; l<d_samp_compare+1; l++){ // go through samples to compare, hint: num of samp_compare is doubled! redef if too confusing
				if(k-l-d_samp_protect<0){
					hold_samp.push_back(0);
				}
				else{
					hold_samp.push_back(pow(abs(in[k-l-d_samp_protect]),2));
				}
				if(k+l+d_samp_protect>=ninput_items[0]){
					hold_samp.push_back(0);
				}
				else{
					hold_samp.push_back(pow(abs(in[k+l+d_samp_protect]),2));
				}
			}
			std::sort(hold_samp.begin(),hold_samp.end()); // sort sample vector
			if(pow(abs(in[k]),2)>hold_samp[(int)((2*d_samp_compare-1)*d_rel_threshold)] && pow(abs(in[k]),2)>d_avg_mult*avg){ // check if in[k] is over dynamic threshold and above average
				if(k<=ninput_items[0]/2) freq.push_back(k*d_samp_rate/(float)ninput_items[0]);
				else freq.push_back(-(float)d_samp_rate+k*d_samp_rate/(float)ninput_items[0]);
				pks.push_back(pow(abs(in[k]),2));
			}
		}
		
		// setup msg pmt
		ptimestamp = pmt::from_long(0); // FIXME: better timestamp! overflow of int?
		pfreq = pmt::init_f32vector(freq.size(),freq);
		ppks = pmt::init_f32vector(pks.size(),pks);
		value = pmt::list3(ptimestamp,pfreq,ppks);
		
		// publish message
		message_port_pub(port_id,value);

        // Tell runtime system how many output items we produced.
        return 0;
    }

  } /* namespace radar */
} /* namespace gr */

