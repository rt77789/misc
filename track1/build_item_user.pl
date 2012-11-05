#!/usr/bin/perl -w
#
#
#
# 用途：构建item->user 数据，每一行对应一个item，对应user用','分割。
#

my %users;
my $lsitem;

while(<stdin>) {
    chomp;
    my ($item, $user, $label) = split /\s+/;
    $lsitem ||= $item;

    if($lsitem ne $item) {
        print "$lsitem ", join(',', sort keys %users), "\n";
        undef %users;
        $lsitem = $item; 
    }
    $users{$user}++ if $label == 1;
}
print "$lsitem ", join(',', sort keys %users), "\n";
