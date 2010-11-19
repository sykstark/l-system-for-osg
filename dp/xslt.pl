#!/usr/bin/perl
# Version         : $Id: xslt.pl,v 2.0.36 2006-06-06 23:00:12+01 darklord Exp $
# Author          : Jan Pavlovic pavlovic@fi.muni.cz
# Created On      : Mon Jan 27 23:00:12 2003
# Last Modified By: Jan Pavlovic

use locale;
use strict;
use Getopt::Long;
use File::Copy;

my $VERSION;
my $VERSION_STRING = $VERSION = "2.0.36";

# Global variables.
my $perl_os = $^O;
my $perl_version = $];
my ($opt_silent,$opt_timing,$opt_novalid,@opt_param,$opt_type,$opt_file,$opt_xsl,$opt_out,$opt_debug,$opt_catalog,$opt_nocatalog,$opt_xinclude,$opt_config,$opt_latex,$opt_nolatex,$opt_processor);

# ------------------------------------------------------------------------------ #

# Paths definitions
my $package_path = "c:\\Program\ Files\\xslt2-2.6.5";

my $share_path;
my $run_path;
my $xalan_home;
my $saxon_home;
my $saxon7_home;
my $saxonb8_home;
my $xerces_home;
my $jd_home;
my $xt_home;
my $xfc_home;
my $fop_home;
my $fop_cp;
my $xep_home;
my $xep_cp;
my $xalan_cp;
my $saxon_cp;
my $saxon7_cp;
my $saxonb8_cp;
my $xerces_cp;
my $jd_cp;
my $xt_cp;
my $xfc_cp;
my $catalog_manager;
my $catalog_files;

# Binary files definitions
my $xsltproc;
my $xmllint;
my $xsltproc_MSWin32;
my $xmllint_MSWin32;
my $java;
my $index;
my $vlna;
my $pdflatex;
my $pdflatex_MSWin32;
my $tex;
my $latex;
my $latex_MSWin32;
my $pdfxmltex;
my $dvips;
my $perl;
my $dvi2bitmap;
my $convert;

# Parameters definitions
my $convertparam;
my $default_config;
my $term;
my $slash;
my $xslversion;
my $xfcparam;
my $bmtype;
my $convertor;
my $xincluder;
my $xincluder_param;
my $resolver;
my $xmlreader;
my $catalogresolver;
my $documentbuilder;
my $saxparser;
my $texpass;
my $xep_config;
my $fop_config;
	
# Platform depended definitions
if ($perl_os eq "MSWin32") {
	$package_path .= "\\";
	$term = ";";
	$slash = "\\";
	$default_config = $ENV{"HOMEPATH"}."/.xsltrc";
}
else {
	$package_path .= "/";
	$term = ":";
	$slash = "/";
	$default_config = $ENV{"HOME"}."/.xsltrc";
}
#elsif ($perl_os eq "linux") { }
#elsif ($perl_os eq "irix") { }
#elsif ($perl_os eq "solaris") { }

# Stylesheets definitions
my $db2latex;
my $fithesis;
my $fithesisprint;
my $fithesisdraft;
my $html;
my $htmlchunk;
my $xhtml;
my $xhtmlchunk;
my $fo;
my $foxep;
my $fofop;
my $htmlhelp;
my $javahelp;

# Templates definitions
my $bcthesis;
my $thesis;
# End of definitions


# ------------------------------- main -------------------------------- #

parseopt();
loadConfig("${package_path}lib${slash}\.xsltrc");
if ($opt_config) { loadConfig($opt_config); }
print "transforming...\n";
jobDecide();
print "done\n";
exit 0;

# ----------------------------- functions ----------------------------- #

# Print and exec command.
sub dow {
	if ($opt_silent) {
		if ($perl_os eq "MSWin32") { system "@_ 1>/nul 2>/nul"; }
		else { system "@_ 1>/dev/null 2>/dev/null"; }
	}
	else {
		print "@_\n";
		system @_;
	}
}

# Printing correct usage.
sub printhelp {
    print<<EOF;

usage: xslt [options] inputfile

-h, --help      prints this help message
-v, --version   prints version info
-d, --debug     debud mode: no temporary files cleaning
-e, --silent    do not display any warnings
-m, --timing    display consumed time
-f, --config    set config file
-n, --nocatalog do not use catalog files
-y, --nolatex   do not make changes in LaTeX file
-x, --xinclude  xinclude support
-l, --latex     make some useful changes in LaTeX file
-s, --xsl       stylesheet
-o, --out       output file
-i, --in        input file
-r, --param     stylesheets parameters
-p, --processor xslt processor [xsltproc|saxon|saxon7|saxonb8|xalan|jd]
-t, --task      task {parse|jparse|valid|jvalid|
                       pdf|foppdf|xeppdf|ps|dvi|rtf|
                       win2utf|iso2utf|math|new|}

examples:
xslt -t valid file.xml
validate file.xml

xslt -s html file.xml
converts file.xml into file.html

xslt -l -t pdf -s fithesis file.xml
converts file.xml into file.pdf with fithesis cls style
more help: http://www.fi.muni.cz/~xpavlov/xml
EOF
}

