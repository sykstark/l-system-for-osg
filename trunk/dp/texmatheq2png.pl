# modified for use in module xslt version 1.3
# created by Datk Lord

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}' && eval 'exec perl -S $0 $argv:q'
  if 0;
use strict;
$^W=1; # turn warning on

my $params = "";
my $textmp = "eqntmp";
my $line = "";
my $math = "";
my $file = "";
my $rm = "";

if ($^O eq "MSWin32") { $rm = 'del /F'; }
else { $rm = 'rm -f'; }

sub usage
{
    die <<"EndUsage";
usage: texmatheq2png.pl tex-math-equations.tex params

texmatheq2png.pl -- A program for making png images of equations from
                  an tex-math-equations.tex source file.

EndUsage
}


sub getparams {
my $i;
	for ($i = 1; $i <= $#ARGV; $i++) {
		$params .= "$ARGV[$i] ";
	} 
}

sub printtex {
	my ($file, $math) = @_;
    	open (TEX, ">$textmp.tex") || die "Can't open $textmp";
    	print TEX "\\documentclass{article}\n";
    	print TEX "\\pagestyle{empty}\n";
    	print TEX "\\begin{document}\n";
    	print TEX "\\LARGE\n";
		print TEX "$math\n";
    	print TEX "\\end{document}\n";
    	close (TEX);
    	system ("latex $textmp");
    	system ("dvips -E -o $textmp.eps $textmp");
    	system ("convert $params $textmp.eps $file");
		system ("$rm $textmp.*");
}

if (@ARGV == 0) { &usage; }
my $doc = $ARGV[0];
getparams();

open (DOC, "<$doc") || die "Can't open $doc";

CTENI: while ($line = <DOC>) {
	chomp $line;
	if (($line eq "\\documentclass{article} ")||
	($line eq "\\pagestyle{empty} ")||
	($line eq "\\begin{document} ")||
	($line eq "\\end{document} "))
	{ next CTENI; }

	if ($line =~ s/\\special\{dvi2bitmap outputfile ([a-zA-Z0-9_\/\.]+)\}\s*//) {
		$file = $1;
		next CTENI;
 	}

	if ($line eq "\\newpage ") {
	printtex($file, $math);
	$math = "";
	}
	else { $math .= $line; }
}

close (DOC);

