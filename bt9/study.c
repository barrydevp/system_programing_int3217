#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

extern char **environ;

int main() {

    printf("This process is identified by: \n");
    printf("UID=%d, GID=%d\n", getuid(), getgid());
    printf("EUID=%d, EGID=%d\n", geteuid(), getegid());

    struct passwd *pw;

    pw = getpwnam("barrya");

    if (pw != 0) {
        printf("Detail for '%s':\n", pw->pw_name);
        printf("pw_passwd = %s\n", pw->pw_passwd);
        printf("pw_uid = %d\n", pw->pw_uid);
        printf("pw_gid = %d\n", pw->pw_gid);
        printf("pw_gecos = %s\n", pw->pw_gecos);
        printf("pw_dir = %s\n", pw->pw_dir);
        printf("pw_shell = %s\n", pw->pw_shell);
    }

    printf("%s\n", environ[0]);
    printf("%d\n", __LINE__);

    printf("%d\n", (int)(strspn("alo 123", " ")));

    return 0;
}
