#!/usr/bin/perl -w
#
#
# 统计性别和年龄信息.
#

## load user_profile.
#
my $dir = '../';
my %ups;

open UP, "<$dir/user_profile.txt" or die "open $dir/user_profile.txt fail.";
while(<UP>) {
    my @tk = split /\s+/;
    if($tk[1] =~ m{^\d+$}) {
    }
    else {
        $tk[1] = 0;
    }

    $up{$tk[0]} = \@tk; #->[0] = $tk[1];
}
close UP;

my @res;
my $lnum = 0;
while(<stdin>) {
    chomp;
    my ($iid, $tk) = split /\s+/;
    my @gender;

    for (0..10) {
        $gender[$_] = 0;
    }
    my @uss = split /,/;
    my $num = @uss;

    #print $num;
    my $str = $num;
    my $tnum = 0;
    for my $uid (@uss) {

        if(defined $up{$uid}->[1]) {
            
            $gender[$up{$uid}->[2]]++;

            #print " ", $up{$uid}->[0] < 1960 || $up{$uid}->[0] > 2000 ? 0 : $up{$uid}->[0], " $up{$uid}->[1]";
            $str .= " " . ($up{$uid}->[1] < 1960 || $up{$uid}->[1] > 2000 ? 0 : $up{$uid}->[1])." $up{$uid}->[2]";
            $tnum++;
        }
        else {
        }
    }
    #print "\n";
    $str .= "\n";
    $res[$lnum]->[0] = $tnum;
    $res[$lnum]->[1] = $iid;
    $res[$lnum]->[2] = $str;

    my $total = $gender[0] + $gender[1] + $gender[2];
    print STDERR "$tnum, male: $gender[1], female: $gender[2], unknown: $gender[0]\n";
    ++$lnum;
}

for my $c (sort { $b->[0] <=> $a->[0] } @res) {
    print "$c->[1] $c->[2]";
}
