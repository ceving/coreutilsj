#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <jv.h>
#include <time.h>

#define PROGRAM "statj"

#define PRETTY JV_PRINT_INDENT_FLAGS(2) | JV_PRINT_COLOR

char *file_type (mode_t mode)
{
  switch (mode & S_IFMT) {
  case S_IFBLK:  return "block device";
  case S_IFCHR:  return "character device";
  case S_IFDIR:  return "directory";
  case S_IFIFO:  return "FIFO/pipe";
  case S_IFLNK:  return "symlink";
  case S_IFREG:  return "regular file";
  case S_IFSOCK: return "socket";
  default:       return "unknown?";
  }
}

jv result;

#define SET_STRING(KEY,VALUE) {{                                        \
      result = jv_object_set (result,                                   \
                              jv_string (KEY),                          \
                              jv_string (VALUE));                       \
    }}

#define SET_NUMBER(KEY,VALUE) {{                                        \
      result = jv_object_set (result,                                   \
                              jv_string (KEY),                          \
                              jv_number ((double)VALUE));               \
    }}

int
main (int argc, char *argv[])
{
  struct stat sb;

  if (argc != 2) {
    fprintf (stderr, "Usage: %s <pathname>\n", PROGRAM);
    exit (EXIT_FAILURE);
  }

  char *file = argv[1];

  if (stat (file, &sb) == -1) {
    perror (PROGRAM);
    exit (EXIT_FAILURE);
  }

  result = jv_object ();

  SET_STRING("file", file);
  SET_NUMBER("device", sb.st_dev);
  SET_STRING("type", file_type (sb.st_mode));
  SET_NUMBER("inode", sb.st_ino);
  SET_NUMBER("mode", sb.st_mode);
  SET_NUMBER("links", sb.st_nlink);
  SET_NUMBER("uid", sb.st_uid);
  SET_NUMBER("gid", sb.st_gid);
  SET_NUMBER("block-size", sb.st_blksize);
  SET_NUMBER("size", sb.st_size);
  SET_NUMBER("blocks", sb.st_blocks);

  char ts[sizeof "YYYY-MM-DDTHH:MM:SSZ"];

  strftime (ts, sizeof ts, "%FT%TZ", gmtime(&sb.st_ctime));
  SET_STRING("ctime", ts);
  strftime (ts, sizeof ts, "%FT%TZ", gmtime(&sb.st_atime));
  SET_STRING("atime", ts);
  strftime (ts, sizeof ts, "%FT%TZ", gmtime(&sb.st_mtime));
  SET_STRING("mtime", ts);

  jv_show (result, PRETTY);
  printf ("\n");

  return 0;
}
