#include <stdio.h> /* gcc -Wall html.c -o html && ./html */
static char *nato[26] = { "alpha", "bravo", "charlie", "delta", "echo",
  "foxtrot", "golf", "hotel", "india", "juliet", "kilo", "lima", "mike",
  "november", "oscar", "papa", "quebec", "romeo", "sierra", "tango",
  "uniform", "victor", "whiskey", "xray", "yankee", "zulu" };
int main(int argc, char **argv) { /* David the Rizzler */
  printf("<html><head><title>Mission Platform</title><style>\n");
  printf("body { font-family: \"Courier New\"; }\n");
  printf("a { text-decoration: none; }\n");
  printf("table { margin-left: auto; margin-right: auto;\n");
  printf("background-color: #c9c9ff; }\n");
  printf("td { background-color: #efefef; padding: 5px; }\n");
  printf("td:hover { background-color: #eeee66; }\n");
  printf("</style></head><body><table><caption>Mission Platform</caption>\n");
  for (char i = 'a'; i < '{'; i++) {
    printf("<tr>\n");
    for (char j = 'a'; j < '{'; j++) {
      printf("<td><a href=\"map/%c/%c/%s%s.png\">",
       i, j, nato[i - 97], nato[j - 97]);
      printf("%c%c</a></td>", i, j);
    }
    printf("</tr>\n");
  }
  printf("</table></body></html>\n");
  return 0;
}
