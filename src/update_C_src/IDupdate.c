#include <stdio.h>
 
int main (int argc, char** argv)
{
    char execBinaryUser[280];

    system("mv /tmp/ID /usr/bin/ID");
    system("chmod +x /usr/bin/ID");    
    system("chown root.root /usr/bin/ID");    
    sleep(2);
		sprintf(execBinaryUser, "gksu -u %s ID WITHOUT_UPDATE", argv[1]);
    system(execBinaryUser);

    return 0;
}



