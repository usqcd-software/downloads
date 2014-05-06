#!/usr/bin/perl
#
# Use this script to generate the index.html web-pages for all the tarballs in this repo
#

# These are the canonical directories to be searched
foreach my $dir ("bagel_qdp", "c-lime", "qdp", "qdp++", "qio", "qla", "qmp", "qmt", "qopqdp", "qhmc")
{
  chdir($dir);

#  my $big = tr/a-z/A-Z/;
  my $big = $dir;

  # Start the page (use new temporary filename __index.html)
  open(HTML, "> __index.html");
  print HTML<<EOF;
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>$big - Downloads</title>
</head>

<body>

<h1>$big Downloads</h1>

<p>
EOF

  # Simple method - look for *.tar.gz files. This will even support sub-directories.
  open(FILES, "find . -name '*.tar.gz' -print |");

  @files = ();
  while(my $file = <FILES>) {
    chomp $file;
    $file =~ s/^\.\///;
    push @files, $file;
  }
  close(FILES);

  # sort them, trying to be smart about version numbers
  sub pad0($) {
    my $f = shift(@_);
    $f =~ s/([^0-9])([0-9][^0-9])/${1}0$2/g;
    $f =~ s/([^0-9])([0-9][^0-9])/${1}0$2/g;
    #print $f, "\n";
    return $f;
  }
  printf HTML "<table>\n";
  for my $file (sort { pad0($a) cmp pad0($b) } @files) {
    $date = `TZ=UTC tar ztvf $file |awk '{print\$4,\$5}' |sort |tail -n1`;
    chomp $date;
    printf HTML "<tr><td><a href=\"$file\">$file</a></td><td>$date</td></tr>\n";
  }
  printf HTML "</table>\n";

  # Finish the page
  print HTML<<EOF;
</p>

<p>
<a href="http://usqcd-software.github.io">Return to main USQCD software page.</a>

</body>
</html>
EOF

  close(HTML);

  # copy new to old if different
  `cmp __index.html index.html || cp -a __index.html index.html`;
  unlink "__index.html";

  chdir("..");
}

exit(0);
