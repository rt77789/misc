#!/usr/bin/perl -w
#
#
# 用途：通过predict文件，构建eval文件。

my $dir = '../data/';

sub build_eval {

    my %uhash;
    my $last;
    my %outhash;

    while(<stdin>) {
        chomp;
        my ($uid, $iid, $pv, $tstamp) = split /\s+/;
        $last = $uid unless defined $last;

        if($last ne $uid) {
            my $i = 0;
            $outhash{$last} = "$last,";

            for (sort { $uhash{$b} <=> $uhash{$a} } keys %uhash) {
                last if $i >= 3;
                $outhash{$last} .= " $_";
                ++$i;
            }
            $outhash{$last} .= "\n";

            undef %uhash;
            $last = $uid;
        }
        $uhash{$iid} = $pv;
    }
    my $i = 0;
    $outhash{$last} = "$last,";

    for (sort { $uhash{$b} <=> $uhash{$a} } keys %uhash) {
        last if $i >= 3;
        $outhash{$last} .= " $_";
        ++$i;
    }
    $outhash{$last} .= "\n";

#    print "$lsuid,", join(" ", $uipt[0]->[1], $uipt[1]->[1], $uipt[2]->[1]), "\n";

    open BS, "<$dir/sub_small_header.csv" or die "open $dir/sub_small_header.csv fail.";
    my $tl = <BS>;
    print $tl;
    while(my $line = <BS>) {
        chomp($line);
        my ($bid) = split /,/, $line;

        print $outhash{$bid};
    }
    close BS;
}

&build_eval;
