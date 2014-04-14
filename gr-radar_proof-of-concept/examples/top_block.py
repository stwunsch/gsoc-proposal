#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Apr 14 17:53:04 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import scopesink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import radar
import wx

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 125000
        self.packet_len = packet_len = 4096
        self.decimator_factor = decimator_factor = 8
        self.samp_rate_dec = samp_rate_dec = samp_rate/decimator_factor
        self.samp_protect = samp_protect = 0
        self.samp_compare = samp_compare = 5
        self.rel_threshold = rel_threshold = 1
        self.packet_len_dec = packet_len_dec = packet_len/decimator_factor
        self.noise = noise = 0.5
        self.center_freq = center_freq = 6e9
        self.avg_mult = avg_mult = 6

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_scopesink2_0 = scopesink2.scope_sink_c(
        	self.GetWin(),
        	title="Scope Plot",
        	sample_rate=samp_rate,
        	v_scale=0,
        	v_offset=0,
        	t_scale=0,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=1,
        	trig_mode=wxgui.TRIG_MODE_AUTO,
        	y_axis_label="Counts",
        )
        self.Add(self.wxgui_scopesink2_0.win)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate_dec,
        	fft_size=packet_len_dec,
        	fft_rate=15,
        	average=True,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.radar_tsfft_cc_0 = radar.tsfft_cc()
        self.radar_simulator_cc_0 = radar.simulator_cc(([0,0,0,0,0]), ([-10,-5,0,3,8]), ([1e9,1e9,1e9,1e9,1e9]), samp_rate, center_freq)
        self.radar_signal_generator_cw_c_0 = radar.signal_generator_cw_c(packet_len, samp_rate, 0, 1)
        self.radar_os_cfar_c_0 = radar.os_cfar_c(samp_rate_dec, samp_compare, samp_protect, rel_threshold, avg_mult)
        self.radar_est_v_cw_0 = radar.est_v_cw(center_freq)
        self.radar_decimator_cc_0 = radar.decimator_cc(decimator_factor)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, noise, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.radar_tsfft_cc_0, 0), (self.radar_os_cfar_c_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_multiply_xx_0, 1))
        self.connect((self.radar_signal_generator_cw_c_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.radar_simulator_cc_0, 0))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.radar_simulator_cc_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.wxgui_scopesink2_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.radar_decimator_cc_0, 0))
        self.connect((self.radar_decimator_cc_0, 0), (self.radar_tsfft_cc_0, 0))
        self.connect((self.radar_decimator_cc_0, 0), (self.wxgui_fftsink2_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.radar_os_cfar_c_0, "cfar_out", self.radar_est_v_cw_0, "cw_in")

# QT sink close method reimplementation

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_rate_dec(self.samp_rate/self.decimator_factor)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.wxgui_scopesink2_0.set_sample_rate(self.samp_rate)

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len
        self.set_packet_len_dec(self.packet_len/self.decimator_factor)

    def get_decimator_factor(self):
        return self.decimator_factor

    def set_decimator_factor(self, decimator_factor):
        self.decimator_factor = decimator_factor
        self.set_packet_len_dec(self.packet_len/self.decimator_factor)
        self.set_samp_rate_dec(self.samp_rate/self.decimator_factor)

    def get_samp_rate_dec(self):
        return self.samp_rate_dec

    def set_samp_rate_dec(self, samp_rate_dec):
        self.samp_rate_dec = samp_rate_dec
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate_dec)

    def get_samp_protect(self):
        return self.samp_protect

    def set_samp_protect(self, samp_protect):
        self.samp_protect = samp_protect

    def get_samp_compare(self):
        return self.samp_compare

    def set_samp_compare(self, samp_compare):
        self.samp_compare = samp_compare

    def get_rel_threshold(self):
        return self.rel_threshold

    def set_rel_threshold(self, rel_threshold):
        self.rel_threshold = rel_threshold

    def get_packet_len_dec(self):
        return self.packet_len_dec

    def set_packet_len_dec(self, packet_len_dec):
        self.packet_len_dec = packet_len_dec

    def get_noise(self):
        return self.noise

    def set_noise(self, noise):
        self.noise = noise
        self.analog_noise_source_x_0.set_amplitude(self.noise)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq

    def get_avg_mult(self):
        return self.avg_mult

    def set_avg_mult(self, avg_mult):
        self.avg_mult = avg_mult

if __name__ == '__main__':
    import ctypes
    import os
    if os.name == 'posix':
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()

