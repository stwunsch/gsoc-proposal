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


#ifndef INCLUDED_CLASSIFY_PERSON_VFI_H
#define INCLUDED_CLASSIFY_PERSON_VFI_H

#include <classify/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace classify {

    /*!
     * \brief <+description of block+>
     * \ingroup classify
     *
     */
    class CLASSIFY_API person_vfi : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<person_vfi> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of classify::person_vfi.
       *
       * To avoid accidental use of raw pointers, classify::person_vfi's
       * constructor is in a private implementation
       * class. classify::person_vfi::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_block, int samp_gap, int samp_gap_fft, int samp_rate_fft, float step_freq_max, float lsquare_max);
    };

  } // namespace classify
} // namespace gr

#endif /* INCLUDED_CLASSIFY_PERSON_VFI_H */

