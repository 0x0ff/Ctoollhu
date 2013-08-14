#!/usr/bin/env perl

# Un script www.0x0ff.info, aout 2013
# http://thanat0s.trollprod.org/2013/08/non-mais-allow-quoi/

use strict;
use warnings;
use WWW::Curl::Easy;

# Usage...
if (@ARGV != 1) {
  print("Usage: lbf5_backender.pl [url]\n");
  exit;
}

# Variables generales
my $url=$ARGV[0];
my $proxyaddress="192.168.1.253";
my $proxyport=8080;
my $proxy_user="user";
my $proxy_pass="password";

# Initialisation de Curl
my $curl = WWW::Curl::Easy->new or die "curl init failed!\n err: $!\n";

# Options de connexion
$curl->setopt(CURLOPT_HEADER,1);
$curl->setopt(CURLOPT_URL, $url);
$curl->setopt(CURLOPT_PROXY, $proxyaddress);
$curl->setopt(CURLOPT_PROXYPORT, $proxyport);
$curl->setopt(CURLOPT_PROXYUSERPWD, $proxy_user.':'.$proxy_pass);
$curl->setopt(CURLOPT_SSL_VERIFYHOST, 0);
$curl->setopt(CURLOPT_SSL_VERIFYPEER, 0);

# A filehandle, reference to a scalar or reference to a typeglob can be used here.
my $response_body;
$curl->setopt(CURLOPT_WRITEDATA,\$response_body);

# Starts the actual request
my $retcode = $curl->perform;

# Looking at the results...
if ($retcode == 0) {
		my $response_code = $curl->getinfo(CURLINFO_HTTP_CODE);
		# judge result and next action based on $response_code
		# print("$response_body\n");
		
		# On recupere les cookies
		$response_body =~ m{Set-Cookie: (.*)}i; 
		my $cookies=$1;
		
		# Si le cookie correspond à un cookie F5, exemple : LBRPSBPLC=2263487148.3013.0000
		if($cookies =~ m{LBRPSBPLC=([0-9]*)\.([0-9]*)\.([0-9]*);}i) {
				print "F5 Found at $url !\n";
				# Conversion en chaine hexa
				my $lbip=sprintf("%x",$1);
				my $lbport=sprintf("%x",$2);
				
				# Tableau de paires de caracteres hexa
				my @hexlbip = ($lbip =~ /(..)/g);
				
				# Si l'IP possede 4 octets, elle est valide
				if(scalar @hexlbip==4) { 
					print "IP Backend : ".sprintf("%d",hex $hexlbip[3]).".".sprintf("%d",hex $hexlbip[2]).".".sprintf("%d",hex $hexlbip[1]).".".sprintf("%d",hex $hexlbip[0])."\n";
				}
				
				# On affiche le port
				my @hexlbport = ($lbport =~ /(..)/g);
				my $port='';
				foreach(@hexlbport) {
					$port=sprintf("%d",hex $_).$port;
				}
				print "Port Backend : ".$port."\n\n";
		}
} 
else {
		# Error code, type of error, error message
		print("An error happened: $retcode ".$curl->strerror($retcode)." ".$curl->errbuf."\n");
}
