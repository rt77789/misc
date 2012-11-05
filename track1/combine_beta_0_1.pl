#!/usr/bin/perl -w
#
#
#
#

my $dir = '../';
open CR, "<$dir/eval_beta_1_new.csv" or die "open $dir/eval_beta_1_new fail.";
open BETA, "<$dir/eval_beta_0.csv" or die "open $dir/eval_beta_0 fail.";
print <CR>;
<BETA>;
my $common = 0;
my $lnum = 0;

while(my $l1 = <CR>) {
    my $l2 = <BETA>;
    my ($uid1) = $l1 =~ /^(\d+)\s+/;
    my ($uid2) = $l2 =~ /^(\d+)\s+/;
    die "$uid1, $uid2\n" if $uid1 ne $uid2;

    $l1 =~ s/^.*?\s+//g;
    $l2 =~ s/^.*?\s+//g;
    my @t1 = split /,/, $l1;
    my @t2 = split /,/, $l2;

    print "$uid1 $t1[0],$t1[1],";
    
    my $i = 0;
    while($i < 3) {
        last if($t2[$i] ne $t1[0] && $t2[$i] ne $t1[1]);
        ++$i;
    }

    if($i == 3) {
        die "$i == 3\n";
    }
   

    ++$lnum;
}
#print "lnum: $lnum, common: $common, ratio: ", $common / ($lnum * 3), " \n";

close BETA;

close CR;

