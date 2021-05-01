grep "ft_" ../srcs/cub3d.h | cut -f2- | tr -d "\t" | tr -d ";" | tr -d "\*" | cut -d\( -f1 | while read -r line ; do 
	echo "\t\tRUNNING $line"
	grep "$line" ../srcs/*.c
done

grep "ft_" ../srcs/raycast.h | cut -f2- | tr -d "\t" | tr -d ";" | tr -d "\*" | cut -d\( -f1 | while read -r line ; do 
	echo "\t\tRUNNING $line"
	grep "$line" ../srcs/*.c
done

grep "ft_" ../srcs/parser.h | cut -f2- | tr -d "\t" | tr -d ";" | tr -d "\*" | cut -d\( -f1 | while read -r line ; do 
	echo "\t\tRUNNING $line"
	grep "$line" ../srcs/*.c
done
