#include<my_global.h>
#include<stdio.h>
#include<mysql.h>
using namespace std;
int main(int argc,char **argv)
{
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int num_fields;
	int i;
	conn = mysql_init(NULL);
	mysql_real_connect(conn,"localhost","root","redhat","test",3306,NULL,0);
mysql_query(conn, "SELECT * FROM new_table");
result=mysql_store_result(conn);
num_fields=mysql_num_fields(result);
while((row=mysql_fetch_row(result)))
{
	for(i=0;i<num_fields;i++)
	{
		printf("%s",row[i]?row[i]:"NULL");
	}
	printf("\n");
}
	
mysql_free_result(result);
mysql_close(conn);	
}


//compiled with  g++ mysql_connector.cpp -o test `mysql_config --cflags --libs`

