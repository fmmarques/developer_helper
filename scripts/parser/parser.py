#!/usr/bin/env python3

import io
import re 

class parser:
	def __init__(self, filename):
		self.filename_ = filename
	
	def parse():
		self.context_ = [dict()]
		with fopen(self.filename, "r") as file:
			iobuf = io.BufferedReader(file)
			for line in iobuf:
				parse_line(line, self.context_)
		
	def parse_line(line,context):
		m = re.search('(?P<hh>[0-9]{2}):(?P<mm>[0-9]{2}):(?P<ss>[0-9]{2}).(?P<mm>[0-9]{3}) (?P<level>[IWE])  (?P<tid>[0-9]+)    (?P<rtp_threads_running>[0-9]+)/(?P<max_rtp_threads>[0-9]+)    (?P<A>[0-9]+)/(?P<B>[0-9]+)   (?P<message>.*)')
		print('%s', m.message)
		
