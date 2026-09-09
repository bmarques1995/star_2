fun square(x#u64)#u64
{
	return x * x;
}

fun dynamic_fun()
{
	return "Olá";
}

print(square(5u64));
print(dynamic_fun());