#!/usr/bin/python
import string

y = []
for i in string.ascii_uppercase:
	y.insert(0, i)

tab = open("chapters/tableA.tex", "w")

tab.write("\\begin{longtable}{")
tab.write("\n\t p{0.010\\textwidth}  ")
tab.write("\n\t p{0.010\\textwidth} | ")
for i in range(26):
	tab.write("\n\t p{0.013\\textwidth}")
tab.write(" \n}\n\\textbf{ID} & \\textbf{\\#} ")

for c in y:
	tab.write("& \\textbf{" + str(c) + "} ")
tab.write("\\\\ \\hline\n")

## DUMMY DATA
#for c in string.ascii_uppercase:
#	tab.write("\\textbf{" + str(c) + "} ")
#	for i in range(26):
#		tab.write("& \\small{20\%} ")
#	tab.write("\\\\\n")


tab.write("\n\\end{longtable}")
tab.close()
