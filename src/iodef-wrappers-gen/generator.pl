#!/usr/bin/perl -w
#
# Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
# Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
#
# This file is part of the Prelude library.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

use strict;

use IODEFTree;
use GenerateDebug;
use GenerateIODEFTreeWrapC;
use GenerateIODEFTreeWrapH;
use GenerateIODEFTreeData;
use GenerateIODEFMessageIdH;
use GenerateIODEFMessageWriteC;
use GenerateIODEFMessageWriteH;
use GenerateIODEFMessagePrintJSONC;
use GenerateIODEFMessagePrintJSONH;
use GenerateIODEFMessageReadJSONC;
use GenerateIODEFMessagePrintC;
use GenerateIODEFMessagePrintH;
use GenerateIODEFValueClassSwigMapping;
use GenerateIODEFTreeWrapCxx;
use GenerateIODEFTreeWrapHxx;


sub	target_need_update
{
    my	$generator = shift;
    my	@file_list = @_;
    my	$source = $generator->{source};
    my	$target = $generator->{target};
    my	$source_time = (stat $source)[9];
    my	$target_time = (stat $target)[9];

    foreach my $file ( @file_list ) {
	return 1 if ( (stat $file)[9] > $target_time );
    }

    return ($source_time > $target_time);
}

sub	target_update
{
    my	$generator = shift;
    my	$iodef_tree = shift;
    my	$target = $generator->{target};
    my	$func = $generator->{func};

    $generator = &$func($target);
    $iodef_tree->process($generator);
}

my $iodef_tree;
my $generator;
my @generator_list = ({ source => 'GenerateIODEFTreeWrapC.pm', 
			target => '../iodef-tree-wrap.c',
			func => sub { new GenerateIODEFTreeWrapC(-filename => shift) } },

		      { source => 'GenerateIODEFTreeWrapH.pm',
			target => '../include/iodef-tree-wrap.h',
			func => sub { new GenerateIODEFTreeWrapH(-filename => shift) } },

		      { source => 'GenerateIODEFTreeData.pm',
			target => '../include/iodef-tree-data.h',
			func => sub { new GenerateIODEFTreeData(-filename => shift) } },

		      { source => 'GenerateIODEFMessageWriteC.pm',
			target => '../iodef-document-write.c',
			func => sub { new GenerateIODEFMessageWriteC(-filename => shift) } },

		      { source => 'GenerateIODEFMessageWriteH.pm',
			target => '../include/iodef-document-write.h',
			func => sub { new GenerateIODEFMessageWriteH(-filename => shift) } },

		      { source => 'GenerateIODEFMessagePrintC.pm',
			target => '../iodef-document-print.c',
			func => sub { new GenerateIODEFMessagePrintC(-filename => shift) } },

		      { source => 'GenerateIODEFMessagePrintH.pm',
			target => '../include/iodef-document-print.h',
			func => sub { new GenerateIODEFMessagePrintH(-filename => shift) } },

		      { source => 'GenerateIODEFMessagePrintJSONC.pm',
			target => '../iodef-document-print-json.c',
			func => sub { new GenerateIODEFMessagePrintJSONC(-filename => shift) } },

		      { source => 'GenerateIODEFMessageJSONH.pm',
			target => '../include/iodef-document-print-json.h',
			func => sub { new GenerateIODEFMessageJSONH(-filename => shift) } },

		      { source => 'GenerateIODEFMessageReadJSONC.pm',
			target => '../iodef-document-read-json.c',
			func => sub { new GenerateIODEFMessageReadJSONC(-filename => shift) } },

		      { source => 'GenerateIODEFMessageIdH.pm',
			target => '../include/iodef-document-id.h',
			func => sub { new GenerateIODEFMessageIdH(-filename => shift) } },

		      { source => 'GenerateIODEFValueClasstSwigMapping.pm',
			target => '../../bindings/low-level/iodef-value-class-mapping.i',
			func => sub { new GenerateIODEFValueClassSwigMapping(-filename => shift) } },

		      { source => 'GenerateIODEFTreeWrapCxx.pm',
			target => '../../bindings/c++/iodef-tree-wrap.cxx',
			func => sub { new GenerateIODEFTreeWrapCxx(-filename => shift) } },

                      { source => 'GenerateIODEFTreeWrapHxx.pm',
                        target => '../../bindings/c++/include/iodef-tree-wrap.hxx',
                        func => sub { new GenerateIODEFTreeWrapHxx(-filename => shift) } },

		      );

$iodef_tree = new IODEFTree(-filename => "iodef-tree.h",
			    -debug => 0);
$iodef_tree->load();

foreach my $generator (@generator_list) {
    target_need_update($generator, 'iodef-tree.h', 'IODEFTree.pm') and target_update($generator, $iodef_tree);
}