# Parsing input parameters.
sub parseopt {
my($result,$opt_help,$opt_version);
	$result = GetOptions (
		'h|help'        => \$opt_help, 
		'd|debug'       => \$opt_debug, 
		'v|version'     => \$opt_version, 
		'c|calalog'     => \$opt_catalog, 
		'n|nocatalog'   => \$opt_nocatalog,
		'x|xinclude'    => \$opt_xinclude, 
		'l|latex'       => \$opt_latex,
		'y|nolatex'     => \$opt_nolatex,
		'm|timing'			=> \$opt_timing,
		'e|silent'			=> \$opt_silent,
		'f|config=s'    => \$opt_config,
		't|type=s'      => \$opt_type,
		's|xsl=s'       => \$opt_xsl,
		'i|in|xml=s'    => \$opt_file,
		'o|out=s'       => \$opt_out,
		'p|processor=s'	=> \$opt_processor,
		'r|param=s'     => \@opt_param,
		'novalid'       => \$opt_novalid
		);

	if (($opt_help) || (!$result)) {
		printhelp();
		exit 1;
	}
	if ($opt_version) {	print "xslt transformation tool version $VERSION_STRING \n"; }

#	# User miss switch: -t.
#	if ((!$opt_type) && ($ARGV[1])) {
#		$opt_type = $ARGV[0];
#		$ARGV[0] = $ARGV[1];
#	} 
#	elsif ((!$opt_type) && (! -f $ARGV[0])){
#		$opt_type = $ARGV[0];
#	}
	
	# User doesn't use switch: -i.
	if ((!$opt_file) && ($ARGV[0])) {
		$opt_file = $ARGV[0];
	} 

	# User doesn't give correct input.
    if (!$opt_file) {
    	if (!$opt_version) {
			print "No input!\n";
		}
		exit 1;
	}
	elsif ((! -f $opt_file) && ($opt_type ne "new")) {
	# User don't give input suffix
		if (-f "$opt_file.xml") {
			$opt_file .= ".xml";
		}
		else {
			print "File: $opt_file not found!\n";
			exit 1;
		}
	}

	# User doesn't give correct config file.
	if (($opt_config)&&(! -f $opt_config)) {
		print "Config file: $opt_config not found!\n";
		exit 1;
	}
	
	# Try to use default config file.
	if (!$opt_config) { 
		if (-f $default_config) {
			$opt_config = $default_config;
		}
		else {
			$opt_config = undef; 
		}
	}

  # enable use saxon8 as well as saxonb8
	if (!$opt_processor)           { $opt_processor = "xsltproc"; }
	if ($opt_processor eq "saxon8") { $opt_processor = "saxonb8"; }
	if ($opt_processor eq "saxon6") { $opt_processor = "saxon"; }
	if ($opt_processor eq "jdxslt") { $opt_processor = "jd"; }
	
	if (($opt_processor ne "xsltproc") &&
		  ($opt_processor ne "saxon") &&
		  ($opt_processor ne "saxon7") &&
		  ($opt_processor ne "saxonb8") &&
		  ($opt_processor ne "xalan") &&
		  ($opt_processor ne "jd") &&
		  ($opt_processor ne "xt")) # deprecated
	{
		print "Bad processor parametr: $opt_processor!\n";
		exit 1;
	}
	
	#catalog changes are implicite
  $opt_nocatalog ? ( $opt_catalog = 0 ) : ( $opt_catalog = 1 );
	$opt_nolatex ? ( $opt_latex = 0 ) : "";

}

