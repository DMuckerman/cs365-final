#!/usr/bin/perl
use strict;
use warnings;
use Math::Complex;

sub matvec {
	my ($x, $a) = @_;
	$x = shift;			# Vector to multiply by
	$a = shift;			# Matrix to multiply

	my $n = scalar @$x;		# n for n*n matrix, and n long vector

	my @arr = ((0) x $n);	# Empty vector of size n to hold product

	# Loop through each row
	for (my $i = 0; $i < $n; $i++ ) {

		# Loop through each column
		# TODO: Throw this into MPI threads
		for (my $j = 0; $j < $n; $j++ ) {
			# Multiply matrix cell by requisite vector cell
			# And add it to the existing row total
			$arr[$i] = $arr[$i] + (@$a[($i * $n) + $j] * @$x[$j]);
		}
	}

	# Print final result
	print "@arr ";
	print "\n";
}

my @arr = (5, 0, 0, 0,
		   0, 1, 0, 0,
		   0, 0, 2, 0,
		   0, 0, 0, 1);

my @vec = (1, 0, 0, 2);

matvec(\@vec, \@arr);