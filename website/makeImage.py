#!/usr/bin/env python
# -*- coding: UTF-8 -*-

# enable debugging
import cgitb
cgitb.enable(logdir='./cgitblog')

import os
import logging
import cgi
from subprocess import call
import os.path

logging.basicConfig(filename='makeImage.log',level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s')
logging.info('starting')

print("Content-Type: text/plain;charset=utf-8")
print("")

arguments = cgi.FieldStorage()
for i in arguments.keys():
	outimage = arguments[i].value.strip()
	logging.debug('argument:' + str(i) + ':' + outimage)

	if (os.path.isfile('../' + outimage)):
		logging.info('Already Exists:' + outimage)
		print('Already exists:' + outimage)
		continue

	tokens = outimage.split('/')
	alg = tokens[0]

	params = tokens[1].split('.')
	image = params[0]
	ext   = params[-1]

	logging.debug('alg:'   + alg)
	logging.debug('image:' + image)
	logging.debug('ext:' + ext)

	# TODO: Make sure parameters are proper filenames (no '&& rm -rf /' :)
	# TODO: All parameters should be int. Enforce.

	inimage  = '../sample/' + image + '.' + ext 	
	cmd = ['./processImage', inimage, '../' + outimage, alg] + params[1:len(params)-1]
	# cmd = cmd[0:3]
	logging.info('Executing:' + str(cmd))

	print("Executing: " + str(cmd))
	call(cmd)

logging.info('done')

