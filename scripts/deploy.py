#!/usr/bin/env python3


import argparse
import wmi
import winreg

class service():
	def __init__(self, binpath, imagepath, name=None):
		self.binpath = binpath
		self.imagepath = imagepath
		self.name = name
		if self.name is None and False == self.read_service_configuration_from_winreg():
			raise ValueError
		
	# HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\<serv.name>\ImagePath - imagem.
	# HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\<serv.name>\ImagePath - imagem.
	def read_service_configuration_from_winreg(self):
		# Start by opening services key and,
		services_key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, 'SYSTEM\ControlSet001\Services')
		(services_subkeys_count, services_subvalues_count, services_modified) = winreg.QueryInfoKey(services_key)
		# enumerate the subkeys of service,
		for index_of_services_subkey in range(0, services_subkeys_count):
			current_service_name = winreg.EnumKey(services_key, index_of_services_subkey)
			subkey = winreg.CreateKey(services_key, current_service_name)
			(subkey_subkeys_count, subkey_subvalues_count, subkey_modfied) = winreg.QueryInfoKey(subkey)
			# to find a key whose values contain
			for index_of_subkey_value in range(0, subkey_subvalues_count):
				(subkey_value_name, subkey_value_value, subkey_value_type) = winreg.EnumValue(subkey, index_of_subkey_value)
				if "ImagePath" not in subkey_value_name:
					continue
				# an ImagePath matching the binpath and then,
				if subkey_value_value == self.binpath:
					# update the service's name and return. 
					self.name = current_service_name
					return 
		# Else [if no such service exists that ImagePath matches self.binpath] then return raise exception.
		raise ValueError("image_path")

def main():
	parser = argparse.ArgumentParser()
	
#	positional or required parameters
	parser.add_argument("bin_path", help="The complete path to the solution's binary dir.")
	parser.add_argument("image_path", help="The complete path to the service's image.")
	

#	
	parser.add_argument("-n", "--name", type=str, help="The service's name.")
	args = parser.parse_args()
	
	args.name = None
	if not args.name:
		args.name = args.bin_path[(args.bin_path.rfind('\\') + 1) : (args.bin_path.rfind('.')) ]
	
	serv = service(args.bin_path, args.image_path, args.name); 

	
	print("service name {}:\n\tbinpath={}".format(serv.name,serv.binpath))
	
	
if (__name__ == '__main__'):
	main();