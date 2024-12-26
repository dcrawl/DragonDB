$gtk.ffi_misc.gtk_dlopen("dragondb")

def boot args
  # Open an SQLite database (Only one database is currently supported)
  # The database will be created if it does not exist
  rc = GTK::DragonDB.sqlite_open("mygame/data/test.db")
  unless rc.nil? then
    # Query and store the message
    message = GTK::DragonDB.sqlite_query("SELECT comment FROM WelcomeData")
    unless message.nil? then
      # First column of the first row
      args.state.message = message
    else
      args.state.message = ["No message found"]
    end

    # Close the database
    GTK::DragonDB.sqlite_close
  end
end

def tick args
  i = 0
  args.state.message.each { |msg|
    args.outputs.labels << [640, 360 - i*48, msg, 5, 1, 0, 0, 0]
    i+=1
  }
end
