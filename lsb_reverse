#!/usr/bin/env perl
use Math::BigInt;

$mask=0x01;

$num_args = $#ARGV;
if ($num_args !=0)
{
   print("\nUsage: ./mysteg.pl gile.bmp\n");
   exit;
}

open (MYBMP, $ARGV[0]);
read(MYBMP,$head,54);
#Variable qui nous permet de sauter le bit inutile de chaque pixel
$rgb=4;
#Variable qui nous permet de multiplier le bit de poid faible
$nbit=128;
#Buffer de creation de chaque octet extrait
$buffer=0;
#Message a decouvrir
$message='';
#Variable de check
$count1=0;
$count2=0;
while (read(MYBMP,$byte,1))
{
   $count1+=1;
   #une fois sur quatre on saute l'octet
   $rgb-=1;
   if($rgb!=0)
   {
    #extraction du lsb
    $lsb=(ord($byte) & $mask);
    #creation de l'octet
    $buffer=$buffer+$lsb*$nbit;
    $nbit/=2;
    #Lorsque l'octet est complet
    if($nbit<1)
    {
      if($buffer>255)
      {
        print "\n\n ! ERROR ERROR ERROR ! \n\n";
      }
      $count2+=1;
      #On recupere le caractere
      $message=$message.chr($buffer);
      #On recupere quand meme le lsb (pas fou!!!)
      $nbit=128;

      $buffer=0;
    }
   }
   else
   {
    $rgb=4;
   }
   #print "byte:".ord($byte)."\n";
   #print "mask:".$mask."\n";
   #print "band:".(ord($byte) & $mask)."\n";
}
#On oublie pas le dernier octet
if($nbit>128)
{
   $count2+=1;
   if($buffer>255)
   {
    print "\n\n ! ERROR ERROR ERROR ! \n\n";
   }
   $message=$message.chr($buffer);
}
print $message."\n";
print $count1."\n";
print $count2."\n";
