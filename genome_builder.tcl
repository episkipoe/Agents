proc WriteFile {outfile file_name} {
	set infile [open $file_name "r"]
	while {1} {
		if [eof $infile] { break }
		gets $infile line
		puts $outfile $line
	}
	close $infile
}

proc ProcessGenome {body} {
	set outfile [open [file rootname $body].genome "w"]
	set genedir [file join [file dirname $body] genes]

	for {set i 0} {$i < 2} { incr i} {
		WriteFile $outfile $body

		foreach file_name [glob -directory $genedir *cpp] {
			WriteFile $outfile $file_name
		}
	}
}

foreach body_file [glob *cpp] {
	ProcessGenome $body_file	
}

exit

