if {1} {
	set genedir /home/dbennett/Agents/genes
	set body /home/dbennett/Agents/body.cpp
	set outfilename /home/dbennett/Agents/organism0
} else {
	set genedir [tk_chooseDirectory -title "Gene Directory"]
	set body [tk_getOpenFile -title "body"]
	set outfilename [tk_getSaveFile -title "Output Genome filename"]
}

proc WriteFile {outfile file_name} {
	set infile [open $file_name "r"]
	while {1} {
		if [eof $infile] { break }
		gets $infile line
		puts $outfile $line
	}
	close $infile
}

set outfile [open $outfilename "w"]

for {set i 0} {$i < 2} { incr i} {
	WriteFile $outfile $body

	foreach file_name [glob -directory $genedir *cpp] {
		WriteFile $outfile $file_name
	}
}

exit

