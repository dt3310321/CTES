#include "my-include.h"


#ifdef MAPPING_CHDIR
char mapped_path[ MAXPATHLEN ] = "/";

char *
#ifdef __STDC__
mapping_getwd(char *path)
#else
     mapping_getwd( path )
     char *path;
#endif
{

  printf("Copying %d chars into buffer path[] whose size = %d\n", strlen(mapped_path) + 1, MAXPATHLEN + 1); 
  
  /* BAD */
  strcpy( path, mapped_path );    /* copies mapped_path to path without doing a size check */
  return path;
}

/* Make these globals rather than local to mapping_chdir to avoid stack overflow */
char pathspace[ MAXPATHLEN ];                     /* This buffer can get overflowed too */
char old_mapped_path[ MAXPATHLEN ];

void
#ifdef __STDC__
/* appends /dir to mapped_path if mapped_path != /, else appends simply dir */
do_elem(char *dir)            
#else
     do_elem( dir )
     char *dir;
#endif
{
  /* . */
  if( dir[0] == '.' && dir[1] == '\0' ){
	/* ignore it */
	return;
      }
      
      /* .. */
      if( dir[0] == '.' && dir[1] == '.' && dir[2] == '\0' ){
              char *last;
              /* lop the last directory off the path */
              if (( last = strrchr( mapped_path, '/'))){
		/* If start of pathname leave the / */
		if( last == mapped_path )
		  last++;
		*last = '\0';
              }
              return;
      }
      
      /* append the dir part with a / unless at root */
      if( !(mapped_path[0] == '/' && mapped_path[1] == '\0') )
	/* BAD */
	strcat( mapped_path, "/" );                                /* no bounds checking is done */
      /* We do not check to see if there is room in mapped_path for dir */
      /* BAD */ 
      strcat( mapped_path, dir );     /* no bounds checking is done */
}

int
#ifdef __STDC__
mapping_chdir(char *orig_path)
#else
mapping_chdir( orig_path )
      char *orig_path;
#endif
{
      int ret;
      char *sl, *path;

      printf("Entering mapping_chdir with orig_path = %s\n", orig_path);

      strcpy( old_mapped_path, mapped_path );  /* old_mapped_path is initially / */
      path = &pathspace[0];
      
      /* BAD */
      strcpy( path, orig_path );                /* suppose path = orig_path = /x/xx/xxx/xxxx/... */
      printf("Copying orig_path to path....max strlen(path) = %d. strlen(path) = %d\n", MAXPATHLEN - 1, strlen(path));
      if (strlen(path) >= MAXPATHLEN){
	printf ("ALERT:pathspace[MAXPATHLEN] has been overflowed!\n");
      }
	
	/* set the start of the mapped_path to / */
	if( path[0] == '/' ){
	  mapped_path[0] = '/';
	  mapped_path[1] = '\0';
	  path++;
	}
	
      printf("so far mapped_path = %s\n", mapped_path);

      while( (sl = strchr( path, '/' )) ){
              char *dir;
              dir = path;
              *sl = '\0';
              path = sl + 1;
              if( *dir )
		do_elem( dir );    /* appends directory names to mapped_path */
              if( *path == '\0' )
		break;
      }
      if( *path )
	{
	  printf("path = %s.. calling do_elem\n", path);
	  do_elem( path );       /* we're in root and path is of the form aaaaa ... mapped_path becomes /aaaa.. */
	}
      printf("mapped_path = %s\n", mapped_path);
      if (strlen(mapped_path) >= MAXPATHLEN){
	printf("ALERT: mapped_path[MAXPATHLEN] has been overflowed!\n");
      }

      
      if( (ret = chdir( mapped_path )) < 0 ){        /* change to the specified path */
	printf("couldn't chdir to %s !\n", mapped_path);
	strcpy( mapped_path, old_mapped_path );   /* change mapped_path back to original, i.e root */
        printf("mapped_path changed to %s\n", mapped_path);
      }

      return ret;
}


/* From now on use the mapping version */

#define getwd(d) mapping_getwd(d)
#define getcwd(d,u) mapping_getwd(d)  

#endif /* MAPPING_CHDIR */



/* Define pwd */

void
#ifdef __STDC__
pwd(void)
#else
pwd()
#endif
{
  int canary = 7;                 /* used to see if path[] gets overflowed */
  char path[MAXPATHLEN + 1];      /* Path to return to client */
  
#ifndef MAPPING_CHDIR
#ifdef HAVE_GETCWD
  extern char *getcwd();
#else
#ifdef __STDC__
  extern char *getwd(char *);
#else
  extern char *getwd();
#endif
#endif
#endif /* MAPPING_CHDIR */
  
#ifdef HAVE_GETCWD
  if (getcwd(path,MAXPATHLEN) == (char *) NULL)   /* mz: call to mapping_getwd might overflow path */
#else
    if (getwd(path) == (char *) NULL)              /* mz: call to mapping_getwd might overflow path buf */  
#endif
      {
	printf("Couldn't get current directory!\n");
      }
    else{
      printf("Current directory = %s\n", path);
      printf("max strlen(path) is %d, strlen(path) = %d\n", MAXPATHLEN-1, strlen(path));
      printf("Canary should be 7.  Canary = %d\n", canary);
      if (canary != 7)
	printf("ALERT: path[MAXPATHLEN + 1] has been overflowed!\n");
    }
}


int main(int argc, char **argv){

  char orig_path[MAXPATHLEN + 20];
  FILE *f;

  assert (argc == 2);
  f = fopen(argv[1], "r");
  assert(f != NULL);

  fgets(orig_path, MAXPATHLEN + 20, f);  /* get path name */
  fclose(f);

  printf("orig_path = %s\n", orig_path);

  mapping_chdir(orig_path);   /* this overflows mapped_path[] and pathspace[] */
  pwd();            /* get current working directory.. this calls getwcd = mapping_getwd*/
                    /* mapping_getwd overflows path[] */
 

  return 0;

}

/*

</source>

*/

