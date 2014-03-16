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


#ifndef INCLUDED_EMULATE_HARDWARE_READIQ_C_H
#define INCLUDED_EMULATE_HARDWARE_READIQ_C_H

#include <emulate_hardware/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace emulate_hardware {

    /*!
     * \brief <+description of block+>
     * \ingroup emulate_hardware
     *
     */
    class EMULATE_HARDWARE_API readIQ_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<readIQ_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of emulate_hardware::readIQ_c.
       *
       * To avoid accidental use of raw pointers, emulate_hardware::readIQ_c's
       * constructor is in a private implementation
       * class. emulate_hardware::readIQ_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string filepath);
    };

  } // namespace emulate_hardware
} // namespace gr

#endif /* INCLUDED_EMULATE_HARDWARE_READIQ_C_H */

