#!/usr/bin/perl
use strict;
use warnings;

# some constants/variables we need
my $template_name = "TEMPLATE";
my $target_name = $ARGV[0];
my $name_checker = "^[a-z][a-z0-9-]*\$"; # starts with lowercase letter
					# and followed by alphanumeric or -
my $pwd = `pwd`;

# verify correct parameters
print_usage() unless $#ARGV == 0;
print_usage() unless $target_name =~ /$name_checker/;
print_usage() if `svn st | grep -v "^\?"` ne "";

# make sure SVN is up to date
system ("svn up") == 0 || die "could not update SVN: $?";

# make a target directory
mkdir $target_name || die "could not create directory $target_name: $?";
# find all the files we have to copy
open FILELIST, "svn ls -R $template_name |";
while(<FILELIST>)
{
  chomp;
  next if /\/$/;  # ignore directories
  
  # get the names of the source and target files
  my $source_file = "$template_name/$_";
  my $target_file = $source_file;
  $target_file =~ s/$template_name/$target_name/g;
  
  # execute the svn copy
  my $command = "cp '$source_file' '$target_file'";
#  print "$command\n"; # echo to the user
  system ($command) == 0 or abort("unable to execute $command: $?");
  
  # process each file's contents by replacing renaming $temlate_name to $target_name
  $command = "perl -i -pe 's/$template_name/$target_name/g' '$target_file'";
#  print "$command\n"; # echo to the user
  system($command) == 0 or abort("unable to execute $command: $?");
}

# we're done, so commit the changes and exit.
exec ("svn add $target_name; svn ci -m 'created a new folder for project $target_name'");
#exec "svn import $target_name https://$target_name.googlecode.com/svn/trunk/ -m 'importing project from TEMPLATE'";

sub print_usage
{
  print <<EOT;
Usage:
$0 <target-name>
    <target-name> must exactly match your Google Code project name. It has to
    start with a lowercase letter and be followed by any number of lowercase
    letters, digits, or -

Also, make sure that your working copy is clean. E.g, updated and no local
modifications.
EOT
  exit -1;
}

sub abort
{
  print "uh-oh, something went wrong!\n@_\nReverting all changes\n\n";
  system ("svn revert -R $target_name");
  system ("rm -rf $target_name");
  exit -1
}
