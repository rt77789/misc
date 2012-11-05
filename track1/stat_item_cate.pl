#!/usr/bin/perl -w
#
#
#
#
#
# 统计item 类别信息,看看每个类别的分布.
#
#
#
my %cates;

while(<stdin>) {
    chomp;
    my ($iid, $icate) = split /\s+/;
    $cates{$icate}->[0]++;
    $cates{$icate}->[1] = "" unless defined $cates{$icate}->[1];
    $cates{$icate}->[1] = $cates{$icate}->[1]." ".$iid;
}

print "$_,$cates{$_}->[0],$cates{$_}->[1]\n" for (sort keys %cates);

