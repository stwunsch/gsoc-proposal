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


#ifndef INCLUDED_RADAR_FMCW_SEPARATE_IQ_CC_H
#define INCLUDED_RADAR_FMCW_SEPARATE_IQ_CC_H

#include <radar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace radar {

    /*!
     * \brief <+description of block+>
     * \ingroup radar
     *
     */
    class RADAR_API FMCW_separate_IQ_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<FMCW_separate_IQ_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of radar::FMCW_separate_IQ_cc.
       *
       * To avoid accidental use of raw pointers, radar::FMCW_separate_IQ_cc's
       * constructor is in a private implementation
       * class. radar::FMCW_separate_IQ_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samp_block);
    };

  } // namespace radar
} // namespace gr

#endif /* INCLUDED_RADAR_FMCW_SEPARATE_IQ_CC_H */

