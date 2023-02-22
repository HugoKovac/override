int main(){

	int input

	puts("***********************************\n");
	puts("*            -Level00 -           *\n");
	puts("***********************************\n");
	printf("Password:");
	scanf("%d", input);

	if (input != 5276){
		puts("Invalid Password!\n");
	}
	else{
		puts("Authenticated!\n");
		system("/bin/sh");
	}

	return 0;
}