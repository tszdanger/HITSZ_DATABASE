import pyodbc

DRIVER = "MySQL ODBC 8.0 Unicode Driver"
SERVER = "localhost"
PORT = 3306
UID = "root"
PWD = "123456"
CONN = "DRIVER=%s;SERVER=%s,%s;UID=%s;PWD=%s;option=3" % (DRIVER, SERVER, PORT, UID, PWD)


my_conn = pyodbc.connect("DSN=test2")


my_cur = my_conn.cursor()
my_cur.execute("select * from employee")
a = my_cur.fetchall()
print(a)