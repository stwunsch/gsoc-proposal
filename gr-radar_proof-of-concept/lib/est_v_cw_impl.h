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

#ifndef INCLUDED_RADAR_EST_V_CW_IMPL_H
#define INCLUDED_RADAR_EST_V_CW_IMPL_H

#include <radar/est_v_cw.h>

namespace gr {
  namespace radar {

    class est_v_cw_impl : public est_v_cw
    {
     private:
      // Nothing to declare in this block.

     public:
      est_v_cw_impl(float center_freq);
      ~est_v_cw_impl();
      
      float d_center_freq;
      
      pmt::pmt_t port_id_in, port_id_out;
      
      std::vector<float> freq;
	  std::vector<float> pks;
	  int timestamp;
	  pmt::pmt_t ptimestamp, pfreq, ppks;
      
      void handle_msg(pmt::pmt_t msg);
      std::vector<float> vel;
      pmt::pmt_t pvel;
      
      const static float c_light = 3e8;
      
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_EST_V_CW_IMPL_H */