sub loadConfig {
my ($config) = @_;
	if (-f $config) {
		my ($line,$key,$value,$command);
		open(IN,"$config") or die "Cannot open file: $config!\n";
		while ( $line = <IN> ){
			if ($line =~ /^\s*\#.*/) { next; } # Ignore comments lines
			if ($line =~ /^\s*$/) { next; } # Ignore empty lines
			if ($line !~ /^\s*\$/) { next; } # Ignore non correct lines
			$line =~ s/(.*[^\#])\#.*/$1/g; # Remove comments
			$line =~ s/;\s*$//g; # Remove ; from line ends
			$line =~ s/^\s*(\$.+[^\s^=])\s*=\s*(["\$].*[^\s])\s*$/$1=$2/g; # Remove irelevant spaces
			$key = $1; $value = $2;
			if ($key =~ /.*_$perl_os$/) {
				$key =~ s/^(.*)_$perl_os$/$1/;
				eval("$key = $value;");
			}
			else {
				eval("$key = $value;");
			}
			$opt_debug ? print "$key = ", eval("$key"), "\n" : "";
		}
		close(IN);
	}
	else { print "Cannot open file: $config!\n"; }
}

sub jobDecide {
my $suffix;
	$opt_xsl = lc($opt_xsl);

	# Setting xsl stylesheets and guessing suffixies if missing.
	if ($opt_xsl eq "html") { $opt_xsl = $html; !$opt_out ? $suffix = "html" : ""; }
	elsif ($opt_xsl eq "htmlchunk") { $opt_xsl = $htmlchunk; !$opt_out ? $suffix = "html" : ""; }
	elsif ($opt_xsl eq "xhtml") { $opt_xsl = $xhtml; !$opt_out ? $suffix = "xhtml" : ""; }
	elsif ($opt_xsl eq "xhtmlchunk") { $opt_xsl = $xhtmlchunk; !$opt_out ? $suffix = "xhtml" : ""; }
	elsif ($opt_xsl eq "foxep") { $opt_xsl = $foxep; !$opt_out ? $suffix = "fo" : ""; }
	elsif ($opt_xsl eq "fofop") { $opt_xsl = $fofop; !$opt_out ? $suffix = "fo" : ""; }
	elsif ($opt_xsl eq "fo") { 
		!$opt_out ? $suffix = "fo" : ""; 
		if (($opt_type eq "fopdf")||($opt_type eq "foppdf")) {
			$opt_xsl = $fofop; 
		}
		elsif ($opt_type eq "xeppdf") {
			$opt_xsl = $foxep; 
		}
		else {
			$opt_xsl = $fo; 
		}		
	}
	elsif ($opt_xsl eq "latex") { $opt_xsl = $db2latex; !$opt_out ? $suffix = "tex" : ""; }
	elsif (($opt_xsl eq "fithesis")||($opt_xsl eq "fi")) { $opt_xsl = $fithesis; !$opt_out ? $suffix = "tex" : ""; }
	elsif (($opt_xsl eq "fithesisprint")||($opt_xsl eq "fithesis-print")||($opt_xsl eq "fiprn")||($opt_xsl eq "fi-print")) { $opt_xsl = $fithesisprint; !$opt_out ? $suffix = "tex" : ""; }
	elsif (($opt_xsl eq "fithesisdraft")||($opt_xsl eq "fithesis-draft")||($opt_xsl eq "fidraft")||($opt_xsl eq "fi-draft")) { $opt_xsl = $fithesisdraft; !$opt_out ? $suffix = "tex" : ""; }
	elsif ($opt_xsl eq "javahelp") { $opt_xsl = $javahelp; $suffix = "void"; }
	elsif ($opt_xsl eq "htmlhelp") { $opt_xsl = $htmlhelp; $suffix = "void"; }
	elsif (($opt_xsl) && (! -f $opt_xsl)) {
		print "File $opt_xsl not found!\n";
		exit 1;
	}

	# Force LaTeX changes
	if ((!$opt_nolatex)&&(($suffix eq "tex")||($opt_type eq "pdf")||($opt_type eq "ps")||($opt_type eq "dvi"))) { $opt_latex = 1; }

	# Guessing suffixies if missing.
	if (($opt_type eq "parse")||($opt_type eq "parser")) { $suffix = "void"; }
	elsif (($opt_type eq "jparse")||($opt_type eq "jparser")) { $suffix = "void"; }
	elsif ($opt_type eq "math") { $suffix = "void"; }
	elsif ($opt_type eq "new") { $suffix = "void"; }
	elsif (($opt_type eq "validate")||($opt_type eq "valid")) { $suffix = "void"; }
	elsif (($opt_type eq "jvalidate")||($opt_type eq "jvalid")) { $suffix = "void"; }
	elsif ($opt_type eq "pdf") { !$opt_out ? $suffix = "pdf" : ""; }
	elsif (($opt_type eq "fopdf")||($opt_type eq "foppdf")||($opt_type eq "xeppdf")||($opt_type eq "latexpdf")) { !$opt_out ? $suffix = "pdf" : ""; }
	elsif ($opt_type eq "ps") { !$opt_out ? $suffix = "ps" : ""; }
	elsif (($opt_type eq "fops")||($opt_type eq "fopps")||($opt_type eq "xepps")||($opt_type eq "latexps")) { !$opt_out ? $suffix = "ps" : ""; }
	elsif ($opt_type eq "dvi") { !$opt_out ? $suffix = "dvi" : ""; }
	elsif ($opt_type eq "rtf") { !$opt_out ? $suffix = "rtf" : ""; }

	# User doesn't give output: output <- same prefix as input.
	if (!$opt_out) {
		if (!$suffix) {
			print "No output!\n";
			exit 1;
		}
		elsif ($suffix ne "void") {
			if ($opt_xsl eq $fithesisprint) {
				$opt_out = getPrefix($opt_file)."-print.$suffix";
			}
			elsif ($opt_xsl eq $fithesisdraft) {
				$opt_out = getPrefix($opt_file)."-draft.$suffix";
			}
			else {
				$opt_out = getPrefix($opt_file).".$suffix";
			}
		}
	}

	if ($opt_type eq "") { processor(); }
	elsif (($opt_type eq "parser")||($opt_type eq "parse")) { parser(); }
	elsif (($opt_type eq "jparser")||($opt_type eq "jparse")) { jparser(); }
	elsif (($opt_type eq "validate")||($opt_type eq "valid")) { validate(); }
	elsif (($opt_type eq "jvalidate")||($opt_type eq "jvalid")) { jvalidate(); }
	elsif (($opt_type eq "pdf")||($opt_type eq "latexpdf")) { pdf(); }
	elsif (($opt_type eq "fopdf")||($opt_type eq "foppdf")) { foppdf(); }
	elsif ($opt_type eq "xeppdf") { xeppdf(); }
	elsif ($opt_type eq "xepps") { xepps(); }
	elsif (($opt_type eq "ps")||($opt_type eq "latexps")) { ps(); }
	elsif ($opt_type eq "dvi") { dvi(); }
	elsif ($opt_type eq "rtf") { xslrtf(); }
	elsif ($opt_type eq "win2utf") { win2utf(); }
	elsif ($opt_type eq "iso2utf") { iso2utf(); }
	elsif ($opt_type eq "math") { math(); }
	elsif ($opt_type eq "new") { newtemplate(); }
	elsif ($opt_type eq "xmltexpdf") { xmltexpdf(); } # deprecated
	else {
		print "Bad type!: $opt_type\n";
		printhelp();
		exit 1;
	}
}

# Return filename without suffix.
sub getPrefix {
my ($file) = @_;
my $dot = rindex($file, ".");
	$dot == -1 ? $dot = length($file) : "";
return substr($file, 0, $dot);
}

# Return filename with given suffix.
sub correctSuffix {
my ($file, $suffix) = @_;
my $dot = rindex($file, ".");
	if ($dot == -1) {
		return "$file.$suffix";
	}
	else {
	return substr($file, 0, $dot).".$suffix";
	}
}

# Return param string explicit identificators.
sub getParams {
my ($p,$fsp,$eql) = @_; 
my @params = @$p; # Need to resolve scalar pointer (perl=fuj)
my $param_string;
    foreach my $index (0 .. $#params) {
		$params[$index] =~ s/=/ /g;
		$params[$index] =~ s/\s/$eql/g;
        $fsp ? ($param_string .= $fsp." $params[$index] ") : ($param_string .= "$params[$index] ");
    }
return $param_string;
}

# Do some logo macros change.
sub changeMacro {
	my $line;
	my $void = 0;
	my ($file) = @_;
	my ($filetmp) = "$file.xslttmp";
	my $linecount = 0;
	rename($file,$filetmp);
	open(IN,"$filetmp") or die "Cannot open file: $filetmp!\n";
	open(OUT,">$file") or die "Cannot open file: $file!\n";
	while ( $line = <IN> ) {
		# Ignore XML header only at first line of file
		$linecount++;
		if (($line =~ /^\s*<\?xml.*\?>\s*$/) && ($linecount == 1)) { next; }
		# Remove addional empty lines
		if ($line =~ /^\s*\n+/) {
			if ($void == 0) { $void = 1; }
			else { next; }
		} else { $void = 0;	}
		# Remove addional tabs (this wasn't good idea, makes mass in programlisting)
#		$line =~ s/^\t+(\S)/$1/g ;
		# Ignore comments lines
		if ($line !~ /^\s*%.*/) {
			# Remove suffix from images
			$line =~ s/^(.*\\includegraphics\[.*\]{)(.*)\.[^}.]*(.*)$/$1$2$3/g;

			# LaTeX2e
			$line =~ s/([^\\])LaTeXe /$1\\LaTeXe{} /g;
#			$line =~ s/([^\\\/])LaTeXe/$1\\LaTeXe /g;
			$line =~ s/^LaTeXe /\\LaTeXe{} /g;
#			$line =~ s/^LaTeXe/\\LaTeXe /g;

			# LaTeX
			$line =~ s/([^\\])LaTeX /$1\\LaTeX{} /g;
			$line =~ s/([^\\\/])LaTeX/$1\\LaTeX /g;
			$line =~ s/^LaTeX /\\LaTeX{} /g;
			$line =~ s/^LaTeX/\\LaTeX /g;

			# TeX
			$line =~ s/([^a\\])TeX /$1\\TeX{} /g;
			$line =~ s/([^a\\\/])TeX/$1\\TeX /g;
			$line =~ s/^TeX /\\TeX{} /g;
			$line =~ s/^TeX/\\TeX /g;

			# METAFONT
			$line =~ s/([^\\])M[Ee][Tt][Aa]F[Oo][Nn][Tt] /$1\\MF{} /g;
			$line =~ s/([^\\\/])M[Ee][Tt][Aa]F[Oo][Nn][Tt]/$1\\MF /g;
			$line =~ s/^M[Ee][Tt][Aa]F[Oo][Nn][Tt] /\\MF{} /g;
			$line =~ s/^M[Ee][Tt][Aa]F[Oo][Nn][Tt]/\\MF /g;

			# METAPOST
			$line =~ s/([^\\])M[Ee][Tt][Aa]P[Oo][Ss][Tt] /$1\\MP{} /g;
			$line =~ s/([^\\\/])M[Ee][Tt][Aa]P[Oo][Ss][Tt]/$1\\MP /g;
			$line =~ s/^M[Ee][Tt][Aa]P[Oo][Ss][Tt] /\\MP{} /g;
			$line =~ s/^M[Ee][Tt][Aa]P[Oo][Ss][Tt]/\\MP /g;

			# mdash
			$line =~ s/&#8212;/---/g;
			# ndash
			$line =~ s/&#8211;/--/g;

			# alpha
			$line =~ s/&#945;/\$\\alpha\$/g;
			# beta
			$line =~ s/&#946;/\$\\beta\$/g;
			# gamma
			$line =~ s/&#947;/\$\\gamma\$/g;
			# delta
			$line =~ s/&#948;/\$\\delta\$/g;
			# epsilon
			$line =~ s/&#8714;/\$\\epsilon\$/g;
			# varepsilon
			$line =~ s/&#949;/\$\\varepsilon\$/g;
			# zeta
			$line =~ s/&#950;/\$\\zeta\$/g;
			# eta
			$line =~ s/&#951;/\$\\eta\$/g;
			# theta
			$line =~ s/&#952;/\$\\theta\$/g;
			# vartheta
			$line =~ s/&#977;/\$\\vartheta\$/g;
			# iota
			$line =~ s/&#953;/\$\\iota\$/g;
			# kappa
			$line =~ s/&#954;/\$\\kappa\$/g;
			# lambda
			$line =~ s/&#955;/\$\\lambda\$/g;
			# mu
			$line =~ s/&#956;/\$\\mu\$/g;
			# nu
			$line =~ s/&#957;/\$\\nu\$/g;
			# xi
			$line =~ s/&#958;/\$\\xi\$/g;
			# pi
			$line =~ s/&#960;/\$\\pi\$/g;
			# varpi
			$line =~ s/&#982;/\$\\varpi\$/g;
			# rho
			$line =~ s/&#961;/\$\\rho\$/g;
			# varrho
			$line =~ s/&#1009;/\$\\varrho\$/g;
			# sigma
			$line =~ s/&#963;/\$\\sigma\$/g;
			# varsigma
			$line =~ s/&#962;/\$\\varsigma\$/g;
			# tau
			$line =~ s/&#964;/\$\\tau\$/g;
			# upsilon
			$line =~ s/&#965;/\$\\upsilon\$/g;
			# phi
			$line =~ s/&#966;/\$\\phi\$/g;
			# varphi
			$line =~ s/&#981;/\$\\varphi\$/g;
			# chi
			$line =~ s/&#967;/\$\\chi\$/g;
			# psi
			$line =~ s/&#968;/\$\\psi\$/g;
			# omega
			$line =~ s/&#969;/\$\\omega\$/g;

			# Gamma
			$line =~ s/&#915;/\$\\Gamma\$/g;
			# Delta
			$line =~ s/&#916;/\$\\Delta\$/g;
			# Theta
			$line =~ s/&#920;/\$\\Theta\$/g;
			# Lambda
			$line =~ s/&#923;/\$\\Lambda\$/g;
			# Xi
			$line =~ s/&#926;/\$\\Xi\$/g;
			# Pi
			$line =~ s/&#928;/\$\\Pi\$/g;
			# Sigma
			$line =~ s/&#931;/\$\\Sigma\$/g;
			# Upsilon
			$line =~ s/&#978;/\$\\Upsilon\$/g;
			# Phi
			$line =~ s/&#934;/\$\\Phi\$/g;
			# Psi
			$line =~ s/&#936;/\$\\Psi\$/g;
			# Sigma
			$line =~ s/&#931;/\$\\Sigma\$/g;
			# Omega
			$line =~ s/&#937;/\$\\Omega\$/g;

			# hellip
			$line =~ s/&#8230;/\\dots/g;
			# copy
			$line =~ s/&#169;/\\copyright/g;
			# reg
			$line =~ s/&#174;/\\textregistered/g;
			# trade
			$line =~ s/&#8482;/\\texttrademark/g;
			# sect
			$line =~ s/�/\$\\S\$/g;
			# times
			$line =~ s/�/\$\\times\$/g;
			# plusmn
			$line =~ s/&#177;/\$\\pm\$/g;
			# cong
			$line =~ s/&#8773;/\$\\cong\$/g;
			# quiv
			$line =~ s/&#8801;/\$\\equiv\$/g;
			# int
			$line =~ s/&#8747;/\$\\int\$/g;
			# le 
			$line =~ s/&#8804;/\$\\leq\$/g;
			# ne 
			$line =~ s/&#8800;/\$\\neq\$/g;
			# ge 
			$line =~ s/&#8805;/\$\\geq\$/g;
			# lsquor
			$line =~ s/&#8218;/\\char13{}/g;
			# rsquor
			$line =~ s/&#8219;/\\char39{}/g;
			# ldquor
			$line =~ s/&#8222;/\\glqq /g;
			# rdquor
			$line =~ s/&#8223;/\\grqq /g;
			# ldquo
			$line =~ s/&#8220;/\\textquotedblleft{}/g;
			# rdquo
			$line =~ s/&#8221;/\\textquotedblright{}/g;
		        # lsquo	
			$line =~ s/&#8216;/\`/g;
			# rsquo
			$line =~ s/&#8217;/\'/g;
			# AE
			$line =~ s/&#198;/\\AE /g;
			# EN SPACE
			$line =~ s/&#8194;/\\,/g;
		}
		print OUT $line;
	}
	close(OUT);
	close(IN);
	unlink($filetmp);
	print "LaTeX changes in: $file\n";
}

sub newtemplate {
	if ($ARGV[0] eq "bc") {
		print "creating new template: bc.xml\n";
		copy("${package_path}templates${slash}${bcthesis}",".") or die "Copy failed: $!";
		copy("${package_path}templates${slash}docbook.css",".") or die "Copy failed: $!";
		copy("${package_path}templates${slash}fi-logo.png",".") or die "Copy failed: $!";
	}
	elsif ($ARGV[0] eq "thesis") {
		print "creating new template: thesis.xml\n";
		copy("${package_path}templates${slash}${thesis}",".") or die "Copy failed: $!";
		copy("${package_path}templates${slash}docbook.css",".") or die "Copy failed: $!";
		copy("${package_path}templates${slash}fi-logo.png",".") or die "Copy failed: $!";
	}
	else {
		exit 1;
	}
}

sub convertcatalog {
	if ($perl_os eq "MSWin32") {
		$catalog_files =~ s/ /%20/g;
		$catalog_files =~ s/\\/\//g;
		$catalog_files = 'file:///'.$catalog_files;
	}	
}

sub xinclude {
my $param;
	$opt_catalog ? $param = "-r $catalogresolver" : "";
	$catalog_manager ne "" ? ($ENV{"CLASSPATH"} .= $term.$catalog_manager.$term.$resolver) : "";
	$xincluder ne "" ? ($ENV{"CLASSPATH"} .= $term.$xincluder) : "";
	$xerces_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xerces_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -f $xincluder) { print "XIncluder not installed.\n"; exit 1; }
    dow("${java} $xincluder_param $param $opt_file > ${opt_file}.xincluder");
	$opt_file = "${opt_file}.xincluder";
}

sub processor {
	eval("$opt_processor()");
}

sub xsltproc {
my $param;
	$#opt_param > -1 ? $param .= getParams(\@opt_param, " --stringparam", " ") : "";
	$opt_debug ? $param .= " --timing --version" : "";
	$opt_timing ? $param .= " --timing" : "";
	$opt_xinclude ? $param .= " --xinclude" : "";
	if ($opt_catalog and ($catalog_files ne "")) { convertcatalog(); $ENV{"XML_CATALOG_FILES"} = $catalog_files };
	$opt_debug ? (print "XML_CATALOG_FILES: ",$ENV{"XML_CATALOG_FILES"},"\n") : "";
#	if (! -f $xsltproc) { print "libxslt not installed.\n"; exit 1; }
	print "using xsltproc\n";
	dow("${xsltproc} ${param} -o $opt_out $opt_xsl $opt_file");
	$opt_latex ? changeMacro($opt_out) : "";
}

sub saxon { 
my $param_end;
my $param;
	$opt_xinclude ? &xinclude() : "";
	# Despite of Alfred parser does not correctly handle with catalog files
	$opt_catalog ? $param = " -Djavax.xml.parsers.DocumentBuilderFactory=$documentbuilder -Djavax.xml.parsers.SAXParserFactory=$saxparser" : "";
	$param .= " com.icl.saxon.StyleSheet";
	$opt_debug ? $param .= " -t -T" : "";
	$opt_timing ? $param .= " -t" : "";
	if ($opt_catalog) { $ENV{"CLASSPATH"} .= $term.$xerces_cp; $param .= " -x $xmlreader -y $xmlreader -r $catalogresolver"; }
	$#opt_param > -1 ? $param_end = getParams(\@opt_param, "", "=") : "";
	$saxon_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$saxon_cp) : "";
	$catalog_manager ne "" ? ($ENV{"CLASSPATH"} .= $term.$catalog_manager.$term.$resolver) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $saxon_home) { print "Saxon6 not installed.\n"; exit 1; }
	print "using saxon6\n";
	dow("${java}${param} -o $opt_out $opt_file $opt_xsl $param_end");
	$opt_latex ? changeMacro($opt_out) : "";
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub saxon7 {
my $param_end;
my $param = "net.sf.saxon.Transform";
	$opt_xinclude ? &xinclude() : "";
	$opt_debug ? $param .= " -t -T" : "";
	$opt_timing ? $param .= " -t" : "";
	$opt_catalog ? $param .= " -x $xmlreader -y $xmlreader -r $catalogresolver"	: "";
	$#opt_param > -1 ? $param_end = getParams(\@opt_param, "", "=") : "";
	$saxon7_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$saxon7_cp) : "";
	$catalog_manager ne "" ? ($ENV{"CLASSPATH"} .= $term.$catalog_manager.$term.$resolver) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $saxon_home) { print "Saxon7 not installed.\n"; exit 1; }
	print "using saxon7\n";
	dow("$java $param -o $opt_out $opt_file $opt_xsl $param_end");
	$opt_latex ? changeMacro($opt_out) : "";
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub saxonb8 {
my $param_end;
my $param = "net.sf.saxon.Transform";
	$opt_xinclude ? &xinclude() : "";
	$opt_debug ? $param .= " -t -T" : "";
	$opt_timing ? $param .= " -t" : "";
	$opt_catalog ? $param .= " -x $xmlreader -y $xmlreader -r $catalogresolver"	: "";
	$#opt_param > -1 ? $param_end = getParams(\@opt_param, "", "=") : "";
#	$saxonb8_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$saxonb8_cp) : "";
	$saxonb8_cp ne "" ? ($ENV{"CLASSPATH"} = $term.$saxonb8_cp) : "";
	$catalog_manager ne "" ? ($ENV{"CLASSPATH"} .= $term.$catalog_manager.$term.$resolver) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $saxonb8_home) { print "Saxonb8 not installed.\n"; exit 1; }
	print "using saxon8\n";
	dow("$java $param -o $opt_out $opt_file $opt_xsl $param_end");
	$opt_latex ? changeMacro($opt_out) : "";
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub xalan {
my $param;
	$opt_xinclude ? &xinclude() : "";
	$xalan_cp ne "" ? ($ENV{"CLASSPATH"} = $term.$xalan_cp) : "";
	$catalog_manager ne "" ? ($ENV{"CLASSPATH"} .= $term.$catalog_manager.$term.$resolver) : "";
	$param .= " org.apache.xalan.xslt.Process";
	$opt_debug ? $param .= " -V -DIAG" : "";
	$opt_timing ? $param .= " -DIAG" : "";
	$opt_catalog ? $param .= " -ENTITYRESOLVER $catalogresolver -URIRESOLVER $catalogresolver" : "";
	$#opt_param > -1 ? $param .= " ".getParams(\@opt_param, "-PARAM", " ") : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $xalan_home) { print "Xalan not installed.\n"; exit 1; }
	print "using xalan\n";
	dow("$java $param -IN $opt_file -XSL $opt_xsl -OUT $opt_out");
	$opt_latex ? changeMacro($opt_out) : "";
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub jd {
my $param = "jd.xml.xslt.Stylesheet";
	$opt_xinclude ? &xinclude() : "";
	$opt_debug ? $param .= " -verbose" : "";
	$#opt_param > -1 ? $param .= " ".getParams(\@opt_param, "-param", " ") : "";
	if ($opt_catalog) { $ENV{"CLASSPATH"} .= $term.$xerces_cp; $param .= " -parserxml $xmlreader -parserxsl $xmlreader -uriresolver $catalogresolver"; }
	$jd_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$jd_cp) : "";
	$catalog_manager ne "" ? ($ENV{"CLASSPATH"} .= $term.$catalog_manager.$term.$resolver) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	$perl_os eq "MSWin32" ? ($opt_xsl = "file:///$opt_xsl") : ($opt_xsl = "file://$opt_xsl");
	if (! -d $jd_home) { print "Jd not installed.\n"; exit 1; }
	print "using jd.xslt\n";
	dow("$java $param -out $opt_out $opt_file $opt_xsl");
	$opt_latex ? changeMacro($opt_out) : "";
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub xt {
my $param = "com.jclark.xsl.sax.Driver";
	$opt_xinclude ? &xinclude() : "";
	$opt_catalog ? $param = "com.sun.xt.xsl.sax.Driver" : "";
	$xt_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xt_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $xt_home) { print "XT not installed.\n"; exit 1; }
	print "using xt\n";
	dow("$java $param $opt_file $opt_xsl $opt_out");
	$opt_latex ? changeMacro($opt_out) : "";
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub parser {
my $param = "--noout";
	$opt_debug ? $param .= " --timing --version" : "";
	$opt_xinclude ? $param .= " --xinclude" : "";
	if ($opt_catalog and ($catalog_files ne "")) { convertcatalog(); $ENV{"XML_CATALOG_FILES"} = $catalog_files };
	$opt_debug ? (print "XML_CATALOG_FILES: ",$ENV{"XML_CATALOG_FILES"},"\n") : "";
#	if (! -f $xmllint) { print "libxml2 not installed.\n"; exit 1; }
	print "parsing\n";
	dow("$xmllint $param $opt_file");
}

sub jparser {
my $param = "sax.Counter";
	$opt_xinclude ? &xinclude() : "";
	$xerces_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xerces_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $xerces_home) { print "Xerces not installed.\n"; exit 1; }
	print "parsing\n";
	dow("$java $param $opt_file");
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub validate {
my $param = "--valid --noout";
	$opt_debug ? $param .= " --timing --version" : "";
	if ($opt_catalog and ($catalog_files ne "")) { convertcatalog(); $ENV{"XML_CATALOG_FILES"} = $catalog_files };
	$opt_debug ? (print "XML_CATALOG_FILES: ",$ENV{"XML_CATALOG_FILES"},"\n") : "";
#	if (! -f $xmllint) { print "libxml2 not installed.\n"; exit 1; }
	print "validating\n";
	dow("$xmllint $param $opt_file");
}

sub jvalidate {
my $param = "dom.Counter -v -s -f";
	$opt_xinclude ? &xinclude() : "";
	$xerces_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xerces_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	if (! -d $xerces_home) { print "Xerces not installed.\n"; exit 1; }
	print "validating\n";
	dow("$java $param $opt_file");
	if (($opt_xinclude) && (!$opt_debug)) { clean("$opt_file") };
}

sub clean {
		if ($opt_silent) {
			print "cleaning\n";
		}
		else {
			print "clean: @_ \n";
		}
		unlink @_;
}

sub pdf {
my $outtmp = getPrefix($opt_out);
my $count;
	$opt_out = correctSuffix($opt_out,"tex");
	$opt_xsl eq "" ? $opt_xsl = $fithesis : "";
#	if (! -f $pdflatex) { print "PdfLaTeX not installed.\n"; exit 1; }
	processor();
#	if (! -f $vlna) { print "vlna not installed.\n"; }
	print "using vlna\n";
	dow("$vlna $opt_out");
	print "using latex\n";
	dow("$pdflatex $outtmp");
#	if (! -f $index) { print "index not installed.\n"; exit 1; }
	print "using index\n";
	dow("$index $outtmp");
	for($count=1; $count <= $texpass; $count++) {
  		print "using latex\n";
		dow("$pdflatex $outtmp");
	}
	!$opt_debug ? clean("${outtmp}.aux","${outtmp}.idx","${outtmp}.ilg","${outtmp}.ind","${outtmp}.log","${outtmp}.out","${outtmp}.toc","${outtmp}.tfm","${outtmp}.glo","${outtmp}.te~","${outtmp}.brf","${outtmp}.lof","${outtmp}.lot") : "";
}

sub ps {
my $outtmp = getPrefix($opt_out);
my $count;
	$opt_out = correctSuffix($opt_out,"tex");
	$opt_xsl eq "" ? $opt_xsl = $fithesis : "";
	processor();
	print "using vlna\n";
	dow("$vlna $opt_out");
	print "using latex\n";
	dow("$latex $outtmp");
	print "using index\n";
	dow("$index $outtmp");
	for($count=1; $count <= $texpass; $count++) {
		print "using latex\n";
		dow("$latex $outtmp");
	}
	print "using dvips\n";
  dow("$dvips -o ${outtmp}.ps ${outtmp}.dvi");
	!$opt_debug ? clean("${outtmp}.aux","${outtmp}.bbl","${outtmp}.blg","${outtmp}.dvi","${outtmp}.idx","${outtmp}.ilg","${outtmp}.ind","${outtmp}.log","${outtmp}.out","${outtmp}.toc","${outtmp}.tfm","${outtmp}.glo","${outtmp}.te~","${outtmp}.brf","${outtmp}.lof","${outtmp}.lot") : "";
}

sub dvi {
my $outtmp = getPrefix($opt_out);
my $count;
	$opt_out = correctSuffix($opt_out,"tex");
	$opt_xsl eq "" ? $opt_xsl = $fithesis : "";
	processor();
	print "using vlna\n";
	dow("$vlna $opt_out");
	print "using latex\n";
	dow("$latex $outtmp");
	print "using index\n";
	dow("$index $outtmp");
	for($count=1; $count <= $texpass; $count++) {
   	print "using latex\n";
		dow("$latex $outtmp");
	}
	!$opt_debug ? clean("${outtmp}.aux","${outtmp}.bbl","${outtmp}.blg","${outtmp}.idx","${outtmp}.ilg","${outtmp}.ind","${outtmp}.log","${outtmp}.out","${outtmp}.toc","${outtmp}.tfm","${outtmp}.glo","${outtmp}.te~","${outtmp}.brf","${outtmp}.lof","${outtmp}.lot") : "";
}

sub xslrtf {
my $outrtf = $opt_out;
my $param = "com.xmlmind.fo.converter.Driver";
	$opt_out = getPrefix($opt_out).".fo";
	$opt_xsl eq "" ? $opt_xsl = $fo : "";
	processor();
	$outrtf = correctSuffix($outrtf,"rtf");
	$xfc_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xfc_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	print "using xfc\n";
	dow("$java $param -$xfcparam $opt_out $outrtf");
	!$opt_debug ? clean("$opt_out") : "";
}

sub win2utf {
	print "converting from WINDOWS-1250 to UTF-8\n";
	dow("$convertor -f WINDOWS-1250 -t UTF-8 $opt_file > $opt_out");
	my $line;
	my ($filetmp) = "$opt_out.xslttmp";
	rename($opt_out,$filetmp);
	open(IN,"$filetmp") or die "Cannot open file: $filetmp!\n";
	open(OUT,">$opt_out") or die "Cannot open file: $opt_out!\n";
	while ( $line = <IN> ) {
		$line =~ s/^\s*(<\?xml.*encoding\s*=\s*["'])[Ww][Ii][Nn][Dd][Oo][Ww][Ss]-1250(["']\?>)\s*$/$1UTF-8$2/g;
		print OUT $line;
	}
	close(OUT);
	close(IN);
	unlink($filetmp);
	print "XML header changed to UTF-8\n";
}

sub iso2utf {
	print "converting from ISO-8859-2 to UTF-8\n";
	dow("$convertor -f ISO-8859-2 -t UTF-8 $opt_file > $opt_out");
	my $line;
	my ($filetmp) = "$opt_out.xslttmp";
	rename($opt_out,$filetmp);
	open(IN,"$filetmp") or die "Cannot open file: $filetmp!\n";
	open(OUT,">$opt_out") or die "Cannot open file: $opt_out!\n";
	while ( $line = <IN> ) {
		$line =~ s/^\s*(<\?xml.*encoding\s*=\s*["'])[Ii][Ss][Oo]-8859-2(["']\?>)\s*$/$1UTF-8$2/g;
		print OUT $line;
	}
	close(OUT);
	close(IN);
	unlink($filetmp);
	print "XML header changed to UTF-8\n";
}

sub math {
my $outtmp = getPrefix($opt_file);
	print "converting math\n";
	if ($convert) { dow ("$perl ${run_path}$convert $opt_file $convertparam"); }
	else {
		dow("$latex $opt_file");
		dow("$dvi2bitmap -t $bmtype ${outtmp}.dvi");
		!$opt_debug ? clean("${outtmp}.dvi","${outtmp}.log","${outtmp}.aux") : "";
	}
}

sub foppdf {
my $param = "org.apache.fop.apps.Fop";
my $outtmp = getPrefix($opt_out);
	$opt_out = correctSuffix($opt_out,"fo");
	$opt_xsl eq "" ? $opt_xsl = $fofop : "";
	processor();
	$fop_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$fop_cp) : "";
	if ($opt_debug) { print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n"; $param .= " -d"; }
	print "using fop\n";
	dow("$java $param -c $fop_config -fo $opt_out -pdf $outtmp.pdf");
	!$opt_debug ? clean("$opt_out") : "";
}

sub xeppdf {
my $param = "com.renderx.xep.XSLDriver";
my $outtmp = getPrefix($opt_out);
	$opt_out = correctSuffix($opt_out,"fo");
	$opt_xsl eq "" ? ($opt_xsl = $foxep) : "";
	processor();
	$xep_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xep_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	print "using xep\n";
	dow("$java $param -DCONFIG=$xep_config -fo $opt_out -pdf $outtmp.pdf");
	!$opt_debug ? clean("$opt_out") : "";
}

sub xepps {
my $param = "com.renderx.xep.XSLDriver";
my $outtmp = getPrefix($opt_out);
	$opt_out = correctSuffix($opt_out,"fo");
	$opt_xsl eq "" ? ($opt_xsl = $foxep) : "";
	processor();
	$xep_cp ne "" ? ($ENV{"CLASSPATH"} .= $term.$xep_cp) : "";
	$opt_debug ? (print "CLASSPATH: ",$ENV{"CLASSPATH"},"\n") : "";
	print "using xep\n";
	dow("$java $param -DCONFIG=$xep_config -fo $opt_out -ps $outtmp.ps");
	!$opt_debug ? clean("$opt_out") : "";
}

sub xmltexpdf {
my $param = "--interaction nonstopmode";
	print "using pdfxmltex\n";
	dow("$pdfxmltex $param \"&pdfxmltex\" $opt_file");
	print "using pdfxmltex\n";
	dow("$pdfxmltex $param \"&pdfxmltex\" $opt_file");
}

__END__
