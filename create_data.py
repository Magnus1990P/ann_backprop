#!/usr/bin/python
# -*- coding: utf-8 -*-
#	Author: Magnus Øverbø
	"""
	for i in range( 5 - len(tTxt) ):
		dData.write( " 0")
	for c in tTxt:
		dData.write( " " + c )
	"""

import Image			#Python Img Lib, used to manipulate and extract pixel values
import string			#Library to use "string.ascii_uppercase" for iteration

char = "A"	#	Initialized starting charachter "A"
targ = 1		#	Initialized starting target for "A"
arr  = []		#	Array to hold the 3x3 pixel subset, used to calculate average
count = 1;	#	Initial count value

dData = open("data.dta", "w")						#Open file for writing testing data
for char in string.ascii_uppercase:			#Iterate through A..Z
	dData.write( char )
	tTxt = str(bin(targ))[2:]
	for i in range(26):
		if i == 26-targ:
			dData.write(" 1")
		else:
			dData.write(" 0" )
	dData.write("\n")
		
	for count in range(1, 21):						#For each letter go through 1 to 20
		iName = char + str(count)  + ".jpg"	#Filename A1.jpg ... Z20.jpg
		print iName
		im = Image.open("pics/" + iName)		#Open the org 30x30 grayscale image
		imOrg = im.load()										#LOad image to manipulate pixels
		imNew = Image.new("L", (30,30))			#Create new grayscale 10x10 image
		imNewP = imNew.load()								#Load image to manipulate pixels
		imgArr= []													#Array to hold the 100 avg pixel values
		
		for y in range(0,10):			#Go through all rows in 10x10 image
			for x in range(0, 10):	#Go through all columns in 10x10 image
				arr = None						#Reset value array
				arr = [								#Create array from pixel values of the 3x3 subset
					imOrg[x*3 , 	y*3],		imOrg[x*3+1 , y*3],		imOrg[x*3+2 , y*3],
					imOrg[x*3 , 	y*3+1],	imOrg[x*3+1 , y*3+1],	imOrg[x*3+2 , y*3+1],
					imOrg[x*3 , 	y*3+2],	imOrg[x*3+1 , y*3+2],	imOrg[x*3+2 , y*3+2]
				]
				imNewP[x*3, 	y*3] = sum(arr)/len(arr)		#Insert avg pixel val to image
				imNewP[x*3+1, y*3] = sum(arr)/len(arr)		#Insert avg pixel val to image
				imNewP[x*3+2, y*3] = sum(arr)/len(arr)		#Insert avg pixel val to image
				
				imNewP[x*3, 	y*3+1] = sum(arr)/len(arr)	#Insert avg pixel val to image
				imNewP[x*3+1, y*3+1] = sum(arr)/len(arr)	#Insert avg pixel val to image
				imNewP[x*3+2, y*3+1] = sum(arr)/len(arr)	#Insert avg pixel val to image
				
				imNewP[x*3, 	y*3+2] = sum(arr)/len(arr)	#Insert avg pixel val to image
				imNewP[x*3+1, y*3+2] = sum(arr)/len(arr)	#Insert avg pixel val to image
				imNewP[x*3+2, y*3+2] = sum(arr)/len(arr)	#Insert avg pixel val to image
				
				imgArr.append( sum(arr)/float(len(arr)) )	#Insert avg pixel val to array
		imNew.save("con/" + iName)	#Save the new 10x10 picture to con/[filename]

		for i in imgArr:						#For all values un the image array
			dData.write( str( float(i) ) + " " )
		dData.write( "\n" )
	targ += 1
	dData.close()
	raw_input("Check the written file")
	dData = open( "data.dta", "a" )
dData.close()
