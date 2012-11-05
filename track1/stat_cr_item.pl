#!/usr/bin/perl -w
#
#
#
#

my $dir = '../';
open CR, "<$dir/eval_beta_1_new.csv" or die "open $dir/eval_beta_1_new fail.";
open BETA, "<$dir/eval_beta_0.csv" or die "open $dir/eval_beta_0 fail.";
<CR>;<BETA>;
my $common = 0;
my $lnum = 0;

while(my $l1 = <CR>) {
    my $l2 = <BETA>;
    $l1 =~ s/^.*?\s+//g;
    $l2 =~ s/^.*?\s+//g;
    my @t1 = split /,/, $l1;
    my @t2 = split /,/, $l2;
    
    for my $v1 (@t1) {
        for my $v2 (@t2) {
            if($v1 eq $v2) {
                $common++;
                last;
            }
        }
    }
    ++$lnum;
}
print "lnum: $lnum, common: $common, ratio: ", $common / ($lnum * 3), " \n";

close BETA;

close CR;

