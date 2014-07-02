#include <stdio.h>
 
int main (int argc, char** argv)
{
    char execBinaryUser[280];

    system("mv /tmp/pig/pig /usr/bin/pig");
    system("chmod +x /usr/bin/pig");    
    system("chown root.root /usr/bin/pig");    
    sleep(2);
		sprintf(execBinaryUser, "gksu -u %s pig WITHOUT_UPDATE", argv[1]);
    system(execBinaryUser);

    return 0;
}



