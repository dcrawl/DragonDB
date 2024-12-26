# Overview
DragonDB is a very basic SQLite wrapper for DragonRuby.  Adds a new Ruby module to the GTK call DragonDB (GTK::DragonDB) which contains the following methods:

## GTK::DragonDB.sqlite_open(filename)

| Argument | Description |
| -- | -- |
| filename | a string that represents the relative path starting at the "mygame" folder. |

This method returns a ```true``` object if the database was opened successfully or ```nil``` otherwise.  Currently only one database can be opened at a time.  If it is necessary to open more than one database at a time, then I may expand this in the future.

## GTK::DragonDB.sqlite_close

This method simply closes and frees up the resources associated with database.

## GTK::DragonDB.sqlite_exec(sql)

| Argument | Description |
| -- | -- |
| sql | an SQL query string |

This method executes an SQL statement.  If the statement succeeded, a ```true``` object will be returned and ```nil``` otherwise.  This method is meant for statements that do not return a result set.

## GTK::DragonDB.sqlite_query(sql)

| Argument | Description |
| -- | -- |
| sql | an SQL query string |

This method executes an SQL query which returns an array of arrays to form a table of the requested data.  If the statement fails for whatever reason, a ```nil``` is returned.


# Compiling
The extension is expected to be loaded from the mygame/native/$PLATFORM folder.  Currently, I'm using ZIG to cross-compile to multiple platforms.  I have only added MacOS at the moment.  You will have to add your own platform if it is not present.

