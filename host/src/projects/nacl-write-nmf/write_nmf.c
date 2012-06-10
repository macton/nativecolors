#include <stdio.h>

int main( int argc, char** argv )
{
  if ( argc < 2 )
  {
    printf("Usage: nacl-write-nmf <nexe_name>\n");
    return (-1);
  }

  char* nexe_name = argv[1];

  printf("{\n");
  printf("  \"program\": {\n");
  printf("    \"x86-32\": {\"url\": \"/usr/bin/newlib-x86-32/%s\"},\n", nexe_name );
  printf("    \"x86-64\": {\"url\": \"/usr/bin/newlib-x86-64/%s\"}\n", nexe_name );
  printf("  }\n");
  printf("}\n");

  return (0);
}
