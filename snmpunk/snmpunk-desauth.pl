#!/usr/bin/env perl
use warnings;
use strict;
use Math::BigInt;
use Digest::MD5 qw(md5_hex);
use Digest::SHA qw(sha1_hex);
sub myHelp() {
		printf "\n _____ _____ _____ _____         _____ \n";
		printf "|   __|   | |     |  _  |_ _ ___|  |  |\n";
		printf "|__   | | | | | | |   __| | |   |    -|\n";
		printf "|_____|_|___|_|_|_|__|  |___|_|_|__|__| Desauthenticator v1.0\n\n";    
        printf "snmpunk-desauth.pl <keyList> <msgAuthenticationParameters> <wholeMsg> <authProto>\n";
        printf "   - keyList : made with snmpunk-klg.pl\n";
        printf "   - msgAuthenticationParameters : a field in SNMPv3 packet\n";
        printf "   - wholeMsg : whole the data part of a SNMPv3 packet\n";
        printf "   - authProto : md5|sha\n\n";
        exit 1;
}

if(@ARGV<4 || @ARGV>5) {
        myHelp();
}

# Some variables
my $ipad = Math::BigInt->new('0x'.'36'x64);
my $opad = Math::BigInt->new('0x'.'5c'x64);
my $authKey;
my $pass;
my $cryptKey;
my $privKey;
# wholeMsg Initial creation (with blank msgAuthenticationParameters)
$ARGV[2] =~ s/$ARGV[1]/000000000000000000000000/g;
$ARGV[2] =~ s/\r|\n//g;
# Brute-force
open (MYKEYS, $ARGV[0]);
while (<MYKEYS>) {
        #Extract(pass:key) from file
        $_ =~ s/\r|\n//g;
                ($pass,$authKey,$privKey) = split(':',$_);
                # SNMPv3 Authentification Algo
                if($ARGV[3] eq 'md5') {
                        my $extAuthKey = Math::BigInt->new('0x'.$authKey.'00'x48);
                        my $md5k1 = md5_hex(pack('H*', substr $extAuthKey->copy()->bxor($ipad)->as_hex().$ARGV[2],2)); # Without copy(), bxor() modify the first argument
                        my $md5k2 = md5_hex(pack('H*', substr $extAuthKey->bxor($opad)->as_hex().$md5k1,2));
                        # Only the first 12 bytes are significants
                        if(substr($md5k2,0,24) eq $ARGV[1]) {
                                        print "=========== Key Found ! ===========\n";
                                        print "pass : $pass\n";
                                        print "authKey : $authKey\n";
                        }
                }
                elsif($ARGV[3] eq 'sha') {
                        my $extAuthKey = Math::BigInt->new('0x'.$authKey.'00'x44);
                        my $shak1 = sha1_hex(pack('H*', substr $extAuthKey->copy()->bxor($ipad)->as_hex().$ARGV[2],2)); # Without copy(), bxor() modify the first argument
                        my $shak2 = sha1_hex(pack('H*', substr $extAuthKey->bxor($opad)->as_hex().$shak1,2));
                        # Only the first 12 bytes are significants
                        if(substr($shak2,0,24) eq $ARGV[1]) {
                                        print "=========== Key Found ! ===========\n";
                                        print "pass : $pass\n";
                                        print "authKey : $authKey\n";
                        }
                }
                else {
                        printf "Bad authProto ! Should be md5 or sha...\n";
                        exit(1);
                }
}
