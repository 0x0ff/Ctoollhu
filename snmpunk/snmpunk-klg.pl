#!/usr/bin/env perl
use Net::SNMP::Security::USM 4.0;
use warnings;
use strict;

sub myHelp() {
		printf "\n _____ _____ _____ _____         _____ \n";
		printf "|   __|   | |     |  _  |_ _ ___|  |  |\n";
		printf "|__   | | | | | | |   __| | |   |    -|\n";
		printf "|_____|_|___|_|_|_|__|  |___|_|_|__|__| Key List Generator v1.0\n\n";    
        printf "snmpunk-klg.pl <authProto> <wordList> <msgAuthoritativeEngineID> [privProto]\n";
        printf "   - authProto : md5|sha\n";
        printf "   - wordList : wordlist file\n";
        printf "   - msgAuthoritativeEngineID : field in SNMPv3 packet\n";
        printf "   - privProto : des|des3|aes\n\n";
        exit 1;
}

if(@ARGV<3 || @ARGV>4) {
        myHelp();
}

open (MYFILE, $ARGV[1]);
while (<MYFILE>) {
	$_ =~ s/\r|\n//g;
			my ($usm,$err) = Net::SNMP::Security::USM->new(
					-authoritative => 1,
					-username      => '0x0ff',
					-authprotocol  => $ARGV[0],
					-authpassword  => $_,
					-engineid      => $ARGV[2],
					-privprotocol  => (@ARGV > 3) ? $ARGV[3] : 'des',
					-privpassword  => $_);
			if (!defined $usm) {
					printf "$err\n";
					exit(1);
			}
			# Without privProto
			if(@ARGV<4) {
					printf "%s:%s:00\n", $_, unpack('H*', $usm->auth_key());
			}
			# With privProto
			else {
					printf "%s:%s:%s\n", $_, unpack('H*', $usm->auth_key()), unpack('H*', $usm->priv_key());
			}
}
