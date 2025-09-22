#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *program_name = NULL;
char *HELP = NULL;
int DEBUG = 0;
int BYTE_SIZE = 4;

static struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"debug", no_argument, NULL, 'd'},
    {"size", required_argument, NULL, 's'},
    {"output", required_argument, NULL, 'o'},
    {NULL, 0, NULL, 0}};

char *strrev(char *stri)
{
  int l = strlen(stri);
  char *stro = malloc(l + 1);
  if (stro == NULL)
  {
    // TODO: handle malloc failures
    return NULL;
  }
  stro[l] = '\0';
  for (int i = 0; i < l; ++i)
  {
    stro[l - i - 1] = stri[i];
  }
  return stro;
}

void set_program_name(char *name)
{
  program_name = name;
}

void try_help()
{
  fprintf(stderr, "Try '%s --help' for more information.\n", program_name);
}

void usage(int status)
{
  if (status == EXIT_FAILURE)
  {
    try_help();
  }
  else
  {
    fprintf(stderr, "usage: test [--help] ");
  }
  exit(status);
}

void help()
{
  printf("Usage: %s [OPTIONS] FILE\n", program_name);
  printf("Options:\n");
  printf("  -h, --help       Show this help message\n");
  printf("  -d, --debug      Enable debug output\n");
  printf("  -s, --size       Configure byte group size\n");
  printf("  -o, --output     Specify output file name\n");
  printf("Examples:\n");
  printf("  %s main.c -o c.niam\n", program_name);
  exit(EXIT_SUCCESS);
}

void debugf(const char *fmt, ...)
{
  if (!DEBUG)
  {
    return;
  }
  va_list args;
  va_start(args, fmt);
  printf("debug: ");
  vprintf(fmt, args);
  va_end(args);
}

int ifd = -1, ofd = -1;
int main(int argc, char *argv[])
{
  set_program_name(argv[0]);
  int ch = -1;
  while ((ch = getopt_long(argc, argv, "dh", longopts, NULL)) != -1)
  {
    switch (ch)
    {
    case 'd':
      DEBUG = 1;
      break;
    case 'h':
      help();
      break;
    case 'o':
      if (access(optarg, F_OK) == 0)
      {
        int c;
        do
        {
          printf("warning: '%s' already exists; overwrite file [y/n]: ", optarg);
          c = getchar();
          if (c == '\n')
            continue;
          while (getchar() != '\n' && !feof(stdin))
            ;
        } while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');

        if (c == 'n' || c == 'N')
        {
          exit(EXIT_SUCCESS);
        }
      }
      ofd = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (ofd == -1)
      {
        fprintf(stderr, "%s: error: unable to open output file '%s'\n", program_name, optarg);
        usage(EXIT_FAILURE);
      }
      break;
    case 's':
      BYTE_SIZE = atoi(optarg);
      break;
    case 0:
      break;
    default:
      usage(EXIT_FAILURE);
    }
  }
  argc -= optind;
  argv += optind;
  if (argc == 0)
  {
    fprintf(stderr, "%s: error: expected filename\n", program_name);
    usage(EXIT_FAILURE);
  }
  char *ifn = argv[0];
  argv++;
  argc--;
  debugf("input filename - %s\n", ifn);
  if ((ifd = open(ifn, O_RDONLY)) == -1)
  {
    fprintf(stderr, "%s: error: unable to open '%s'\n", program_name, ifn);
    debugf("error code: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  debugf("opened input file\n");
  if (ofd == -1)
  {
    char *ofn = strrev(ifn);
    int olen = 0;
    while (olen < strlen(ofn) && ofn[olen] != '/')
      olen++;
    if (olen != strlen(ofn) - 1)
      ofn[olen] = '\0';
    debugf("output filename - %s\n", ofn);
    char c;
    do
    {
      printf("write to '%s' [y/n]: ", ofn);
      c = getchar();
      if (c == '\n')
        continue;
      while (getchar() != '\n' && !feof(stdin))
        ;
    } while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');
    if (c == 'n')
    {
      printf("canceled\n");
      exit(EXIT_SUCCESS);
    }
    if ((ofd = open(ofn, O_WRONLY | O_CREAT, 0644)) == -1)
    {
      fprintf(stderr, "%s: error: unable to open '%s'\n", program_name, ofn);
      debugf("error code: %d", errno);
      exit(EXIT_FAILURE);
    }
    free(ofn);
  }
  int blen = -1;
  char bb[BYTE_SIZE * 1024];
  while ((blen = read(ifd, bb, sizeof(bb))) > 0)
  {
    if (blen % BYTE_SIZE != 0)
    {
      fprintf(stderr, "%s: error: bytes in input file not multiple of byte group size (%d)\n", program_name, BYTE_SIZE);
      usage(EXIT_FAILURE);
    }

    for (int i = 0; i < blen; i += BYTE_SIZE)
    {
      for (int j = 0; j < BYTE_SIZE / 2; j++)
      {
        char temp = bb[i + j];
        bb[i + j] = bb[i + BYTE_SIZE - j - 1];
        bb[i + BYTE_SIZE - j - 1] = temp;
      }
    }
    debugf("writing %d\n", blen);
    write(ofd, bb, blen);
  }
  printf("done\n");
  return 0;
}
