#include "ptoc.h"

#define __sezunit_implementation__


#include "sezunit.h"


struct markertype {
              word length;
              longint offset;
              byte checksum;
};

untyped_file f;
longint number;
markertype marker;
word fv;
byte sum;

void sez_setup()     /* This procedure sets up the Sez system (obviously!) */
{;

  /* Set up variables */

 fillchar(chain,sizeof(chain),'\261'); /* blank out gunk in "chain" */
 chainsize=0;  /* it's empty (for now...) */
 sezerror=sezok;  /* everything's fine! */

  /* Set up AVALOT.SEZ */

 assign(f,"avalot.sez"); reset(f,1);
 seek(f,255); blockread(f,sezhead,sizeof(sezhead));
 if (ioresult!=0)  {; sezerror=sezgunkyfile; return; }         /* too short */
 {;
  if ((sezhead.initials!="TT") || (sezhead.gamecode!=thisgamecode)
   || (sezhead.revision!=thisvercode)) 
  {;
   sezerror=sezgunkyfile;  /* not a valid file */
   return;
  }
 }
}

byte sumup()
{
    word fv; byte total;
byte sumup_result;
;
 total=0;
 for( fv=0; fv <= chainsize; fv ++)
 {;
  total += ord(chain[fv]);
 }
 sumup_result=total;
return sumup_result;
}

void getchain(longint number)
{;
 seek(f,longint(262)+number*longint(7)); blockread(f,marker,7);
 {;
  seek(f,longint(270)+sezhead.chains*longint(7)+marker.offset);
  blockread(f,chain,marker.length+1);
  for( fv=0; fv <= marker.length; fv ++) chain[fv] -= 3+177*fv*marker.length; /* unscramble */
  chainsize=marker.length;
  if (sumup()!=marker.checksum) 
  {;
   sezerror=sezhacked;
   return;
  }
 }
 close(f);
 sezerror=sezok;  /* nowt went wrong */
}

