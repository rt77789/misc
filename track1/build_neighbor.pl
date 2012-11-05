#!/usr/bin/perl
#
# 将user_sns信息转化为简单格式，即：
#
# userid: neighbor1 neighbor2 neighbor3 ...
#
#
# 用途：将每个用户的neighbor信息引入计算。
#
#
#

my $lastid;
my %nhash;
my $max_nnum = 0;
my $total_nnum = 0;
my $user_num = 0;

while(<stdin>) {
    chomp;
    my ($uid, $nid) = split /\s+/;
    $lastid ||= $uid; 

    if($lastid ne $uid) {
        print $lastid;
        my $nnum = 0;
        for my $n (sort keys %nhash) {
            print " $n";
            $nnum++;
        }
        $max_nnum = $max_nnum > $nnum ? $max_nnum : $nnum;
        #print " $_" for (sort keys %nhash);
        print "\n";
        undef %nhash;
        $lastid = $uid;

        ++$user_num;
    }
    $total_nnum++;
    $nhash{$nid}++;
}

++$user_num;
print $lastid;

my $nnum = 0;
for my $n (sort keys %nhash) {
    print " $n";
    $nnum++;
}
$max_nnum = $max_nnum > $nnum ? $max_nnum : $nnum;

print "\n";

print STDERR "max_neighbor_num: $max_nnum, ave_neighbor_num: ", $total_nnum / $user_num, "\n";
