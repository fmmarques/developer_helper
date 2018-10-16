#!/usr/bin/env python

import sys
import argparse
import pefile
import time

def parse_pe(image):
	try:
		parsed_pe = pefile.PE(image)
		print('{}',parsed_pe)	
		# print('timestamp = {}({})' % (time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(parsed_pe.timestamp)), parsed_pe.timestamp))
	except Exception as e:
		print('{}' % e)
	
def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("image", help="The image to check.")
	arguments = parser.parse_args()
	parse_pe(arguments.image)

if __name__ == '__main__':
	main()