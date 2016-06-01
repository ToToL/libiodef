#!/usr/bin/env perl

use LibIodef;

sub log_func {
	($level, $str) = @_;
	print("log: " . $str);
}

sub print_array($)
{
    my $arrRef = shift;
    for (my $i = 0; $i < scalar(@$arrRef); ++$i) {
        if (ref($arrRef->[$i]) eq 'ARRAY') {
            print ', ' if ($i);
            print '[';
            print_array($arrRef->[$i]);
            print ']';
        } else {
            print ', ' if ($i);
            print $arrRef->[$i];
        }
    }
    print ' ' if (!scalar(@$arrRef));
}


LibIodef::LibIodefLog::setCallback(\&log_func);

$iodef = new LibIodef::IODEF;

print "*** IODEF->Set() ***\n";
$iodef->set("alert.classification.text", "My Message");
$iodef->set("alert.source(0).node.address(0).address", "s0a0");
$iodef->set("alert.source(0).node.address(1).address", "s0a1");
$iodef->set("alert.source(1).node.address(0).address", "s1a0");
$iodef->set("alert.source(1).node.address(1).address", "s1a1");
$iodef->set("alert.source(1).node.address(2).address", undef);
$iodef->set("alert.source(1).node.address(3).address", "s1a3");
print $iodef->toString();


print "\n*** IODEF->Get() ***\n";
print $iodef->get("alert.classification.text") . "\n";

print "\n\n*** Value IODEF->Get() ***\n";
print $iodef->get("alert.classification.text");

print "\n\n*** Listed Value IODEF->Get() ***\n";
print_array($iodef->get("alert.source(*).node.address(*).address"));

print "\n\n*** Object IODEF->Get() ***\n";
print $iodef->get("alert.source(0).node.address(0)");

print "\n\n*** Listed Object IODEF->Get() ***\n";
print_array($iodef->get("alert.source(*).node.address(*)"));


open FILE, ">foo.bin" or die "arg";
$iodef->write(FILE);
close FILE;

print "\n\n*** IODEF->Read() ***\n";
open FILE2, "<foo.bin" or die "arg2";
my $iodef2 = new LibIodef::IODEF;
while ( $iodef2->read(FILE2) ) {
	print $iodef2->toString();
}

close FILE2;

print "\n*** Client ***";
$client = new LibIodef::ClientEasy("libiodef-lml");
$client->start();

$client->sendIODEF($iodef);
