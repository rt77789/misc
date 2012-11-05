#!/usr/bin/perl -w
#
#
# 将user_sns文件格式转化为 followee - follower. 原来的格式是 follower - followee.
# 然后shell: sort .
#
my $tmp = <stdin>;
print $tmp;
while(<stdin>) {
    chomp;
    my @a = split /\s+/;
    print "$a[1] $a[0]\n";
}
