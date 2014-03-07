#!/usr/bin/env perl
use warnings;
use strict;
use Crypt::CBC;
use Math::BigInt;
use Switch;

sub myHelp() {
		printf "\n _____ _____ _____ _____         _____ \n";
		printf "|   __|   | |     |  _  |_ _ ___|  |  |\n";
		printf "|__   | | | | | | |   __| | |   |    -|\n";
		printf "|_____|_|___|_|_|_|__|  |___|_|_|__|__| Decryptor v1.0\n\n";                                   	
        printf "snmpunk-decrypt.pl <keyList> <msgAuthoritativeEngineID> <msgPrivacyParameters> <encryptedMsgData> <cryptProto>\n";
        printf "   - keyList : made with whith snmpunk-dicomaker.pl\n";
		printf "   - msgAuthoritativeEngineID : a field in SNMPv3 packet\n";
        printf "   - msgPrivacyParameters : a field in SNMPv3 packet\n";
        printf "   - encryptedMsgData : encrypted field msgData.\n";
		printf "   - cryptProto : des|3des|aes\n\n";
        exit 1;
}

if(@ARGV!=5) {
        myHelp();
}

switch ($ARGV[4]) {
	case "des"	{ $ARGV[4] = "Crypt::DES" }
	case "DES"	{ $ARGV[4] = "Crypt::DES" }
#	case "aes"	{ print "Not implemented yet...\n" }
#	case "AES"	{ print "Not implemented yet...\n" }
#	case "3des"	{ print "Not implemented yet...\n" }
#	case "3DES"	{ print "Not implemented yet...\n" }
	else {
		print "cryptProto should be des|3des|aes\n";
		print "AES and 3DES are not implemented yet...\n";
		exit 1;
	}
}

# Some variables
my $pass;
my $authKey;
my $privKey;
# Brute-force
open (MYKEYS, $ARGV[0]);
while (<MYKEYS>) {
	# Get pass and keys from file (pass:authKey:privKey)
	$_ =~ s/\r|\n//g;
	($pass,$authKey,$privKey) = split(':',$_);
	
	#=============DEBUG BLOCK=============#
	#print "pass : ".$pass."\n";
	#print "key  : ".$authKey."\n";
	#print "Cryp : ".$privKey."\n";
	#=====================================#
	
	# Build varaibles
	my $iv = Math::BigInt->new('0x'.substr($privKey,-16));
	my $salt = Math::BigInt->new('0x'.$ARGV[2]);
	
	#=============DEBUG BLOCK=============#
	#print "preiv : ".$iv->as_hex()."\n";
	#=====================================#
	
	$iv->bxor($salt);
	
	#=============DEBUG BLOCK=============#
	#print "salt : ".$salt->as_hex()."\n";
	#print "iv : ".$iv->as_hex()."\n";
	#=====================================#
	
	$privKey = pack "H*", substr($privKey,0,16);
	$iv = pack "H*", substr($iv->as_hex(),2);
	
	#=============DEBUG BLOCK=============#
	#print "$privKey : ".$privKey."\n"; 
	#print "$iv  : ".$iv."\n";
	#print "$privKey (unpack) : ".unpack("H*", $privKey)."\n"; 
	#print "$iv (unpack)       : ".unpack("H*",$iv)."\n";
	#=====================================#
	
	# CBC Algo
	my $cipher = Crypt::CBC->new(	-key    => $privKey,
									-literal_key => 1,
									-iv		=> $iv,
									-header => "none",
									-padding=> "null",
									-cipher => $ARGV[4]);
	# Get plainText
	my $plainText = $cipher->decrypt(pack("H*",$ARGV[3]));
	$plainText = unpack "H*", $plainText;

	if (index($plainText, $ARGV[1]) != -1) {
		print "=========== Key Found ! ===========\n";
		print "Pass found : $pass!\n";
		printf "privKey : %s\n", unpack("H*",$privKey);
	} 
}
