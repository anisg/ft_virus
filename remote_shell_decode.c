int main(void)
{
	char buff;
	int key = 1234;

	while (read(0, &buff, 1) == 1)
	{
		buff ^= key;
		write(1, &buff, 1);
		key = (key * 16807) % 2147483647;
	}
	return (0);
}
