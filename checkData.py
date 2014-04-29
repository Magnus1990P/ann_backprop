#!/usr/bin/python
# -*- coding: utf-8 -*-
#	Author:	Magnus Øverbø

inp = open( "data2.dta" )					#Open data set file
for c in range(0, 546):						#for all images in file(546 lies)
	image = inp.readline()					#read new line
	if c % 21 != 0 :								#Skip all first lines(informative lines)
		print c%21 										#Print current image set
		image = image.strip(" \t\r\n")#remove pre/post whitespace
		image = image.split(" ")			#split into array
		for i in range( len(image) ):	# for all items in array
			if i % 10 == 0:							#split lines into ten symbols
				print ""
			if float(image[i]) > 200:	#If pixel value is white
				print " ",								#Print space
			else:												#If pixel value is dark
				print "#",								#Print a "#"-symbold
																	#Pause printing
		raw_input("Click for next image")
