# make.pl
#
# Use this to build zebra on any standard *nix with GCC
#
# Use at your own risk. I use Perl instead of autotools because the latter
#    are an opaque morass of Mad Hatter gibberish.
#
#   USAGE:
#
#   Build all:
#       perl make.pl
#
#   Clean:
#       perl make.pl clean

unless ($#ARGV > -1) {
    make_all();
}
else{
    if($ARGV[0] eq "clean"){
        clean();
    }
    elsif($ARGV[0] eq "libs"){
        libs();
    }
    elsif($ARGV[0] eq "all"){
        make_all();
    }
}

sub make_all{
    libs();
    build();
}

sub libs{
    `mkdir -p obj`;
    chdir "src";
    `g++ -c *.cpp -std=c++11`;
    `mv *.o ../obj`;
    chdir "../";
}

sub build{
    `mkdir -p bin`;
    my @objs = `ls obj`;
    my $obj_string = "";
    for(@objs){ chomp $_; $obj_string .= "obj/$_ " };
    my $build_string =
        "g++ -std=c++11 test/main.cpp $obj_string -Isrc -o bin/test";
    `$build_string`;
}

sub clean{
    `rm -rf obj`;
    `rm -rf bin`;
}


