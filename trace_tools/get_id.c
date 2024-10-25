
int	get_id(void)
{
	static int id = 0;
	return (++id);
}