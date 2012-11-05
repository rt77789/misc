#!/usr/bin/perl -w
#
#
#
# 用途：预测rec_log_test文件。
#
#

my %cr;
my %uitem;
my %uneib;
my %gender;

my $dir = '../data/';

## 读入用户性别信息.
sub load_gender {
    open GE, "<$dir/user_profile.txt" or die "open $dir/user_profile.txt fail.";
    while(<GE>) {
        chomp;
        my @tk = split /\s+/;
        $tk[2] = 0 if $tk[2] != 1 && $tk[2] != 2;
        $gender{$tk[0]} = $tk[2];
    }
    close GE;
}

## 读入cr信息。
sub load_cr {
    my $path = $_[0];
    my $index = $_[1];

    #open CR, "<$dir/entire_item_cr.info" or die "open $dir/entire_item_cr.info fail.";

    open CR, "<$path" or die "open $path fail.";
    while(<CR>) {
        my ($uid, $tk) = split /\s+/;
        my @tks= split /,/, $tk;

        $cr{$uid}->[$index] = $tks[-1];
    }
    close CR;
}

sub load_uneighbors {
    open NE, "<$dir/user_neighbor.info" or die "open $dir/user_neighbor.info fail.";
    <NE>;
    while(<NE>) {
        chomp;
        my ($u, @ne) = split /\s+/;
        $uneib{$u} = \@ne;
    }
    close NE;
}

sub load_uitem {
    open IU, "<$dir/item_user.sorted" or die "open $dir/item_user.sorted fail.";
    while(<IU>) {
        my ($iid, $tk) = split /\s+/;
        for my $uid ( split /,/, $tk) {
            next if $uid =~ /^\s*$/;
            #print "[$iid], [$uid]\n";
            $uitem{$iid}->{$uid}++;
        }
    }
    close IU;
}

## 计算w(u,n)，返回相关度。
sub cal_w {
    my $u = $_[0];
    my $n = $_[1];

    return 1;
}

## 计算r(n,i)，返回相关读。
sub cal_r {
    my $n = $_[0];
    my $i = $_[1];

    return 1;
}

sub get_neighbors {
    my $u = $_[0];

    $uneib{$_[0]};
}

##
sub cal_score {
    my $u = $_[0];
    my $i = $_[1];

    ## get neighbors of u.
    my $neibs = &get_neighbors($u);
    #my $score = $cr{$i}->[0] || 0;
    my $score = $cr{$i}->[$gender{$u}] || 0;
    my $neib_num = defined $neibs ? @{$neibs} : 0;

    for my $n (@{$neibs}) {
        next unless defined $uitem{$i}->{$n};
        $score += (&cal_w($u, $n) * &cal_r($n, $i) * $cr{$i}->[$gender{$u}]) / $neib_num;
    }
    return $score;
}

sub process {
    while(<stdin>) {
        chomp;
        my ($uid, $iid, $label, $ts) = split /\s+/;
        
        my $score = &cal_score($uid, $iid);

        print "$uid $iid $score $ts\n";
    }
}

sub main {
    &load_gender;
    &load_uitem;

    &load_cr("$dir/entire_item_cr.info", 0);
    &load_cr("$dir/male_item_cr.info", 1);
    &load_cr("$dir/female_item_cr.info", 2);
        
    &load_uneighbors;

    &process;
}

&main;

