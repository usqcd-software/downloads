#!/usr/bin/perl
#
# Use this script to generate the index.html web-pages for all the tarballs in this repo
#

# These are the canonical directories to be searched
foreach my $dir ("bagel_qdp", "c-lime", "qdp", "qdp++", "qio", "qla", "qmp", "qmt", "qopqdp")
{
  chdir($dir);

#  my $big = tr/a-z/A-Z/;
  my $big = $dir;

  # Start the page
  open(HTML, "> index.html");
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
  open(HTML, ">> index.html");
  open(FILES, "find . -name '*.tar.gz' -print |");

  while(my $file = <FILES>)
  {
    chomp $file;
    $file =~ s/^\.\///;

    printf HTML "<a href=\"$file\">$file</a><br>\n";
  }

  close(FILES);
  close(HTML);

  # Finish the page
  open(HTML, ">> index.html");
  print HTML<<EOF;
</p>

<p>
<a href="http://usqcd-software.github.io">Return to main USQCD software page.</a>

</body>
</html>
EOF

  close(HTML);
  chdir("..");
}

exit(0);
