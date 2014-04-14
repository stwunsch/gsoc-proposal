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
#include "est_v_cw_impl.h"
#include <iostream>

namespace gr {
  namespace radar {

    est_v_cw::sptr
    est_v_cw::make(float center_freq)
    {
      return gnuradio::get_initial_sptr
        (new est_v_cw_impl(center_freq));
    }

    /*
     * The private constructor
     */
    est_v_cw_impl::est_v_cw_impl(float center_freq)
      : gr::block("est_v_cw",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
		d_center_freq = center_freq;
		
		// Register input message port
		port_id_in = pmt::mp("cw_in");
		message_port_register_in(port_id_in);
		set_msg_handler(port_id_in, boost::bind(&est_v_cw_impl::handle_msg, this, _1));
		
		// Register output message port
		port_id_out = pmt::mp("cw_out");
		message_port_register_out(port_id_in);
	}

    /*
     * Our virtual destructor.
     */
    est_v_cw_impl::~est_v_cw_impl()
    {
    }

    void
    est_v_cw_impl::handle_msg(pmt::pmt_t msg)
    {
		ptimestamp = pmt::nth(0,msg);
		pfreq = pmt::nth(1,msg);
		ppks = pmt::nth(2,msg);
		
		timestamp = pmt::to_long(ptimestamp);
		freq = pmt::f32vector_elements(pfreq);
		pks = pmt::f32vector_elements(ppks);
		
		vel.clear();
		for(int k=0; k<freq.size(); k++){
			vel.push_back(freq[k]*c_light/2/d_center_freq);
			if(abs(vel[vel.size()-1])<50) std::cout << vel[vel.size()-1] << " ";
		}
		std::cout << std::endl;
		pvel = pmt::init_f32vector(vel.size(),vel);
	}

  } /* namespace radar */
} /* namespace gr */

