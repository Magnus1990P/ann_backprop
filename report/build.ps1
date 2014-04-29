######################################################
# Parameters for build.ps1 is "-length" or "-l"
######################################################
# doxygen  - builds the only the doxygen report
# manual   - builds the only the manual
# guide    - builds the only the user guide
# external - builds all the external projects
# short    - builds only the report once
# medium   - builds only the report twice
# normal   - builds only the report with glossaries, 
#            definitions, bibtex and crossreferencing
# all      - builds everything
######################################################
Param(
	[parameter(Mandatory=$FALSE)]
    [alias("l")]
    $length="all"
)

#############################
# Variable declarations
#############################
# count - error counter
$count=0

# Writes first line to log
Write-Output "Error log for the build script in /rapport/hovedprosjekt/" > log.txt

#############################
# External script usage
#############################
# ./buildRef.ps1
# ../manual/buildRef.ps1

#Creates source code documentation using doxygen
if($length -eq "all" -OR $length -eq "external" -OR $length -eq "doxygen") {
	Write-Host "Feference manual"
	.\buildRef.ps1 | where-object{ write-output $_ >> log.txt }
}

#Creates the system manual for BeLT
if($length -eq "all" -OR $length -eq "external" -OR $length -eq "manual") {
	cd ..\manual
	Write-Host "Manual"
	.\buildRef.ps1 | where-object{ write-output $_ >> log.txt }
	cd ..\hovedprosjekt
}

#Creates the system manual for BeLT
if($length -eq "all" -OR $length -eq "external" -OR $length -eq "guide") {
	cd ..\brukerveiledning
	Write-Host "Manual"
	.\buildRef.ps1 | where-object{ write-output $_ >> log.txt }
	cd ..\hovedprosjekt
}

# Generating the main report
if(	$length -ne "clean"		-AND $length -AND "external" -AND 
	$length -ne "doxygen"	-AND $length -ne "manual"  -AND
	$length -ne "guide"){
	if($TRUE -eq $(Test-Path main.pdf)){
		Write-Host "Deleting old main.pdf file"
		remove-item main.pdf 
	}

	Write-Host "Starting to build the report"
	Write-Host "Running PDF-latex"
	pdflatex main.tex | where-object {
		if( $_ -match '(Warning|Error|^l.|^<use)' ){
			$count = $count+1;
		}
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"

# if short it will exit here and cleanup
if($length -ne "short"){
	$count=0
	Write-Host "Running PDF-latex"
	pdflatex main.tex | where-object{
		if( $_ -match '(Warning|Error|^l.|^<use)' ){
			$count = $count+1;
		} 
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"
	
	
# if medium it will exit here and cleanup
if($length -ne "short" -AND $length -ne "medium") {
	$count=0
	Write-Host "Running bibtex"
	bibtex main.aux | where-object{
		if( $_ -match '(Warning|Error|missing)' ){
			Write-Output $_ >> log.txt
			$count = $count+1;
		}
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"
	
	$count=0
	Write-Host "Running glossary"
	makeindex main.nlo -s nomencl.ist -o main.nls | where-object{
		if( $_ -match '(Warning|Error|missing)' ){
			$count = $count+1;
		}
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"
	
	$count=0
	Write-Host "Running glossary"
	makeglossaries.exe main.glo | where-object{
		if( $_ -match '(Warning|Error|missing)' ){
			Write-Output $_ >> log.txt
			$count = $count+1;
		}
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"
	
	$count=0
	Write-Host "Running PDF-latex"
	pdflatex main.tex | where-object{
		if( $_ -match '(Warning|Error|^l.|^<use)' ){
			$count = $count+1;
		}
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"
	
	$count=0
	Write-Host "Running PDF-latex"
	pdflatex main.tex | where-object{
		if( $_ -match '(Warning|Error|^l.|^<use)' ){
			Write-Output $_ >> log.txt
			$count = $count+1;
		}
	}
	Write-host "We found $count warnings/errors when running pdflatex, see log.txt"
}#end All
}#end medium
}#end short