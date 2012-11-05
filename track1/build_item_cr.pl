#!/usr/bin/perl -w
#
#
# 用途：计算click ratio信息，保存到文件。
# 根据向彪哥请教结果,得知应该把每个类别均值作为prior来计算.
#
#
#
my %ugen;
my $dir = '../data/';

sub load_user_gender {
    open GEN, "<$dir/user_profile.txt" or die 'open $dir/user_profile.txt fail.';
    while(<GEN>) {
        chomp;
        my @tk = split /\s+/;

        ## 设置不合法的年龄和性别为未知.

        $tk[1] = 0 unless $tk[1] =~ m{^\d+$};

        $tk[1] = 0 if $tk[1] < 1960 || $tk[1] > 2000;
        $tk[2] = 0 if $tk[2] != 1 && $tk[2] != 2;

        $ugen{$tk[0]} = \@tk;
    }
    close GEN;
}

sub load_item_cate {
    ## 读取item的类别信息.
    my %iid2icate;

    open IC, "<$dir/item.txt" or die "open $dir/item.txt fail.";
    while(<IC>) {
        # iid icate xxx
        chomp;
        my ($iid, $icate) = split /\s+/;
        $iid2icate{$iid} = $icate;
    }
    close IC;
    return \%iid2icate;
}
sub sort_by_click_ratio {
    ## 未知性别用户用整体信息的cr来推荐.
    my %ihash;
    my %mhash;
    my %fhash;

    my $iid2icate = &load_item_cate;

    while(<stdin>) {
        #my ($uid, $iid, $res, $stamp) = m{^(\d+)\s+(\d+)\s+(.*?)\s+(\d+)$};
        my ($uid, $iid, $res) = m{^(\d+)\s+(\d+)\s+(.*?)$};


        $ihash{$iid}->[0] ||= 0;# if $res > 0;
        $ihash{$iid}->[0]++ if $res > 0;
        $ihash{$iid}->[1]++;
        $ihash{$iid}->[2] = $iid2icate->{$iid};

        unless(defined $ugen{$uid}) {

        }
        elsif ($ugen{$uid}->[2] == 1) {
            $mhash{$iid}->[0] ||= 0;# if $res > 0;
            $mhash{$iid}->[0]++ if $res > 0;
            $mhash{$iid}->[1]++;
            $mhash{$iid}->[2] = $iid2icate->{$iid};
        }
        elsif ($ugen{$uid}->[2] == 2) {
            $fhash{$iid}->[0] ||= 0; #if $res > 0;
            $fhash{$iid}->[0]++ if $res > 0;
            $fhash{$iid}->[1]++;
            $fhash{$iid}->[2] = $iid2icate->{$iid};
        }
        else {
        }

        # print "$uid | $iid | $res | $stamp\n";
    }

    &store_cr(\%ihash, "$dir/entire_item_cr.info", $iid2icate);    
    &store_cr(\%mhash, "$dir/male_item_cr.info", $iid2icate);    
    &store_cr(\%fhash, "$dir/female_item_cr.info", $iid2icate);    
}

sub store_cr {
    my $hash = $_[0];
    my $file = $_[1];
    my $iid2icate = $_[2];

    ## 计算先验,根据每个类别的平均ratio.

    my %prior;

    for my $id (keys %{$hash}) {
        $prior{$iid2icate->{$id}}->[0] ||= 0;# unless defined $prior{$iid2icate->{$id}}->[0];
        $prior{$iid2icate->{$id}}->[1] ||= 0;# unless defined $prior{$iid2icate->{$id}}->[1];

        $prior{$iid2icate->{$id}}->[0] += $hash->{$id}->[0];
        $prior{$iid2icate->{$id}}->[1] += $hash->{$id}->[1];
    }

	my ($pa, $pb) = (0, 0);

    ## 输出每个类别的prior信息,看看是否程序有bug.
    my ($pa, $pb) = (0, 0);
    for (keys %prior) {
		$pa += $prior{$_}->[0];
		$pb += $prior{$_}->[1];

        print STDERR "$_,$prior{$_}->[0],$prior{$_}->[1]\n";
        $pa += $prior{$_}->[0];
        $pb += $prior{$_}->[1];
    }

    my $smooth = 6095; ## 如果结果不好换成0再试试.
    for (keys %{$hash}) {
        #if ($hash->{$_}->[0] < 1000) {
            $hash->{$_}->[0] += 1; # + $prior{$hash->{$_}->[2]}->[0] + $pa; 
            $hash->{$_}->[1] += $smooth; # + $prior{$hash->{$_}->[2]}->[1] + $pb; 
            #}
    }

    open ECR, ">$file" or die "open $file fail.";

    #my $right = ($hash->{$b}->[0] + 1) / ($hash->{$b}->[1] + $smooth);     
    #my $left = ($hash->{$a}->[0] + 1) / ($hash->{$a}->[1] + $smooth);
    #$right <=> $left;

    for (sort { ($hash->{$b}->[0] / $hash->{$b}->[1]) <=> ($hash->{$a}->[0] / $hash->{$a}->[1]) } keys %{$hash}) {
        print ECR "$_ ", $hash->{$_}->[0], ",", $hash->{$_}->[1],",", ($hash->{$_}->[0]
            )/($hash->{$_}->[1]), "\n";
    }
    close ECR;
}

sub main {
    &load_user_gender;
    &sort_by_click_ratio;
}

&main;
