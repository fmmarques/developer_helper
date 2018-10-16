#!/usr/bin/env python3

import thread
import argparse
import re

import parser


def main():
""" """
""" """
	argparser = argparse.ArgumentParser(description='Process OneSIPConnector logs.')
	argparser.add_argument('directory', type=str, help='the directory holding the logs.')
	args = argparser.parse_args()
	del argparser 
	
	for key in vars(args):
		print(key,':', vars(args)[key])
	directory = vars(args)[key]
	del args
	
	
	logs = [];
	for logname in os.listdir(directory):
		matches = re.match("^OneSIPConnector.*\.log$", logname)
		if (matches):
			logs.append(logname)
			print("Adding \"", logname, "\" to the processing queue."); 
	
			
if (__name__ == "__main__"):
	main()