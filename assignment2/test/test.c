
#include <stdio.h>
#include <string.h>


int main() {
	char str[200];
	for (int i = 0; i < 10; ++i)
	{
		scanf("%[^\n]",str);
	
		printf("%s\n",str);
	}
}
