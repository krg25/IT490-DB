# Database Information

On the database computer:

Run "sudo mysql -u root"

mysql> source <path>\script.sql
  
Mysql will now generate our tables, but they will not have data

If you want to remove the database to start over the syntax is "drop database <name>" but only use this in testing, this *will* delete your database and is irreversible.

Then simply run "./server.php"
