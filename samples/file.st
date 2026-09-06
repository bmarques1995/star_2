//Single line comment
/*
Multi line
comment
*/
var lang = `Star é uma LP`;
print(lang);

{
	var lang = `Star é uma LP de múltiplo encadeamento`;
	print(lang);
}

auto x = 8u32;

if(x == 10u32)
{
    print("x é 10");
}
else
{
	if(x < 10u32)
	{
		print("x é menor que 10");
	}
	else
	{
		print("x é maior que 10");
	}
}

print(lang);

auto x3 = 0u32;

while(x3 < 4u32)
{
	print(x3);
	x3 += 1;
}